var phoenix = require('phoenix');
var wb = require('websocket');
socket = new phoenix.Socket("http://127.0.0.1:8080/socket", {transport: wb.w3cwebsocket});
socket.connect();
socket.channel("rooms:lobby").join()
  .receive( "error", () => console.log("Failed to connect") )
  .receive( "ok",    () => console.log("Connected") );