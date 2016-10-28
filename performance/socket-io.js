var io = require('socket.io')(8080);
io.transports = ['websocket'];

io.on('connection', function (socket) {
  //console.log("on connection");

  socket.on('disconnect', function () {
    //console.log('on disconnect');
  });
  socket.on('run', function (data) {
  });
  socket.on('start', function (data) {
    console.time('performance');
  });
  socket.on('end', function (data) {
    console.timeEnd('performance');
  });
});