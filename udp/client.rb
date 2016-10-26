require 'socket'

ADDR = ARGV.shift
PORT = 12345
@udp = UDPSocket.new

# イベント駆動で書き込みしてみる
loop do
  if input = IO.select([$stdin])[0][0]
    @udp.send(input.gets, 0, ADDR, PORT)
  end
end