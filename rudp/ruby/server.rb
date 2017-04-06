require './rudp.rb'

server = RUDP::Server.new(5000)
server.run!

loop do
  if input = IO.select([$stdin])[0][0]
    server.broadcast(input.gets)
  end
  sleep(0)
end