var fs = require("fs");
var max = JSON.parse(fs.readFileSync("./settings.json").toString())["max_send"];

var client = require('socket.io-client');
var socket = client.connect('http://localhost:8080', { transports: ["websocket"] });
socket.on('connect',function(){
    socket.emit('start');
    for (var i = 0; i < max; i++) {
      socket.emit('run', i.toString());
    }
    socket.emit('end');
});