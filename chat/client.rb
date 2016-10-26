require 'socket'
require 'json'

PORT = 12345
@name = ARGV[0] || "no name"
@socket = TCPSocket.open(ARGV[1] || "localhost", PORT)
@socket.puts(@name)

recv = Thread.new do
  while msg = @socket.gets 
    puts msg.chomp
  end  
end

send = Thread.new do
  while msg = $stdin.gets
    @socket.puts("#{@name} : #{msg}")
  end 
  @socket.close
end

recv.join
send.join