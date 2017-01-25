package main

import (
    "log"
    "net/http"
    "github.com/googollee/go-socket.io"
    "encoding/json"
    "io/ioutil"
    "fmt"
)

type Setting struct {
    MaxSend int `json:"max_send"`
    MaxConn int `json:"max_conn"`
}

func main() {    
    var settings Setting
    raw, err := ioutil.ReadFile("./settings.json")
    json.Unmarshal(raw, &settings)
    fmt.Println(settings)

    server, err := socketio.NewServer([]string{"websocket"})
    if err != nil {
        log.Fatal(err)
    }
    log.SetOutput(ioutil.Discard)

    server.SetMaxConnection(settings.MaxConn)
    server.On("connection", func(so socketio.Socket) {
        so.On("disconnection", func() {
        })
        so.On("run", func(msg string) {
        })
    })

    http.Handle("/", server)
    log.Fatal(http.ListenAndServe(":8080", nil))
}