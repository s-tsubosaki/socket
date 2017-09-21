package main

import (
    "log"
    "net"
    "os"
)

func main() {
    udpAddr := &net.UDPAddr{
        IP:   net.ParseIP("127.0.0.1"),
        Port: 0,
    }
		udpLn, err := net.ListenUDP("udp", udpAddr)

    if err != nil {
        log.Fatalln(err)
        os.Exit(1)
		}
		
		laddr := udpLn.LocalAddr()
		log.Printf("Auto binded Port:%d",laddr.(*net.UDPAddr).Port)

    buf := make([]byte, 1024)

    for {
        n, addr, err := udpLn.ReadFromUDP(buf)
        if err != nil {
            log.Fatalln(err)
            os.Exit(1)
        }

        go func() {
            log.Printf("Reciving data: %s from %s", string(buf[:n]), addr.String())

            log.Printf("Sending data..")
            udpLn.WriteTo([]byte("Pong"), addr)
            log.Printf("Complete Sending data..")
        }()
    }
}