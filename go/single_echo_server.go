package main

import (
	"fmt"
	"io"
	"net"
	"runtime"
)

func main() {
	runtime.GOMAXPROCS(1)
	listener, err := net.Listen("tcp", "0.0.0.0:30000")
	if err != nil {
		panic(err)
	}
	fmt.Println("wait...")
	for {
		conn, err := listener.Accept()
		if err != nil {
			panic(err)
		}
		_, err = conn.Write([]byte("Hello World!\r\n"))

		buf := make([]byte, 1024)
		n, err := conn.Read(buf)
		if err != nil {
			if err == io.EOF {
				return
			}
			panic(err)
		}
		_, err = conn.Write(buf[:n])
		if err != nil {
			panic(err)
		}
		conn.Close()
	}
}
