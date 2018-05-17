package main

import (
	"fmt"
	"io"
	"net"
)

func main() {
	listener, err := net.Listen("tcp", "0.0.0.0:30000")
	if err != nil {
		panic(err)
	}
	fmt.Println("wait...")
	for {
		conn, err := listener.Accept()
		defer conn.Close()
		if err != nil {
			panic(err)
		}
		go func(conn net.Conn) {
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
		}(conn)
	}
}
