package main

import (
    "log"
    "net/http"
    "github.com/googollee/go-socket.io"
    "time"
    "os"
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

    start := time.Now()
    end   := time.Now()
    server.SetMaxConnection(settings.MaxConn)
    server.On("connection", func(so socketio.Socket) {
        //log.Println("on connection")
        so.On("disconnection", func() {
            //log.Println("on disconnect")
        })
        so.On("run", func(msg string) {
        })
        so.On("end", func(msg string) {
            end = time.Now()
            fmt.Println("performance: ", end.Sub(start))
        })

        if(server.Count()==1){
            start = time.Now()
        }
        if(server.Count()>=settings.MaxConn){
            end = time.Now()
            fmt.Println("clients: ", end.Sub(start))
            os.Exit(0)
        }
    })

    http.Handle("/", server)
    log.Fatal(http.ListenAndServe(":8080", nil))
}