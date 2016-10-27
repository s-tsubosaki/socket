var io = require('socket.io')(8080);
var clients = {};

io.on('connection', function (socket) {
  console.log("connected : " + socket.id)

  socket.on('enter', function (data) {
    clients[socket.id] = data
    console.log('enter : ' + data);
  });

  socket.on('disconnect', function () {
    console.log('leave : ' + clients[socket.id]);
  });
});