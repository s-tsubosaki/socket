require 'socket'

PORT = 12345
server = TCPServer.open(PORT)

while true
  Thread.start(server.accept) do |socket|
    puts "#{socket.addr} is accepted"
    while msg = socket.gets
      puts "server received : #{msg}"
      # echo message
      socket.puts(msg)
    end
    puts "#{socket.addr} is gone..."
    socket.close
  end
end