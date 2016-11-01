var fs = require("fs");
var max = JSON.parse(fs.readFileSync("./settings.json").toString())["max_conn"];

var io = require('socket.io-client');

for (var i = 0; i < max; i++) {
  var socket = io.connect('http://localhost:8080', { forceNew: true, transports: ["websocket"] });
  socket.on('connect',function(){
  });
}