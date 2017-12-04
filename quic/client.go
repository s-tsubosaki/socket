package main

import (
		"log"
		"os"
		"bufio"
		"time"
		quic "github.com/lucas-clemente/quic-go"

		"crypto/tls"
)

func main() {
	session, err := quic.DialAddr("0.0.0.0:8080", generateInsecureTLSConfig(), nil)
	if err != nil {
		log.Printf("DialAddr error: %s\n", err)
		return
	}

	stream, err := session.OpenStreamSync()
	if err != nil {
		panic(err)
	}

	recv := func() []byte {
		buf := make([]byte, 32*1024)
		_, err := stream.Read(buf)
		if err != nil {
			panic(err)
		}
		return buf
	}

	send := func(b []byte) {
		_, err := stream.Write(b)
		if err != nil {
			panic(err)
		}
	}

	// 受信
	go func() {
		for {
			buf := recv()
			log.Printf("受信: %s\n", string(buf))
		}
	}()

	// 送信
	stdin := bufio.NewScanner(os.Stdin)
  for {
		if stdin.Scan() {
			message := stdin.Text()
			send([]byte(message))
			log.Printf("送信: %s\n", message)
			time.Sleep(100)
		}
	}
	
	
}

func generateInsecureTLSConfig() *tls.Config {
	return &tls.Config{InsecureSkipVerify: true}
}