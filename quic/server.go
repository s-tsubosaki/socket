package main

import (
		"log"
		"sync"
		"time"
		quic "github.com/lucas-clemente/quic-go"

		// fot tls1.3
		"crypto/rand"
		"crypto/rsa"
		"crypto/tls"
		"crypto/x509"
		"encoding/pem"
		"math/big"
)

var mtx sync.RWMutex

type Message struct {
	stream *quic.Stream
	session *quic.Session
	data []byte
}

func main() {
	listener, err := quic.ListenAddr("0.0.0.0:8080", generateTLSConfig(), nil)
	if err != nil {
		log.Printf("Listen error: %s\n", err)
		return
	}
	defer listener.Close()

	streams := []quic.Stream{}
	mtx := sync.RWMutex{}
	message := make(chan Message)

	go func() {
		for {
			sess, err := listener.Accept()
			if err != nil {
				return
			}

			stream, err := sess.AcceptStream()
			if err != nil {
				panic(err)
			}

			mtx.Lock()
			streams = append(streams, stream)
			mtx.Unlock()

			log.Printf("Accept: %s\n", sess.RemoteAddr())

			go func() {
				for {
					buf := make([]byte, 1024)
					_, err = stream.Read(buf)
					if err != nil {
						panic(err)
					}
					message <- Message{ stream: &stream, session: &sess, data: buf }
				}
			}()
		}
	}()

	for {
		msg := <- message 
		log.Printf("Message: %s from: %s\n", string(msg.data), (*(msg.session)).RemoteAddr())
		time.Sleep(50)
	}
}

// Setup a bare-bones TLS config for the server
func generateTLSConfig() *tls.Config {
	key, err := rsa.GenerateKey(rand.Reader, 1024)
	if err != nil {
		panic(err)
	}
	template := x509.Certificate{SerialNumber: big.NewInt(1)}
	certDER, err := x509.CreateCertificate(rand.Reader, &template, &template, &key.PublicKey, key)
	if err != nil {
		panic(err)
	}
	keyPEM := pem.EncodeToMemory(&pem.Block{Type: "RSA PRIVATE KEY", Bytes: x509.MarshalPKCS1PrivateKey(key)})
	certPEM := pem.EncodeToMemory(&pem.Block{Type: "CERTIFICATE", Bytes: certDER})

	tlsCert, err := tls.X509KeyPair(certPEM, keyPEM)
	if err != nil {
		panic(err)
	}
	return &tls.Config{Certificates: []tls.Certificate{tlsCert}}
}
