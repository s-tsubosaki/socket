var io = require('socket.io-client');
var cc = 0;
var max = 1000;

for (var i = 0; i < max; i++) {
  var socket = io.connect('http://localhost:8080', { 'force new connection' : true, transports: ["websocket"] });
  socket.on('connect',function(){
      if(cc++==0){
        socket.emit('start');
      }else if(cc==max){
        socket.emit('end');
      }
  });
}