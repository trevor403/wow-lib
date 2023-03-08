package main

import "C" // this enables cgo runtime

import (
	"fmt"
)

func init() {
	fmt.Println("this is running in the Go init func")
}

//export go_hello
func go_hello() {
	fmt.Println("Hello!")
}
