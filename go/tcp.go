package main

import (
	"fmt"
	"net"
)

func main() {
	tcpAddr, _ := net.ResolveTCPAddr("tcp", ":0")
	listener, err := net.ListenTCP("tcp", tcpAddr)
	if err != nil {
		fmt.Printf("Listen error: %s\n", err)
		return
	}
	defer listener.Close()

	addr := listener.Addr()
	fmt.Printf("Auto binded Port:%d", addr.(*net.TCPAddr).Port)

	conn, err := listener.AcceptTCP()
	if err != nil {
		fmt.Printf("Accept error: %s\n", err)
		return
	}
	defer conn.Close()

	buf := make([]byte, 1024)
	for {
		n, err := conn.Read(buf)
		if err != nil {
			fmt.Printf("Read error: %s\n", err)
			_, ok := err.(net.Error)
			fmt.Println(ok)
			break
		}
		fmt.Print(string(buf[:n]))
	}
}
