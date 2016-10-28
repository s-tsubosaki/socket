package main

import (
    "log"
    "net/http"
    "github.com/googollee/go-socket.io"
    "time"
    "os"
)

func main() {
    start := time.Now()
    end   := time.Now()
    server, err := socketio.NewServer([]string{"websocket"})
    if err != nil {
        log.Fatal(err)
    }
    server.On("connection", func(so socketio.Socket) {
        //log.Println("on connection")
        so.On("disconnection", func() {
            //log.Println("on disconnect")
        })
        so.On("run", func(msg string) {
        })
        so.On("start", func(msg string) {
            start = time.Now()
        })
        so.On("end", func(msg string) {
            end = time.Now()
            log.Println("performance: ", end.Sub(start))
        })

        if(server.Count()==1){
            start = time.Now()
        }
        if(server.Count()>=1000){
            end = time.Now()
            log.Println("clients: ", end.Sub(start))
            os.Exit(0)
        }
    })

    http.Handle("/", server)
    log.Fatal(http.ListenAndServe(":8080", nil))
}