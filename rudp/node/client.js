var rudp = require('rudp');
var dgram = require('dgram');

var socket = dgram.createSocket('udp4');

var client = new rudp.Client(socket, '127.0.0.1', 5000);

client.send(new Buffer("Hello"));
/* パケ詰まりする
var i = 0;
while(true){
  i++;
  client.send(new Buffer(i.toString()));
}
*/