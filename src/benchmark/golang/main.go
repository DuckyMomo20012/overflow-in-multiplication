package main

import (
	"encoding/json"
	"fmt"
	"os"
	"time"

	ericDecimal "github.com/ericlagergren/decimal"
	shopDecimal "github.com/shopspring/decimal"
)

func benchmark(function func() any) int64 {
	start := time.Now()
	for range 100000 {
		_ = function()
	}
	return time.Since(start).Nanoseconds() // Return nanoseconds as integer
}

func main() {
	shopA := shopDecimal.NewFromFloat(12345.6789)
	shopB := shopDecimal.NewFromFloat(98765.4321)

	ericA := ericDecimal.New(12345, -4)
	ericB := ericDecimal.New(98765, -4)
	ericRes := &ericDecimal.Big{}

	results := map[string]int64{
		"shopspring/decimal": benchmark(func() any {
			return shopA.Mul(shopB)
		}),
		"ericlagergren/decimal": benchmark(func() any {
			ericRes = ericRes.Mul(ericA, ericB)
			return ericRes
		}),
	}

	// Save as JSON (with numeric values)
	file, _ := json.MarshalIndent(results, "", "  ")
	_ = os.WriteFile("go_benchmark.json", file, 0o644)
	fmt.Println("Go benchmark saved to go_benchmark.json")
}
