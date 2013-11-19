// Creates a websocket with socket.io
// Make sure to install socket.io: terminal, goto /var/lib/cloud9 and enter: npm install socket.io
// Installing this takes a few minutes; wait until the installation is complete

var app = require('http').createServer(handler);
var io = require('socket.io').listen(app);
var fs = require('fs');
var b = require('bonescript');

app.listen(7000);
// socket.io options go here
io.set('log level', 2);   // reduce logging - set 1 for warn, 2 for info, 3 for debug
io.set('browser client minification', true);  // send minified client
io.set('browser client etag', true);  // apply etag caching logic based on version number

console.log('DeathBot Server running on: http://' + getIPAddress() + ':7000');

var SerialPort = require("serialport").SerialPort
var serialPort = new SerialPort("/dev/ttyO1", {
  baudrate: 9600
});
var throttle = 0;
var LeftRight = 0;
var Reverse = false;
var SuperSpeed = false;
var Claw = false;
var Light = false;
var RobotPower = false;
var CameraPower = false;

function handler (req, res) {
  if (req.url == "/favicon.ico"){   // handle requests for favico.ico
  res.writeHead(200, {'Content-Type': 'image/x-icon'} );
  res.end();
  console.log('favicon requested');
  return;
  }
  fs.readFile('index.html',    // load html file
  function (err, data) {
    if (err) {
      res.writeHead(500);
      return res.end('Error loading index.html');
    }
    res.writeHead(200);
    res.end(data);
  });
}

io.sockets.on('connection', function (socket) {
  // listen to sockets and send values to arduino
  // Throttle data
    socket.on('throttle', function (data) {
    serialPort.write("T " + data + '\n');
    console.log("T " + data);
  });
  // Left and Right data
    socket.on('LeftRight', function (data) {
    serialPort.write("L " + data + '\n');
    console.log("L " + data);
  });
  // Reverse mode
    socket.on('Reverse', function (data) {
    console.log("R " + data);
    // switch mode
    if (data == 'on') {
       Reverse = true;
       serialPort.write("R " + '1' + '\n');
    } else if (data == 'off') {
       Reverse = false;
       serialPort.write("R " + '0' + '\n');
    }
  });
  // Super Speed mode
    socket.on('SuperSpeed', function (data) {
    console.log("S " + data);
    // switch mode
    if (data == 'on') {
       SuperSpeed = true;
       serialPort.write("S " + '1' + '\n');
    } else if (data == 'off') {
       SuperSpeed = false;
       serialPort.write("S " + '0' + '\n');
    }
  });
  // Claw mode
    socket.on('Claw', function (data) {
    console.log("C " + data);
    // switch mode
    if (data == 'on') {
       Claw = true;
       serialPort.write("C " + '1' + '\n');
    } else if (data == 'off') {
       Claw = false;
       serialPort.write("C " + '0' + '\n');
    }
  });
  // Light mode
    socket.on('Light', function (data) {
    console.log("G " + data);
    // switch mode
    if (data == 'on') {
       Light = true;
       serialPort.write("G " + '1' + '\n');
    } else if (data == 'off') {
       Light = false;
       serialPort.write("G " + '0' + '\n');
    }
  });
  // Robot Power mode
    socket.on('RobotPower', function (data) {
    console.log("P " + data);
    // switch mode
    if (data == 'on') {
       RobotPower = true;
       serialPort.write("P " + '1' + '\n');
    } else if (data == 'off') {
       RobotPower = false;
       serialPort.write("P " + '0' + '\n');
    }
  });
  // Camera Power mode
    socket.on('CameraPower', function (data) {
    console.log("I " + data);
    // switch mode
    if (data == 'on') {
       CameraPower = true;
       serialPort.write("I " + '1' + '\n');
    } else if (data == 'off') {
       CameraPower = false;
       serialPort.write("I " + '0' + '\n');
    }
  });
});

// Get server IP address on LAN
function getIPAddress() {
  var interfaces = require('os').networkInterfaces();
  for (var devName in interfaces) {
    var iface = interfaces[devName];
    for (var i = 0; i < iface.length; i++) {
      var alias = iface[i];
      if (alias.family === 'IPv4' && alias.address !== '127.0.0.1' && !alias.internal)
        return alias.address;
    }
  }
  return '0.0.0.0';
}
 
