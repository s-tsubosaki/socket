require 'socket'

ADDR = ARGV.shift
SIZE = ARGV.shift
PORT = 12345
@udp = UDPSocket.new
@udp.send('A' * SIZE.to_i, 0, ADDR, PORT)