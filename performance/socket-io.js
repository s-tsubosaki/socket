var fs = require("fs");
var max = JSON.parse(fs.readFileSync("./settings.json").toString())["max_conn"];

var io = require('socket.io')(8080);
io.transports = ['websocket'];

console.time('performance');
io.on('connection', function (socket) {
  //console.log("on connection");

  socket.on('disconnect', function () {
    //console.log('on disconnect');
  });
  socket.on('run', function (data) {
  });
  socket.on('end', function (data) {
    console.timeEnd('performance');
  });

  if(socket.client.conn.server.clientsCount==1){
    console.time('clients');
  }else if(socket.client.conn.server.clientsCount>=max){
    console.timeEnd('clients');
    //process.exit(0);
  }
});