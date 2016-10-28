var io = require('socket.io-client');
var cc = 0;
var max = 10000;

for (var i = 0; i < max; i++) {
  var socket = io.connect('http://localhost:8080', { 'force new connection' : true, transports: ["websocket"] });
  socket.on('connect',function(){
    socket.emit('count', max);
  });
}