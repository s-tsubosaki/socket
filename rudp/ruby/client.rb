require './rudp.rb'

client = RUDP::Client.new("127.0.0.1", 5000)
client.run!

loop do
  if input = IO.select([$stdin])[0][0]
    client.send(input.gets)
  end
  sleep(0)
end