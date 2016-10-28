package main

import (
    "log"
    "net/http"
    "github.com/googollee/go-socket.io"
)

func main() {
    server, err := socketio.NewServer([]string{"websocket", "polling"})
    if err != nil {
        log.Fatal(err)
    }
    server.On("connection", func(so socketio.Socket) {
        log.Println("on connection")
        so.On("disconnection", func() {
            log.Println("on disconnect")
        })
    })
    server.On("error", func(so socketio.Socket, err error) {
        log.Println("error:", err)
    })

    http.Handle("/", server)
    http.ListenAndServe(":8080", nil)
}