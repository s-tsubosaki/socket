var phoenix = require('phoenix');
var wb = require('websocket');
socket = new phoenix.Socket("http://127.0.0.1:8080/socket", {transport: wb.w3cwebsocket});
socket.connect({user_id: "test"});