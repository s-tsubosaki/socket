var rudp = require('rudp');
var dgram = require('dgram');

var socket = dgram.createSocket('udp4');

socket.bind(5000);

console.log('UDP socket bound to port 5000');

var server = new rudp.Server(socket);

server.on('connection', function (connection) {
  connection.on('data', function (data) {
    console.log(data.toString('utf8').trim());
  });
});