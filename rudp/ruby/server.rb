require './rudp.rb'

server = RUDP::Server.new(5000)
server.run!

loop do
  sleep(0)
end