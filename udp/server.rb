require 'socket'

PORT = 12345
@udp = UDPSocket.open
@udp.bind("0.0.0.0", PORT)

loop do
  puts @udp.recv(65536)
end