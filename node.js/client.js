var client = require('socket.io-client');
var socket = client.connect('http://localhost:8080', { transports: ["websocket"]});
socket.on('connect',function(){
    console.log('connected!');
    socket.emit('enter', 'bob')
});