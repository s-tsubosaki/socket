require 'socket'
require 'json'

PORT = 12345
server = TCPServer.open(PORT)
@clients = {}

def send_to_client(msg, socket)
  puts "#{socket.addr.last} : #{msg}"
  @clients.each do |object_id, c|
    c[1].puts(msg) unless object_id == socket.object_id
  end
end 

while true
  Thread.start(server.accept) do |socket|
    name = socket.gets.chomp
    begin
      send_to_client("#{name}が入室しました", socket)
      @clients[socket.object_id] = [name, socket]
      while msg = socket.gets
        send_to_client(msg, socket)
      end
    rescue => e
      puts e 
    end
    send_to_client("#{name}が退室しました", socket)
    @clients.delete(socket.object_id)
    socket.close
  end
end