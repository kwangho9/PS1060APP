#include <WiFiClient.h>
//#include <ThingSpeak.h>

#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <DNSServer.h>

#include <SPI.h>

#include <FS.h>

#define _I2C_LCD    1
#define _I2C_KEYPAD    1

#include <Wire.h>
#if _I2C_LCD
#include <LiquidCrystal_I2C.h>
#endif

#if _I2C_KEYPAD
#include <Keypad_I2C.h>
#include <Keypad.h>
#endif

//#include "index.h"

// WiFi & Network define.
char ssid[21] = "DS1_2G";
char pass[21] = "ds12ds!@";
//const char *ssid = "DS1_2G";
//const char *password = "ds12ds!@";

const char *APssid = "PS1060";
const char *APpassword = "12345678";

const byte DNS_PORT = 53;
IPAddress APip(192,168,5,1);
DNSServer dnsServer;

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

#if _I2C_LCD
LiquidCrystal_I2C lcd(0x20, 16, 2);
#endif

const uint8_t c_GroupType[] = {4, 6, 8, 10, 12, 16};

const char *deviceName[] = {"PS1030", "PS1060", "PS1120", "NoDevice"};

const byte BitMask[] = {0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01};
const byte SFR_ADDR[] = {0x60, 0x62, 0x64, 0x66, 0x68, 0x84, 0x86, 0x90, 0x92, 0x96};

#if _I2C_KEYPAD
#define I2CADDR     0x21
const byte ROWS = 4;    // four rows
const byte COLS = 4;    // four columns
const char keys[ROWS][COLS] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'F', '0', 'E', 'D'},
};

// Digitran keypad, bit numbers of PCF8574 i/o port
byte rowPins[ROWS] = {0, 1, 2, 3}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {4, 5, 6, 7}; //connect to the column pinouts of the keypad

Keypad_I2C kpd( makeKeymap(keys), rowPins, colPins, ROWS, COLS, I2CADDR, PCF8574 );
#endif

int gMaxChip = 0;       // '1'로 고정되어 있음.
uint8_t command[21];    // Sync + Cmd + CS + switch's + operate + shift + dummy -> SPI Tx buffer.
uint8_t result[32];     // SPI Tx Buffer.
uint8_t gDevice;
uint8_t gRow;
uint8_t gColumn;
uint8_t gTotalSwitch;
uint8_t gMaxBytes;
uint8_t gMaxFrame;
uint8_t gGroupType;
uint8_t gChipSelect;
uint8_t gChipAddr;

char buffer[128];       // Serial 또는 Http로 전송되는 Command Buffer.

uint8_t nBytes;
uint8_t Bytes[32];      // Serial 또는 KeyMatrix에서 String을 Hex로 변환할때 사용되는 Buffer.
//String msg = "";
uint8_t CurrentChip = 0;
uint8_t CurrentStatus[60];    // Device(1-bit) + ID(3-bit) + Group(3-bit) + Switch's(60-bytes)


#define CHIP_CTRL       (0x96)

#if 0
uint8_t bit_count(uint8_t bits, byte *bytes)
{
    uint8_t i;

    for(i=0; i<bits; i++) {
        cnt = 
    }
	bytes[pos >> 3] |= BitMask[pos & 0x7];
}
#endif

void set_bit(uint8_t pos, byte *bytes)
{
	bytes[pos >> 3] |= BitMask[pos & 0x7];
}

void clear_bit(uint8_t pos, byte *bytes)
{
	bytes[pos >> 3] &= ~BitMask[pos & 0x7];
}

uint8_t check_bit(uint8_t pos, byte *bytes)
{
	return (bytes[pos >> 3] & BitMask[pos & 0x7]);
}

uint8_t set_switch(uint8_t opcode, uint8_t sw)
{
    if( sw >= gTotalSwitch ) return -1;

    if( opcode == 0 ) {         // switch off
        if( CurrentStatus[sw] & (1<<1) ) return 0;
        CurrentStatus[sw] = 0x0;
    } else if( opcode == 1 ) {  // switch on
        if( CurrentStatus[sw] & (1<<1) ) return 0;
        CurrentStatus[sw] = 0x1;
    } else if( opcode == 2 ) {  // hold off
        if( CurrentStatus[sw] & (1<<1) ) return 0;
        CurrentStatus[sw] = 0x2;
    } else if( opcode == 3 ) {  // hold on
        if( CurrentStatus[sw] & (1<<1) ) return 0;
        CurrentStatus[sw] = 0x3;
    } else if( opcode == 4 ) {  // release off
        if( CurrentStatus[sw] & (1<<1) ) {
            CurrentStatus[sw] = 0x0;
        }
    } else if( opcode == 5 ) {  // release on
        if( CurrentStatus[sw] & (1<<1) ) {
            CurrentStatus[sw] = 0x1;
        }
    } else if( opcode == 6 ) {  // force off
        CurrentStatus[sw] = 0x0;
    } else if( opcode == 7 ) {  // force on
        CurrentStatus[sw] = 0x1;
    }

    return 0;
}

uint8_t groupIdx(uint8_t group)
{
    for(uint8_t i=0; i<6; i++) {
        if( group == c_GroupType[i] ) return i;
    }

    return 0;
}

uint8_t command_parser(uint8_t *cmd)
{
    uint8_t SwitchSelect[8];
    uint8_t idx = 1;
    uint8_t c = cmd[idx++];
    uint8_t type = (c & 0x38) >> 3;
    uint8_t op = (c & 0x07);

    if( c & (1 << 6) ) {       // chip address check.
        if( (cmd[idx++] & gChipSelect) == 0 ) return 0;
    }
    /*
     * Switch의 상태를 읽거나 Emulation해서 결과를 모든 web socket client들에게 전송한다.
     * - Device, ID, GroupType 등의 변경된 상태. -> 1-byte
     * - 각 Switch별 : on, off, hold on, hold off -> max 15-bytes(60 x 2-bits = 15-bytes)
     * Read Command에 대해서는 전송할 필요가 없다.
     */
    switch( type ) {
        case 0:         // All Switch.
            for(uint8_t i=0; i<gTotalSwitch; i++) {
                set_switch(op, i);
            }
            break;
        case 1:         // single Switch.
            set_switch(op, cmd[idx++]);
            break;
        case 2:         // Row All Switch.
            uint8_t row;
//            Serial.printf("%d Row All Switch : ", gRow);
            for(row=0; row<gRow; row++) {
                if( check_bit(row, &cmd[idx]) ) {   // MSB(upper row select) first check.
//                    Serial.printf("(%d)", row);
                    for(uint8_t sw=row; sw<gTotalSwitch; sw+=gRow) {
                        set_switch(op, sw);
//                        Serial.printf("%d ", sw);
                    }
                }
            }
//            Serial.println("\n");
            break;
        case 3:         // Row Each Switch.
//            Serial.printf("%d Row Each Switch : ", gRow);
            for(row=0; row<gRow; row++) {
                if( check_bit(row, &cmd[idx]) ) {   // MSB first check.
//                    Serial.printf("(%d)", row);
                    for(uint8_t i=0; i<gColumn; i++) {
                        if( check_bit(gRow+i, &cmd[idx]) ) {
                            uint8_t sw = row + (i * gRow);
                            set_switch(op, sw);
//                            Serial.printf("%d ", sw);
                        }
                    }
                }
            }
//            Serial.println("\n");
            break;
        case 4:         // Column All Switch.
            uint8_t col;
//            Serial.printf("%d Column All Switch : ", gColumn);
            for(col=0; col<gColumn; col++) {
                if( check_bit(col, &cmd[idx]) ) {   // MSB first check.
//                    Serial.printf("(%d)", col);
                    uint8_t start = col * gGroupType;
                    for(uint8_t sw=0; sw<gGroupType; sw++) {
                        if( sw >= gTotalSwitch ) break;
                            set_switch(op, sw+start);
//                            Serial.printf("%d ", sw+start);
                    }
                }
            }
//            Serial.println("\n");
            break;
        case 5:         // Column Each Switch.
            break;
        case 6:         // Special Switch.
            if( op == 1 ) {         // Set Group Type.
                gGroupType = c_GroupType[cmd[idx] >> 5];
                gRow = gGroupType;
                gColumn = (gTotalSwitch / gRow) + ((gTotalSwitch % gRow) ? 1 : 0);
                CurrentChip = (CurrentChip & 0xF8) | groupIdx(gGroupType);
            } else if( op == 2 ) {  // Software Reset.
            } else if( op == 3 ) {  // ...
            } else if( op == 4 ) {  // Active Discharge Disable.
                CurrentChip = (CurrentChip & 0xF7);
            } else if( op == 5 ) {  // Active Discharge Enable.
                CurrentChip = (CurrentChip | 0x08);
            } else if( op == 6 ) {  // ...
            } else if( op == 7 ) {  // Chip Address Assignment.
                uint8_t mask = cmd[idx];
                for(uint8_t i=0; i<8; i++) {
                    if( check_bit(i, &mask) ) {
                        gChipAddr = i;
                        break;
                    }
                }
                CurrentChip = (CurrentChip & 0x8F) | (gChipAddr << 4);
            }
        case 7:         // All Each Switch.
            for(uint8_t i=0; i<gTotalSwitch; i++) {
                if( check_bit(i, &cmd[idx]) )
                    set_switch(op, i);
            }
            break;
    }

//    ws.binaryAll(CurrentStatus, 65);

    String str = String(CurrentChip | (gDevice << 7));
//    String str = String(0xFF);
    for(uint8_t i=0; i<gTotalSwitch; i++) {
        str += " " + String(CurrentStatus[i]);
    }
    ws.textAll(str);

    return 0;
}



uint8_t transmit(byte *cmd, size_t cnt)
{
    uint8_t ret = 0;

#if 0   // Tx dump.
    Serial.print("TX: ");
    for(uint8_t k=0; k<cnt; k++) {
        Serial.printf("0x%02X ", cmd[k]);
    }
    Serial.println("");
#endif
    digitalWrite(SS, LOW);
    SPI.transferBytes(cmd, result, cnt);
    digitalWrite(SS, HIGH);

#if 0   // Rx dump.
    Serial.print("RX: ");
    for(uint8_t k=0; k<cnt; k++) {
        Serial.printf("0x%02X ", result[k]);
    }
    Serial.println("");
#endif

    if( cmd[1] & (1<<7) ) {         // Special Command.
        switch( cmd[1] ) {
            case 0xC0:              // Chip SFR Read.
                for(uint8_t k=0; k<8; k++) {
                    if( check_bit(gMaxChip+4+k, &result[3]) ) {
                        set_bit(k, &ret);
                    }
                }
                break;
            case 0xC1:
            case 0x81:              // Chip SFR Write.
                break;
            case 0xC2:              // Switch On/Off Read.
                break;
            case 0xC4:              // Switch Hold Read.
                break;
            case 0xF6:              // 1-Switch Status Read.
                for(uint8_t k=0; k<2; k++) {
                    if( check_bit(gMaxChip+3+k, &result[4]) ) {
                        set_bit(k+6, &ret);
                    }
                }
                break;
            case 0xFF:              // Pattern Reset.
                memset(CurrentStatus, 0, sizeof(CurrentStatus));
                break;
        }
    } else {                        // Switch Control Command.
        ret = cnt;
        /*
         * Switch의 상태를 읽거나 Emulation해서 결과를 모든 web socket client들에게 전송한다.
         * - Device, ID, GroupType 등의 변경된 상태. -> 1-byte
         * - 각 Switch별 : on, off, hold on, hold off -> max 15-bytes(60 x 2-bits = 15-bytes)
         * Read Command에 대해서는 전송할 필요가 없다.
         */
        if( cmd[1] == 0x37 ) {
            if( gMaxChip == 8 ) ret = result[3];
            else ret = (result[2] << gMaxChip) | (result[3] >> (8-gMaxChip));
        }
        command_parser(cmd);
    }

    return ret;
}

#define SWITCH_OFF          0
#define SWITCH_ON           1
#define HOLD_OFF            2
#define HOLD_ON             3
#define RELEASE_OFF         4
#define RELEASE_ON          5
#define FORCE_OFF           6
#define FORCE_ON            7

#define Bits2Bytes(n)           ((n >> 3) + ((n & 0x7) ? 1 : 0))

uint8_t SINGLE_CTRL(uint8_t chip, uint8_t op)
{
    uint8_t idx = 0;

    command[idx++] = 0xAA;
    if( chip == 0x0 ) {
        command[idx++] = 0x00 | op;
    } else {
        command[idx++] = 0x40 | op;
        command[idx++] = chip;
    }

    command[idx++] = 0;     // operation cycle.
    command[idx++] = 0;     // shift cycle.
    command[idx++] = 0;     // dummy cycle.

    return transmit(command, idx);
}

uint8_t SINGLE_SWITCH_CTRL(uint8_t chip, uint8_t op, uint8_t num)
{
    uint8_t idx = 0;

    command[idx++] = 0xAA;
    if( chip == 0x0 ) {
        command[idx++] = 0x08 | op;
    } else {
        command[idx++] = 0x48 | op;
        command[idx++] = chip;
    }

    command[idx++] = num;
    command[idx++] = 0;     // operation cycle.
    command[idx++] = 0;     // shift cycle.
    command[idx++] = 0;     // dummy cycle.

    return transmit(command, idx);
}

uint8_t ROW_ALL_SWITCH_CTRL(uint8_t chip, uint8_t op, uint8_t *select)
{
    uint8_t idx = 0;
    uint8_t n = Bits2Bytes(gRow);

    command[idx++] = 0xAA;
    if( chip == 0x0 ) {
        command[idx++] = 0x10 | op;
    } else {
        command[idx++] = 0x50 | op;
        command[idx++] = chip;
    }

    for(int i=0; i<n; i++) {
        command[idx++] = select[i];
    }
    command[idx++] = 0;     // operation cycle.
    command[idx++] = 0;     // shift cycle.
    command[idx++] = 0;     // dummy cycle.

    return transmit(command, idx);
}

uint8_t ROW_EACH_SWITCH_CTRL(uint8_t chip, uint8_t op, uint8_t *select)
{
    uint8_t idx = 0;
    uint8_t n = Bits2Bytes(gRow+gColumn);

    command[idx++] = 0xAA;
    if( chip == 0x0 ) {
        command[idx++] = 0x18 | op;
    } else {
        command[idx++] = 0x58 | op;
        command[idx++] = chip;
    }

    for(int i=0; i<n; i++) {
        command[idx++] = select[i];
    }
    command[idx++] = 0;     // operation cycle.
    command[idx++] = 0;     // shift cycle.
    command[idx++] = 0;     // dummy cycle.

    return transmit(command, idx);
}

uint8_t COLUMN_ALL_SWITCH_CTRL(uint8_t chip, uint8_t op, uint8_t *select)
{
    uint8_t idx = 0;
    uint8_t n = Bits2Bytes(gColumn);

    command[idx++] = 0xAA;
    if( chip == 0x0 ) {
        command[idx++] = 0x20 | op;
    } else {
        command[idx++] = 0x60 | op;
        command[idx++] = chip;
    }

    for(int i=0; i<n; i++) {
        command[idx++] = select[i];
    }
    command[idx++] = 0;     // operation cycle.
    command[idx++] = 0;     // shift cycle.
    command[idx++] = 0;     // dummy cycle.

    return transmit(command, idx);
}

uint8_t COLUMN_EACH_SWITCH_CTRL(uint8_t chip, uint8_t op, uint8_t *select)
{
    uint8_t idx = 0;
    uint8_t n = Bits2Bytes(gColumn+gRow);

    command[idx++] = 0xAA;
    if( chip == 0x0 ) {
        command[idx++] = 0x28 | op;
    } else {
        command[idx++] = 0x68 | op;
        command[idx++] = chip;
    }

    for(int i=0; i<n; i++) {
        command[idx++] = select[i];
    }
    command[idx++] = 0;     // operation cycle.
    command[idx++] = 0;     // shift cycle.
    command[idx++] = 0;     // dummy cycle.

    return transmit(command, idx);
}

uint8_t ALL_SWITCH_CTRL(uint8_t chip, uint8_t op, uint8_t *select)
{
    uint8_t idx = 0;
    uint8_t n = gMaxBytes;

    command[idx++] = 0xAA;
    if( chip == 0x0 ) {
        command[idx++] = 0x38 | op;
    } else {
        command[idx++] = 0x78 | op;
        command[idx++] = chip;
    }

    for(int i=0; i<n; i++) {
        command[idx++] = select[i];
    }
    command[idx++] = 0;     // operation cycle.
    command[idx++] = 0;     // shift cycle.
    command[idx++] = 0;     // dummy cycle.

    return transmit(command, idx);
}


uint8_t GROUP_TYPE_SELECT(uint8_t type)
{
    byte buf[] = {0xAA, 0x31, type << 5, 0, 0};

    return transmit(buf, sizeof(buf));
}

uint8_t SFR_READ(uint8_t chip, uint8_t addr)
{
    byte buf[] = {0xAA, 0xC0, (chip << 4)|((addr & 0xF0) >> 4), (addr & 0x0F) << 4, 0, 0, 0};

    return transmit(buf, sizeof(buf));
}

uint8_t CHIP_ADDRESS_ASSIGNMENT(uint8_t mask)
{
    byte buf[] = {0xAA, 0x37, mask, 0, 0};

    return transmit(buf, sizeof(buf));
}

uint8_t PATTERN_RESET(void)
{
    byte buf[22];

    for(uint8_t k=0; k<22; k++) {
        if( k < 18 ) buf[k] = 0xFF;
        else buf[k] = 0x0;
    }
    return transmit(buf, sizeof(buf));
}

uint8_t device_detect()
{
    Serial.println("Pattern Reset.");
    PATTERN_RESET();

    Serial.println("CHIP Address Assignment.");
    CHIP_ADDRESS_ASSIGNMENT(0xFF);

    for(uint8_t k=1; k<9; k++) {
        uint8_t tmp = (*(uint16_t *)&result[0] >> k) & 0xFF;
        Serial.printf("tmp = 0x%X\n", tmp);

        if( tmp == 0xAA ) {
            gMaxChip = k;
            break;
        }
    }

    uint8_t mask;
    mask = (result[2] << gMaxChip) | (result[3] >> (8 - gMaxChip));
    Serial.printf("Return ID Mask : 0x%X\n", mask);

    GROUP_TYPE_SELECT(3);

    for(uint8_t id=0; id<8; id++) {
        if( (mask & (1<<(7-id))) == 0 ) {
            uint8_t ch = SFR_READ(id, CHIP_CTRL);
            Serial.printf("\t%d. CHIP_CTRL(0x%X) : ", id, ch);
            if( ch & 0x80 ) {
                gDevice = 2;        // PS1120
                gTotalSwitch = 120;
                gMaxBytes = 15;         // max switch inform
                // sync + cmd + cs + switch + op + shift = 1 + 1 + 1 + 15 + 1 + 1 = 20
                gMaxFrame = 20;
                if( ch & 0x40 ) Serial.printf("Upper 120");
                else Serial.printf("Lower 120");
            } else {
                if( ch & 0x40 ) {
                    gDevice = 1;        // PS1060
                    gTotalSwitch = 60;
                    gMaxBytes = 8;
                    // sync + cmd + cs + switch + op + shift = 1+1+1+8+1+1 = 13
                    gMaxFrame = 13;
                    Serial.printf("60");
                } else {
                    gDevice = 0;        // PS1030
                    gTotalSwitch = 30;
                    gMaxBytes = 4;
                    // sync + cmd + cs + switch + op + shift = 1+1+1+4+1+1 = 9
                    gMaxFrame = 9;
                    Serial.printf("30");
                }
            }
            CurrentChip = (CurrentChip & 0x7F) | (gDevice << 7);
            Serial.printf("-channel, ");
            gGroupType = c_GroupType[ch & 0x7];
            gRow = gGroupType;
            gColumn = (gTotalSwitch / gRow) + ((gTotalSwitch % gRow) ? 1 : 0);
            Serial.printf("%d-group\n", gGroupType);
            gChipSelect |= (1 << (7-id));
        }
    }
    Serial.printf("%s device %d-chain detected.\n", deviceName[gDevice], gMaxChip);

    return gDevice;
}

void notFound(AsyncWebServerRequest *request) {
	int params = request->params();
    for(int i=0; i<params; i++) {
        AsyncWebParameter *p = request->getParam(i);
        if( p->isFile()) {
            Serial.printf("FILE[%s]: %s, size: %u\n", p->name().c_str(), p->value().c_str(), p->size());
        } else if(p->isPost()){
            Serial.printf("POST[%s]: %s\n", p->name().c_str(), p->value().c_str());
        } else {
            Serial.printf("GET[%s]: %s\n", p->name().c_str(), p->value().c_str());
        }
    }
//    Serial.print(millis());Serial.print(" : ");Serial.println(__func__);
    Serial.println(request->url());
    request->send(404, "text/plain", "Not found");
}


#define CharToDigit(c)      ((c < 'A') ? (c - '0') : (c - 'A' + 10))
uint8_t mode = 0;
uint8_t state = 0;
uint8_t HexDigit = 0;
uint8_t HexValue = 0;
uint8_t Value = 0;
uint8_t _x = 0;
uint8_t _y = 0;
#define SET_KEY     'D'
#define ENTER_KEY   'E'
#define MODE_KEY    'F'
void lcd_LineClear(uint8_t line)
{
    lcd.setCursor(0, line);
    lcd.print("                ");
    lcd.setCursor(0, line);
}

uint8_t nSW = 0;
unsigned long period = 10;
unsigned long startMillis;
uint8_t ctrl = 0;
uint8_t nLoop = 0;
uint8_t nRow = 0;
uint8_t nColumn = 0;
uint8_t gMAX_SWITCH = 30;


/************************************************************************
 * SPIFFS 관련 함수.
 */
String Name(String a) {
    String temp = "\"{v}\":";
    temp.replace("{v}", a);
    return temp;
}
String strVal(String a) {
    String temp = "\"{v}\",";
    temp.replace("{v}", a);
    return temp;
}
void stringTo(String ssidTemp, String passTemp) { // 스트링 SSID / PASS 배열에 저장
    for (int i = 0; i < ssidTemp.length(); i++)
        ssid[i] = ssidTemp[i];
    ssid[ssidTemp.length()] = '\0';
    for (int i = 0; i < passTemp.length(); i++)
        pass[i] = passTemp[i];
    pass[passTemp.length()] = '\0';
}
/* 데이터 저장 형식 - JSON 형식
   첫 번째, "스트링"처럼 큰 따옴표 기호(")로 묶인 스트링은 변수 명이고 고유하다.
   두 번째, 변수에 대한 값은 연이어 나오는':'문자 다음에 위치한다.
   세 번째, 값이 스트링이면 큰 따옴표(")로 묶이고, 값이 숫자(소수/정수)이면 큰 따옴표(")가 없다
   네 번째, 값 다음에 반드시 콤마(,)가 있다.
 */
// "SSID":"YOUR_SSID","PASS":"YOUR_PASS","ZONE":9.00,"SUMMER":0,
bool saveConfig()
{
    String value;
    value = Name("SSID") + strVal(ssid);
    value += Name("PASS") + strVal(pass);
//    value += Name("ZONE") + floatNum(timeZone);
//    value += Name("SUMMER") + intNum(summerTime);
    File configFile = SPIFFS.open("/config.txt", "w");
    if (!configFile) {
        Serial.println("Failed to open config file for writing");
        return false;
    }
    configFile.println(value); // SPIFF config.txt에 데이터 저장, '\n'포함
    configFile.close();
    return true;
}
String json_parser(String s, String a)
{
    String val;
    if (s.indexOf(a) != -1) {
        int st_index = s.indexOf(a);
        int val_index = s.indexOf(':', st_index);
        if (s.charAt(val_index + 1) == '"') { // 값이 스트링 형식이면
            int ed_index = s.indexOf('"', val_index + 2);
            val = s.substring(val_index + 2, ed_index);
        } else { // 값이 스트링 형식이 아니면
            int ed_index = s.indexOf(',', val_index + 1);
            val = s.substring(val_index + 1, ed_index);
        }
    } else {
        Serial.print(a);
        Serial.println(F(" is not available"));
    }
    return val;
}
bool loadConfig()
{
    File configFile = SPIFFS.open("/config.txt", "r");
    if (!configFile) {
        Serial.println("Failed to open config file");
        return false;
    }

    String line = configFile.readStringUntil('\n');
    Serial.println(line);
    configFile.close();
    String ssidTemp = json_parser(line, "SSID");
    String passTemp = json_parser(line, "PASS");
    stringTo(ssidTemp, passTemp); // String을 배열에 저장
//    String temp = json_parser(line, "ZONE");
//    timeZone = temp.toFloat(); // 스트링을 float로 변환
//    temp = json_parser(line, "SUMMER");
//    summerTime = temp.toInt(); // 스트링을 int로 변환
    return true;
}

/************************************************************************
 * Web Socket 관련 함수.
 */
void notifyClients() {
    ws.textAll(String("Server Message"));
}

void handleWebSocketMessage(void *arg, uint8_t *data, size_t len) {
    AwsFrameInfo *info = (AwsFrameInfo*)arg;
    if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
        data[len] = 0;

        // rx data
        Serial.println((char *)data);
//        if (strcmp((char*)data, "toggle") == 0) {
//            ledState = !ledState;
//            notifyClients();
//        }
    }
}

void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type,
             void *arg, uint8_t *data, size_t len) {
    String str;
    switch (type) {
        case WS_EVT_CONNECT:
            Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
            /*
               uint8_t content[3] = {10, 22, 43};
               client->binary(content, 3);
             */
            /* 현재 Switch 상태를 접속된 client에 전송한다.
             * ws.text(client->id(), "message");
             */
            str = String(CurrentChip | (gDevice << 7));
            for(uint8_t i=0; i<gTotalSwitch; i++) {
                str += " " + String(CurrentStatus[i]);
            }
            ws.text(client->id(), str);
            break;
        case WS_EVT_DISCONNECT:
            Serial.printf("WebSocket client #%u disconnected\n", client->id());
            break;
        case WS_EVT_DATA:
            handleWebSocketMessage(arg, data, len);
            break;
        case WS_EVT_PONG:
        case WS_EVT_ERROR:
            break;
    }
}

void initWebSocket() {
    ws.onEvent(onEvent);
    server.addHandler(&ws);
}

String processor(const String & var)
{
    Serial.print("processor() : ");
    Serial.println(var);
}


/***********************************************************************************
 */
void setup()
{
    int speed;

	Serial.begin(57600);
	while( !Serial ) continue;
	Serial.println("...");

    Wire.begin();

    // booting mode check
    pinMode(D0, INPUT);
//    Serial.printf("LED_BUILTIN = %d\n", LED_BUILTIN);
    if( digitalRead(D0) == HIGH ) {
        // keypad 8475 I/O input mode.
//        Wire.begin();
        pinMode(2, OUTPUT);
        digitalWrite(2, HIGH);
#if _I2C_KEYPAD
        kpd.begin( makeKeymap(keys) );
#endif
        for(int i=0; i<8; i++)
            kpd.pin_mode(i, INPUT);

        // LCD 8475 I/O input mode.
        Keypad_I2C kpd2( makeKeymap(keys), rowPins, colPins, ROWS, COLS, 0x20, PCF8574 );
        kpd2.begin( makeKeymap(keys) );
        for(int i=0; i<8; i++)
            kpd2.pin_mode(i, INPUT);

        // enter Deep Sleep mode.
        Serial.println("Enter Deep Sleep mode.");
        ESP.deepSleep(0);
        while(1);
    }
    pinMode(2, OUTPUT);
    digitalWrite(2, LOW);

#if _I2C_LCD
    lcd.begin(4, 5);        // Initialize I2C LCD module(SDA = GPIO4, SCL = GPIO5)
    lcd.backlight();        // Turn backlight ON
    lcd_LineClear(0);
    lcd_LineClear(1);
#endif

#if _I2C_KEYPAD
    kpd.begin( makeKeymap(keys) );
#endif

    Serial.println("Mounting FS ...");
    if( !SPIFFS.begin() ) {
        Serial.println("Failed to mount file system");
        return;
    }
    if( !SPIFFS.exists("/config.txt") ) {
        saveConfig();
    }
    loadConfig();
    Serial.println(ssid);
    Serial.println(pass);
//    SPIFFS.end();       // SPIFFS 파일 시스템을 마운트 해제한다.

	// AP mode
	Serial.print("\nSoftrawre AP ");
	WiFi.softAPConfig(APip, APip, IPAddress(255,255,255,0));
	if(!WiFi.softAP(APssid, APpassword)){
		Serial.println("failed");  
	} else {
		IPAddress myIP = WiFi.softAPIP();
		Serial.print(": IP address(");
		Serial.print(myIP);
		Serial.println(") started");
	}

	dnsServer.setTTL(60);		// 60 sec.
	dnsServer.setErrorReplyCode(DNSReplyCode::ServerFailure);
	dnsServer.start(DNS_PORT, "www.ps1060.co.kr", APip);

	// STATION mode
    int cnt = 0;
	WiFi.begin(ssid, pass);
	while ( WiFi.status() != WL_CONNECTED) {
		Serial.print(".");
		delay(500);
        if( ++cnt == 20 ) {
            Serial.println("");
            break;
        }
	}
	Serial.println(WiFi.localIP());

    initWebSocket();

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        String str = "device=" + String(gDevice);
        Serial.println("HTTP GET");
        AsyncWebServerResponse *response = request->beginResponse(SPIFFS, "/index.html", "text/html");
        response->addHeader("Set-Cookie", str);
        str = "group=" + String(gGroupType);
        response->addHeader("Set-Cookie", str);
//        Serial.println(str);

        request->send(response);
//        request->send(SPIFFS, "/index.html", "text/html");
#if 0
        // Cookie를 사용하기 위해서는 beginResponse_P(200, "text/html", ...)함수를 사용해야 한다.
        AsyncWebServerResponse *response = request->beginResponse_P(200, "text/html", index_html);
        response->addHeader("Set-Cookie", str);
        str = "group=" + String(gGroupType);
        response->addHeader("Set-Cookie", str);
//        Serial.println(str);

        request->send(response);
//        request->send_P(200, "text/html", index_html);
#endif
    });

    server.on("/jquery-3.4.1.min.js", HTTP_GET, [](AsyncWebServerRequest *request) {
        Serial.println("jquery GET");
        request->send(SPIFFS, "/jquery-3.4.1.min.js", "text/javascript");
    });

    /*
    server.on("/get_device", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(200, "text/plain", String(gDevice));
    });
    */

    /*
    server.on("/setGroup", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(204, "text/plain", "");
        uint8_t group = request->getParam("GROUP")->value().toInt();
        Serial.printf("[WiFi] Set Group = %d\n", group);           // 4, 6, 8, 10, 12, 16
        GROUP_TYPE_SELECT(groupIdx(group));
    });
    */

    server.on("/Command", HTTP_GET, [](AsyncWebServerRequest *request) {
#if 0
        int params = request->params();
        for(int i=0; i<params; i++) {
    		AsyncWebParameter *p = request->getParam(i);
            Serial.printf("Command -> GET[%s]: %s\n", p->name().c_str(), p->value().c_str());
        }
#endif
        String str = request->getParam("CMD")->value();
        byte bytes[32];
        int endIndex;
        int startIndex = 0;
        int n = 0;

        request->send(204, "text/plain", "");

        str.toCharArray(buffer, str.length());
//        Serial.println("CMD : " + str);

        char *token = strtok(buffer, " ");
        while( token != NULL ) {
            bytes[n++] = strtol(token, NULL, 16);
            token = strtok(NULL, " ");
        }

        transmit(bytes, n);
//        Serial.print("TX: ");
//        for(uint8_t k=0; k<n; k++) {
//            Serial.printf("0x%02X ", bytes[k]);
//        }
//        Serial.println("");
    });

    server.onNotFound(notFound);

	server.begin();
	Serial.println("HTTP server started!");

    // PS1060/30 SPI Initialize.
    pinMode(SS, OUTPUT);
    digitalWrite(SS, HIGH);
    SPI.begin();
    SPI.beginTransaction(SPISettings(10*1000000, MSBFIRST, SPI_MODE0));

    // mSwitch initialize.
    {
        uint8_t cmd[] = {0x0, 0x0, 0x0, 0x0, 0x0};
        digitalWrite(SS, LOW);
        SPI.writePattern(cmd, 5, 100);	// 5*8*100 = 4000 clock.
        digitalWrite(SS, HIGH);
    }
    device_detect();

#if _I2C_LCD
//    lcd.begin(4, 5);        // Initialize I2C LCD module(SDA = GPIO4, SCL = GPIO5)
    lcd.backlight();        // Turn backlight ON
    lcd.setCursor(0, 0);    // Go to column 0, row 0
    lcd.print(String("[ ") + String(deviceName[gDevice]) + String(" Test ]"));
    lcd.setCursor(0, 1);    // Go to column 0, row 0
    lcd.print(WiFi.localIP());
#endif

#if _I2C_KEYPAD
//    Wire.begin();
//    kpd.begin( makeKeymap(keys) );
#endif
//	yield();
//    Serial.read();
//    Serial.printf("%ld free heap\n", ESP.getFreeHeap());
    state = 0;
    nBytes = 0;
}

int delta = 1;
bool OnOff = true;
uint8_t switchs[15];
void checkKey()
{
    char ckey = kpd.getKey();
    if( ckey ) {
        Serial.printf("key = %d\n", ckey);
        switch ( state ) {
            case 0:                 // main ckey read state.
                if( ckey == MODE_KEY ) {
                    state = 10;    // change mode select.
                    lcd_LineClear(1);
                    lcd.print("Mode Select ?");
                } else if( ckey == '1' ) {
                    // single switch on/off
                    nSW = 0;
//                    ctrl = 1;
                    delta = 1;
                    OnOff = true;
                    nLoop = 1;
                } else if( ckey == '2' ) {
                    ctrl = 1;
                    nLoop = 2;
                } else if( ckey == '3' ) {
                    ctrl = 1;
                    nLoop = 3;
                    nRow = 0;
                } else if( ckey == '4' ) {
                    ctrl = 1;
                    nLoop = 4;
                    nColumn = 0;
                } else if( ckey == '5' ) {
                    nLoop = 5;
                    OnOff = true;
                    for(int i=0; i<15; i++) switchs[i] = 0;
                } else if( ckey == SET_KEY ) {
                    nLoop = 0;
                    SINGLE_CTRL(0, FORCE_OFF);
                } else {
                }
                break;
            case 10:
                if( ckey == '1' ) {          // hex command mode.
                    state = 20;
                    HexDigit = 0;
                    HexValue = 0;
                    nBytes = 0;
                    lcd_LineClear(1);
                    lcd.print("Hex Value ? ");
                } else if( ckey == '2' ) {   // simple switch mode.
                    state = 30;
                    Value = 0;
                    { uint8_t k=0; for(k=0; k<32; k++) Bytes[k] = 0; }
                    lcd_LineClear(1);
                    lcd.print("Switch OP ? ");
                } else {
                }
                break;
            case 20:
                if( HexDigit == 2 ) {
                    if( ckey == ENTER_KEY ) {           // 다음 Hex를 입력 받는다.
                        lcd.write(' ');
                        Bytes[nBytes++] = HexValue;
                        HexDigit = 0;
                        HexValue = 0;
                    } else if( ckey == SET_KEY ) {      // hex command를 전송한다.
                        Bytes[nBytes++] = HexValue;
                        transmit(Bytes, nBytes);
                        state = 10;
//                        HexDigit = 0;
//                        HexValue = 0;
//                        nBytes = 0;
//                        lcd_LineClear(1);
//                        lcd.print("Next Hex ? ");
                    }
                } else {
                    if( (nBytes == 0) && (HexDigit == 0) ) {
                        lcd_LineClear(1);
                    }
                    HexValue = (HexValue * 16) + CharToDigit(ckey);
                    lcd.print(ckey);
                    HexDigit++;
                }
                break;
            case 30:
                if( ckey < '8' ) {          // opcode.
                    Bytes[nBytes++] = 0xAA;
                    Bytes[nBytes++] = 0x38 | CharToDigit(ckey);
                    lcd.print(ckey);
                } else {
                    lcd_LineClear(1);
                    if( (nBytes == 2) && (ckey == ENTER_KEY) ) {
//                        lcd.write(' ');
                        state = 31;
                    } else {
                        lcd.print("unknown opcode");
                        Serial.println("unknown opcode.");
                        state = 10;
                    }
                }
                break;
            case 31:
                if( ckey < 'A' ) {
                    Value = (Value * 10) + CharToDigit(ckey);
                    lcd.print(ckey);
                } else if( (ckey == ENTER_KEY) || (ckey == SET_KEY) ) {
                    set_bit(Value, &Bytes[2]);
                    Value = 0;
                    lcd.print(' ');
                    if( ckey == SET_KEY ) {
                        transmit(Bytes, 11);        // 1030 : 7, 1060 : 11
                        nBytes = 0;
                        state = 10;
                    }
                } else {
                    Serial.println("NC");
                }
                break;
        }
    }
}

void longRun(int nLoop)
{
    if( nLoop == 1 ) {
        if( (millis() - startMillis) > period ) {
            SINGLE_SWITCH_CTRL(0, (OnOff == true) ? SWITCH_ON : SWITCH_OFF, nSW);
            OnOff = !OnOff;
            if( OnOff == true ) {
                if( nSW == (gTotalSwitch - 1) ) {
                    delta = -1;
                } else if( nSW == 0 ) {
                    delta = 1;
                }
                nSW += delta;
            }
            /*
            if( ctrl == 1 ) {
                ctrl = 0;
            } else {
                ctrl = 1;
                nSW++;
                if( nSW == gTotalSwitch ) nSW = 0;
            }
            */
            startMillis = millis();
        }
    } else if( nLoop == 2 ) {
        if( (millis() - startMillis) > (period * 10) ) {
            SINGLE_CTRL(0, ctrl);
            ctrl = (ctrl == 0) ? 1 : 0;
            startMillis = millis();
        }
    } else if( nLoop == 3 ) {
        if( (millis() - startMillis) > period ) {
            byte buf[] = {0, 0};

            set_bit(nRow, buf);

            ROW_ALL_SWITCH_CTRL(0, ctrl, buf);

            if( ctrl == 0 ) {
                nRow++;
                if( nRow > 10 ) nRow = 0;
            }
            ctrl = (ctrl == 1) ? 0 : 1;
            startMillis = millis();
        }
    } else if( nLoop == 4 ) {
        if( (millis() - startMillis) > period ) {
            byte buf[] = {0, 0};

            set_bit(nColumn, buf);

            COLUMN_ALL_SWITCH_CTRL(0, ctrl, buf);

            if( ctrl == 0 ) {
                nColumn++;
                if( nColumn > 6 ) nColumn = 0;
            }
            ctrl = (ctrl == 1) ? 0 : 1;
            startMillis = millis();
        }
    } else if( nLoop == 5 ) {
        if( (millis() - startMillis) > period ) {
            uint8_t sw = random(0, gTotalSwitch);
            int i;

            set_bit(sw, switchs);
            ALL_SWITCH_CTRL(0, OnOff, switchs);

            for(i=0; i<gMaxBytes; i++) {
                if( i == (gMaxBytes-1) ) {
                    if( switchs[i] != 0xF0 ) break;
                } else {
                    if( switchs[i] != 0xFF ) break;
                }
            }
//            Serial.printf("OnOff = %d\n", i);
            if( i == gMaxBytes ) {
                OnOff = !OnOff;
                for(i=0; i<15; i++) switchs[i] = 0;
            }

            startMillis = millis();
        }
    }
}

/*
 */
void loop()
{
//    Serial.print(millis());Serial.print(" : ");Serial.println(__func__);
    dnsServer.processNextRequest();

#if _I2C_LCD
#if _I2C_KEYPAD
    checkKey();
#endif
#endif
    /*
	if( Serial.available() ) {
        char ch = Serial.read();
        // 한 라인을 추과하면 더이상 출력되지 않는다.
        if( ch == '0' ) {
            lcd.clear();            // 기존 문자를 모두 지우고, cursor의 위치를 0,0 으로 이동한다.
            Serial.println("clear");
        } else if( ch == '1' ) {
            lcd.home();             // cursor의 위치만 0,0 으로 이동한다. 기존 글자는 그대로 있다.
            Serial.println("home");
        } else if( ch == '2' ) {
            lcd.autoscroll();       // 한 라인에서 cursor의 위치에 새로운 글자가 입력되고, 이전 글자는 왼쪽으로 이동하여 출력된다.
            Serial.println("autoscroll");
        } else if( ch == '3' ) {
            lcd.noAutoscroll();
            Serial.println("noAutoscroll");
        } else if( ch == '4' ) {
            lcd.scrollDisplayLeft();    // 2라인이 모두 한문자씩 왼쪽으로 이동한다.
            Serial.println("scrollDisplayLeft");
        } else if( ch == '5' ) {
            lcd.rightToLeft();    // 2라인이 모두 한문자씩 왼쪽으로 이동한다.
            Serial.println("rightToLeft");
        } else if( ch == '6' ) {
            lcd_LineClear(0);
            lcd_LineClear(1);
        } else {
            lcd.write(ch);
            Serial.print(ch);
        }
    }
    */

	// read command.
	if( Serial.available() ) {
        char *token;
        Serial.readBytesUntil('\n', buffer, sizeof(buffer));
        token = strtok(buffer, " :,.-");

        if( strncmp(token, "getDevice", 9) == 0 ) {
            Serial.printf("device:%s:group:%d\n", deviceName[gDevice], gGroupType);
//            lcd.setCursor(0, 1);
//            lcd.print(deviceName[gDevice]);
            return;
        } else if( strncmp(token, "ssid", 4) == 0 ) {
            token = strtok(NULL, " :,.-");
            String ss(token);

            token = strtok(NULL, " :,.-");
            if( strncmp(token, "pass", 4) == 0 ) {
                token = strtok(NULL, " :,.-");
                String pa(token);
                stringTo(ss, pa);
//                SPIFFS.begin();       // SPIFFS 파일 시스템을 마운트 해제한다.
                saveConfig();
                SPIFFS.end();       // SPIFFS 파일 시스템을 마운트 해제한다.
                Serial.println("save complete!");
                ESP.restart();
            }
        /*
        } else if( strncmp(token, "getGroup", 8) == 0 ) {
        } else if( strncmp(token, "setGroup", 8) == 0 ) {
            token = strtok(NULL, " :,.-");
            int g = strtol(token, 0, 10);
//            Serial.printf("group:%d\n", g);     // 4, 6, 8, 10, 12, 16
//            lcd.setCursor(0, 1);
//            lcd.print(String("group:")+String(g));
            GROUP_TYPE_SELECT(groupIdx(g));
//            for(int i=0; i<6; i++) {
//                if( c_GroupType[i] == g )
//                    GROUP_TYPE_SELECT(i);
//            }
            for(uint8_t i=0; i<sizeof(buffer); i++) buffer[i] = 0;
        } else if( strncmp(token, "websocket", 9) == 0 ) {
            ws.textAll(String("Server Message"));
        */
        } else {
            nBytes = 0;
            while( token != NULL ) {
                Bytes[nBytes++] = strtol(token, 0, 16);
                token = strtok(NULL, " :,.-");
            }
            transmit(Bytes, nBytes);
            for(uint8_t i=0; i<sizeof(buffer); i++) buffer[i] = 0;
        }
    }

    if( nLoop ) longRun(nLoop);
}

