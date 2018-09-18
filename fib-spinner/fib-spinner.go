package main

import (
	"fmt"
	"time"
)

func main() {
	// Call the spinner, which will run until main returns.
	// main returns when either the timer is up OR fib has completed.
	go spinner(200 * time.Millisecond)

	// Naive "timer" implementation.
	// dur := 10 * time.Second
	// for dur > 0 {
	// 	// fmt.Printf("Waiting..\n")
	// 	dur -= 1 * time.Second
	// 	time.Sleep(1 * time.Second)
	// }

	// Sleep for 2 seconds. Same effect as using timer in the case.
	// time.Sleep(2 * time.Second)

	then := time.Now()

	// Timer blocks on this channel until the timer has completed.
	timer := time.NewTimer(5 * time.Second)
	<-timer.C

	// Naive recursive calculation of fib.
	// const n = 45
	// fibN := fib(n)
	// fmt.Printf("\rFibonacci(%d) = %d\n", n, fibN)
}

func fib(x int) int {
	if x < 2 {
		return x
	}
	return fib(x-1) + fib(x-2)
}

func spinner(delay time.Duration) {
	for {
		for _, c := range `/-|\` {
			fmt.Printf("\r%c", c)
			time.Sleep(delay)
		}
	}
}
