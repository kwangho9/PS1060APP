//const path = require('path');
//const fs = require('fs');

const path = require('path');
const fs = require('fs');
const express = require('express')
const app = express()
const open = require('open');
const serialPort = require('serialport');
const Readline = require('@serialport/parser-readline');
const WebSocket = require('ws');


//const port = 9988;
var myPort = null;
var result = "";
var comport = "";
var deviceIdx = 3;
var gGroup;
const deviceName = ["PS1030", "PS1060", "PS1120", "NoDevice"];
const cGroupType = [4, 6, 8, 10, 12, 16];
const groupIdx = [0, 0, 0, 0, 0, 0, 1, 0, 2, 0, 3, 0, 4, 0, 0, 0, 5];


app.set('port', process.env.PORT || 9988);

/* 모든 request(요청)을 받아서 처리하지는 않고, next()로 다음 미들웨어에 넘긴다. */
app.use((req, res, next) => {
    res.cookie('device', deviceIdx.toString(), {
        httpOnly: false,
        signed: false,
        maxAge: 10000
    });
    res.cookie('group', gGroup.toString(), {
        httpOnly: false,
        signed: false,
        maxAge: 10000
    });
    next();
});

/* root directory('/')를 server의 'static' directory로 연결한다.
 * main 페이지('/')가 "index.html"이면, 자동으로 파일을 찾아서 전송한다.
 * client의 요청(request) URL에 기술된 파일을 검색해서 전송한다.
 * 요청(..../img/a.png)라고 하면, 'static' 폴더내에 'img'폴더와 'a.png' 파일이 있어야 한다.
 */
app.use('/', express.static(path.join(__dirname, 'static')));
//app.use('/', express.static(__dirname + '/static'));

function processkill(){
    if( deviceIdx == 3 ) {
        console.log('exit()');
        server.close();
        process.exit();
        Port.close();
    } else {
        open(`http://localhost:${app.get('port')}/`);
    }
}

let processDone = async ()=>{
//  await setInterval(processkill,1000)
  await setTimeout(processkill,500)
}

// COM 포트를 검색하고, 각각의 포트에 대해서 Device 정보를 검색하도록 한다.
// Device 정보를 먼저 가져온다.
// 만약, Device정보가 없다면, Server를 실행하지 않는다.


function detectDevice(com) {
    const Port = new serialPort(com, {baudRate: 57600}, false);
//    const parser = myPort.pipe(new Readline({delimiter: '\n'}));
    const parser = Port.pipe(new Readline({delimiter: '\n'}));
    Port.open( function() {
        console.log(`${com} open.`);
        Port.write("getDevice\n", function(err, results) { });
    });
    Port.on('error', function(err) {
//        console.log('Serial port error : ' + err);
    });
    Port.on('close', function(err) {
        console.log('Serial port close : ' + err);
        myPort = null;
    });

    parser.on('data', (data) => {
        console.log(`${com} rx : ${data}`);

        var header = data.split(':');
//        var header = data.split(':').split(',');
        if( (header[0] != undefined) && (header[0] == 'device') ) {
//            console.log(`Header : ${header[0]}, ${header[1]}, ${header[2]}, ${header[3]}`);
            if( (header[1] == 'PS1030') ) {
                deviceIdx = 0;
                gTotalSwitch = 30;
                CurrentDevice = CurrentDevice & ~0x80;
            } else if( (header[1] == 'PS1060') ) {
                deviceIdx = 1;
                gTotalSwitch = 60;
                CurrentDevice = CurrentDevice | 0x80;
            }
            if( header[2] == 'group' ) {
                gGroup = header[3];
                gGroupType = parseInt(gGroup);
                gRow = gGroupType;
//                console.log(`Type : ${typeof(gGroup)}, ${typeof(gGroupType)}, $typeof(gRow)}`);
                gColumn = Math.ceil(gTotalSwitch / gGroupType);
                CurrentDevice = (CurrentDevice & 0xF8) | groupIdx[gGroupType];
            }
            comport = com;
            myPort = Port;
        } else if( header[0] == 'switch' ) {
        } else if( header[0] == 'info' ) {
            console.log(header[1]);
        } else {
            console.log(`${data} received`);
//            comport = '';
//            deviceIdx = 3;              // NoDevice
        }
    });
}

serialPort.list().then(async ports => {
    var list = '';
    ports.sort(function(a, b) {         // COM 포트를 오름차순으로 정렬한다.
        return a.path < b.path ? -1 : a.path > b.path ? 1 : 0;
    });
    ports.forEach(async function(port) {
        list += port.path + ' ';
//        console.log(`${port.path} connected.`);
        detectDevice(port.path);
    });
    console.log('port list : ' + list);
});

processDone()
//process.exit(1);              // Node.js를 강제 종료시킨다.

// http request 처리함수를 작성한다.
/*
app.get('/', function(req, res) {
    console.log(__dirname);
    res.sendFile(path.join(__dirname, '/static/PS1xxx UI.html'));
//    if( myPort != null ) {
//        console.log(myPort);
//        console.log(myPort.path);
//        console.log(myPort.settings.baudRate);
//    }
//    processDone()
})
*/

/*
app.get('/get_device', function(req, res) {
    if( myPort != null ) {
        var str = "0x55 0xAA\n";
        console.log("get_device");
        detectDevice(myPort.path);
        deviceIdx = 0;
        res.send(deviceIdx.toString());
//        myPort.write(str, function(err, results) {});
//        result = '';
//        console.log("get_device : " + result);
    }
});
*/

var gTotalSwitch;
var gRow;
var gColumn;
var gGroupType;
var gChipAddr;

var CurrentDevice;    // Device(1-bit) + Address(3-bt) + AD(1-bit) + Group(3-bit)
var CurrentSwitch = new Uint8Array(60);
const MSBMask = [0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01];
function set_bit(pos, bytes)
{
    bytes[pos >> 3] |= MSBMask[pos & 0x7];
}

function clear_bit(pos, bytes)
{
    bytes[pos >> 3] &= ~MSBMask[pos & 0x7];
}

function check_bit(pos, bytes)
{
    return (bytes[pos >> 3] & MSBMask[pos & 0x7]);
}

function set_switch(opcode, sw)
{
    if( sw >= gTotalSwitch ) return -1;

    if( opcode == 0 ) {             // switch off
        if( CurrentSwitch[sw] & (1<<1) ) return 1;      // when switch hold.
        CurrentSwitch[sw] = opcode;
    } else if( opcode == 1 ) {      // switch on
        if( CurrentSwitch[sw] & (1<<1) ) return 1;      // when switch hold.
        CurrentSwitch[sw] = opcode;
    } else if( opcode == 2 ) {      // hold off
        if( CurrentSwitch[sw] & (1<<1) ) return 2;      // when switch hold.
        CurrentSwitch[sw] = opcode;
    } else if( opcode == 3 ) {      // hold on
        if( CurrentSwitch[sw] & (1<<1) ) return 2;      // when switch hold.
        CurrentSwitch[sw] = opcode;
    } else if( opcode == 4 ) {      // release off
        if( (CurrentSwitch[sw] & (1<<1) == 0) ) return 3;
        CurrentSwitch[sw] = 0x0;
    } else if( opcode == 5 ) {      // release on
        if( (CurrentSwitch[sw] & (1<<1) == 0) ) return 3;
        CurrentSwitch[sw] = 0x1;
    } else if( opcode == 6 ) {      // force off
        CurrentSwitch[sw] = 0x0;
    } else if( opcode == 7 ) {      // force on
        CurrentSwitch[sw] = 0x1;
    }

    return 0;
}

function toHexString(bytes) {
    return Array.from(bytes, function(byte) {
        return '0x' + ('0' + (byte & 0xFF).toString(16)).slice(-2);
    });
}

function command_parser(str)
{
    var bytes = str.split(' ').map(Number);
//    console.log(bytes);
    console.log(toHexString(bytes));
    console.log(`gTotalSwitch = ${gTotalSwitch}, gRow = ${gRow}, gColumn = ${gColumn}`);

    if( (bytes[0] != 0xAA) && (bytes[0] != 0xFF) ) return;

    var SwitchSelect = [];
    var idx = 1;

    var c = bytes[idx++];
    var type = (c & 0x38) >> 3;
    var op = (c & 0x07);

    if( c & (1<<7) ) {      // Special Command.
    } else {
        if( c & (1<<6) ) {
            if( (bytes[idx] & gChipSelect) == 0 ) return;
        }

        switch( type ) {
            case 0:         // All Switch Select Command.
                for(var i=0; i<gTotalSwitch; i++) {
                    set_switch(op, i);
                }
                break;
            case 1:         // Single Switch Select Command.
                set_switch(op, bytes[idx++]);
                break;
            case 2:         // Row All Switch Select Command.
//                console.log(`gTotalSwitch = ${gTotalSwitch}`);
//                process.stdout.write(`${gRow} Row All Switch : `);
                var arr = [];
                arr.push(bytes[idx++]);
                arr.push(bytes[idx++]);
                arr.push(bytes[idx++]);
                arr.push(bytes[idx++]);
                for(var row=0; row<gRow; row++) {
                    if( check_bit(row, arr) ) {
//                        process.stdout.write(`(${row}) `);
                        for(var i=row; i<gTotalSwitch; i+=gRow) {
                            set_switch(op, i);
//                            process.stdout.write(`${i} `);
                        }
                    }
                }
//                console.log("");
                break;
            case 3:         // Row Each Switch Select Command.
//                console.log(`gTotalSwitch = ${gTotalSwitch}, gRow = ${gRow}, gColumn = ${gColumn}`);
//                process.stdout.write(`${gRow} Row Each Switch : `);
                var arr = [];
                arr.push(bytes[idx++]);
                arr.push(bytes[idx++]);
                arr.push(bytes[idx++]);
                arr.push(bytes[idx++]);
                for(var row=0; row<gRow; row++) {
                    if( check_bit(row, arr) ) {
//                        process.stdout.write(`(${row}) `);
                        for(var i=0; i<gColumn; i++) {
                            if( check_bit(gRow+i, arr) ) {
                                var sw = row + (i * gRow);
                                set_switch(op, sw);
//                                process.stdout.write(`${sw} `);
                            }
                        }
                    }
                }
//                console.log("");
                break;
            case 4:         // Column All Switch Select Command.
                var arr = [];
                arr.push(bytes[idx++]);
                arr.push(bytes[idx++]);
                arr.push(bytes[idx++]);
                arr.push(bytes[idx++]);
//                process.stdout.write(`${gRow} Column All Switch : `);
                for(var col=0; col<gColumn; col++) {
                    if( check_bit(col, arr) ) {
//                        process.stdout.write(`(${col}) `);
                        var start = col * gGroupType;
                        for(var sw=0; sw<gGroupType; sw++) {
                            if( sw >= gTotalSwitch ) break;
                            set_switch(op, sw+start);
//                            process.stdout.write(`${sw+start} `);
                        }
                    }
                }
//                console.log("");
                break;
            case 5:         // Column Each Switch Select Command.
                break;
            case 6:         // Special Switch Command.
                if( op == 1 ) {             // Set Group Type.
                    gGroupType = cGroupType[Number(bytes[idx]) >> 5];
                    gRow = gGroupType;
                    gColumn = Math.ceil(gTotalSwitch / gGroupType);
                    CurrentDevice = (CurrentDevice & 0xF8) | groupIdx[gGroupType];
                } else if( op == 2 ) {      // Software Reset.
                } else if( op == 3 ) {      // ...
                } else if( op == 4 ) {      // Active Discharge Disable.
                    CurrentDevice = (CurrentDevice & 0xF7);
                } else if( op == 5 ) {      // Active Discharge Enable.
                    CurrentDevice = (CurrentDevice & 0x08);
                } else if( op == 6 ) {      // ...
                } else if( op == 7 ) {      // Chip Address Assignment.
                    var mask = cmd[idx];
                    for(var i=0; i<8; i++) {
                        if( mask & (1<<(7 - i)) ) {
                            gChipAddr = i;
                            break;
                        }
                    }
                    CurrentDevice = (CurrentDevice & 0x8F) | (gChipAddr << 4);
                } else {
                }
                break;
            case 7:         // Each Switch Select Command.
                var arr = [];
                arr.push(bytes[idx++]);
                arr.push(bytes[idx++]);
                arr.push(bytes[idx++]);
                arr.push(bytes[idx++]);
                for(var i=0; i<gTotalSwitch; i++) {
                    if( check_bit(i, arr) ) {
                        set_switch(op, i);
                    }
                }
                break;
        }
    }

//    console.log(`CurrentSwitch : ${CurrentSwitch}`);
    console.log(CurrentDevice.toString()+','+CurrentSwitch.toString('utf8'));
    wss.broadcast(CurrentDevice.toString()+','+CurrentSwitch.toString('utf8'));
}

app.get('/Command', function(req, res) {
    res.status(204).send("");

    var str = req.query.CMD + '\n';

//    console.log("Received Command : "+str);
    if( myPort != null ) {
        myPort.write(str, function(err, results) {});
        command_parser(str);
    } else {
        console.log("COM Port Not Available.");
    }
});

/*
app.get('/setGroup', function(req, res) {
    res.status(204).send("");

    var str = 'setGroup:' + req.query.GROUP + '\n';

    console.log("Received Command : "+str);
    if( myPort != null ) {
        myPort.write(str, function(err, results) {});
        gGroupType = Number(req.query.GROUP);
        CurrentDevice = (CurrentDevice & 0xF8) | groupIdx[gGroupType];
        wss.broadcast(CurrentDevice.toString()+','+CurrentSwitch.toString('utf8'));
    } else {
        console.log("COM Port Not Available.");
    }
});
*/

// http server 실행.
const server = app.listen(app.get('port'), () => {
    console.log(`Web Server listening at http://localhost:${app.get('port')}`);
});

const wss = new WebSocket.Server({server});

wss.on('connection', (ws, req) => {
//        const ip = req.headers['x-forwarded-for'] || req.connection.remoteAddress;
//        console.log('새로운 클라이언트 접속', ip);
        wss.broadcast(CurrentDevice.toString()+','+CurrentSwitch.toString('utf8'));

        ws.on('message', (message) => {
            console.log(message);
        });
        ws.on('error', (error) => {
            console.error(error);
        });
        ws.on('close', () => {
            console.log('클라이언트 접속 해제');
//            console.log('클라이언트 접속 해제', ip);
//            clearInterval(ws.interval);
        });

//        ws.interval = setInterval(() => {
//            if( ws.readyState === ws.OPEN ) {
//                ws.send('서버에서 클라이언트로 메시지를 보냅니다.');
//            }
//        }, 3000);
});

wss.broadcast = function broadcast(msg) {
    wss.clients.forEach(function each(client) {
        client.send(msg);
    });
};

//open(`http://localhost:${app.get('port')}/`);
