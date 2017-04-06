require './rudp.rb'

client = RUDP::Client.new("127.0.0.1", 5000)
client.run!

loop do
  client.send("aaa")
  sleep(0)
end