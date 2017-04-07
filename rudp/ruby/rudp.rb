require 'socket'
require 'json'

module RUDP
  class Socket < UDPSocket
    def send(command, addr, port)
      super(command.to_packet, 0, addr, port)
    end  
    def recv 
      packet, info = self.recvfrom(1024)
      command = RUDP::Command.from_packet(packet)
      [command, info]
    end
  end  

  class Command
    attr_accessor :type, :seq, :data
    attr_accessor :sent_at, :acked
    attr_accessor :reliable

    def initialize(type: nil, seq: nil, data: nil, reliable: false)
      @type = type 
      @seq  = seq
      @data = data
      @sent_at = nil 
      @acked = false
      @reliable = reliable
    end

    def to_packet
      { type: @type, seq: @seq, data: @data, reliable: @reliable }.to_json
    end

    def self.from_packet(packet)
      hash = JSON.parse(packet)
      r = self.new
      hash.each do |key, v|
        r.__send__("#{key}=", v) 
      end
      r
    end
  end

  class Client
    def initialize(addr, port)
      @addr, @port = addr, port
      @rudp = RUDP::Socket.new
      @seq = 0
      @mutex = Mutex.new
      @reliables = []
      @unreliables = []
      @recvs = []
      @recv_seq = 1
    end

    def run!
      send(0, type: :connect)
      recv_thread = Thread.new do
        begin
          loop do
            recv_command, info = @rudp.recv
            if recv_command.reliable && recv_command.seq >= @recv_seq
              @recvs << recv_command
              command = @recvs.find{|r| r.seq == @recv_seq}
              if command 
                puts "reliable:#{command.type}:#{command.data}"
                @recvs.delete_if{|r| r.seq == @recv_seq}
                @recv_seq += 1
              end
            else 
              command = recv_command
              puts "unreliable:#{command.type}:#{command.data}"
            end

            if command
              case command.type.to_sym
              when :ack 
                puts "acked:seq:#{command.data}"
                remove_send_command(command.data)
              end
            end
            sleep(0)
          end
        rescue => e  
          puts e  
        end
      end
      send_thread = Thread.new do
        begin
          loop do
            send_commands
            sleep(0)
          end
        rescue => e  
          puts e  
        end
      end
    end

    def send(data, type: :exe, reliable: true)
      if reliable
        @mutex.synchronize do
          @seq += 1
          @reliables << RUDP::Command.new(type: type, seq: @seq, data: data, reliable: reliable)
        end
      else 
        @unreliables << RUDP::Command.new(type: type, seq: @seq, data: data, reliable: reliable)
      end
    end

    def send_commands
      @mutex.synchronize do
        @reliables.each do |command|
          if command.sent_at.nil?
            @rudp.send(command, @addr, @port)
            command.sent_at = Time.now 
          end
        end

        while command = @unreliables.pop
          @rudp.send(command, @addr, @port)
        end
      end
    end

    def remove_send_command(seq)
      @mutex.synchronize{ @reliables.delete_if{|s| s.seq==seq} }
    end

    def ack(seq)
      send(seq, type: :ack, reliable: false)
    end
  end

  class Server
    def initialize(port)
      @rudp = RUDP::Socket.open
      @port = port 
      @clients = {}
    end

    def run! 
      send_thread = Thread.new do
        begin
          loop do
            @clients.each do |id, client|
              client.send_commands
            end
            sleep(0)
          end  
        rescue => e  
          puts e  
        end
      end  

      recv_thread = Thread.new do
        begin
          @rudp.bind("0.0.0.0", @port)
          loop do
            command, info = @rudp.recv
            connection_id = "#{info.last}:#{info[1]}"
            puts connection_id
            case command.type.to_sym
            when :connect
              puts "new connection #{connection_id}"
              @clients[connection_id] = RUDP::Client.new(info.last, info[1])
            when :ack 
              puts command.to_packet
              @clients[connection_id].remove_send_command(command.seq)
            end

            if command.reliable
              @clients[connection_id].ack(command.seq)
            end
            puts command.to_packet
            sleep(0)
          end
        rescue => e  
          puts e
        end
      end
    end

    def broadcast(data)
      @clients.each do |id, client|
        client.send(data)
      end
    end
  end
end