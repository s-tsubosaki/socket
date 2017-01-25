var fs = require("fs");
var max = JSON.parse(fs.readFileSync("../performance/settings.json").toString())["max_conn"];

var phoenix = require('phoenix');
var wb = require('websocket');
var c = 0;

console.time('connections');
for (var i = 0; i < max; i++) {
  var socket = new phoenix.Socket("http://127.0.0.1:8080/socket", {transport: wb.w3cwebsocket});
  socket.connect();
  socket.onOpen( () => {
    if(++c >= max){
      console.timeEnd('connections');
    }
  } );
}