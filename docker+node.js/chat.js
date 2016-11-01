var io = require('socket.io')(8080);
var clients = {};

io.on('connection', function (socket) {
  socket.on('enter', function (name) {
    clients[socket.id] = { socket: socket, name: name };
    console.log(name+"さんが入室しました");
  });

  socket.on('message', function (msg) {
    msg = clients[socket.id]['name'] + " : " + msg
    for(id in clients) {
      if(id != socket.id) {
        clients[id]['socket'].send(msg);
      }
    }
    console.log(msg);
  })

  socket.on('disconnect', function () {
    console.log(clients[socket.id]['name']+"さんが退室しました");
    delete clients[socket.id];
  });
});