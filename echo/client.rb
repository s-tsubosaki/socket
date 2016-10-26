require 'socket'

PORT = 12345
socket = TCPSocket.open(ARGV.first || "localhost", PORT)
msg = ''
while msg != 'kill'
  msg = gets.chomp
  socket.puts(msg)
  puts "client received : #{socket.gets}"
end 
socket.close