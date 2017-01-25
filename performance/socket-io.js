var fs = require("fs");
var max = JSON.parse(fs.readFileSync("./settings.json").toString())["max_conn"];

var io = require('socket.io')(8080);
io.transports = ['websocket'];

console.time('performance');
io.on('connection', function (socket) {
  socket.on('disconnect', function () {
  });
  socket.on('run', function (data) {
  });
});
