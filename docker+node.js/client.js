var name = process.argv[2] || 'NoName';

var client = require('socket.io-client');
var socket = client.connect('http://localhost:8080', { transports: ["websocket"]});
socket.on('connect',function(){
    console.log('connected!');
    socket.emit('enter', name);
});
socket.on('message', function(msg) {
  console.log(msg);
})

var reader = require('readline').createInterface({
  input: process.stdin,
  output: process.stdout
});

reader.on('line', function(line) {
  socket.send(line);
});
reader.on('close', function() {
  socket.disconnect();
});