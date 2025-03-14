package main

import (
	"encoding/json"
	"fmt"
	"os"
	"time"

	ericDecimal "github.com/ericlagergren/decimal"
	shopDecimal "github.com/shopspring/decimal"
)

const ITERATIONS int64 = 100000

type TestResult struct {
	Result   any    `json:"result"`
	Time     int64  `json:"time"`
	Expected string `json:"expected"`
}

type TestInputShop struct {
	X shopDecimal.Decimal `json:"x"`
	N shopDecimal.Decimal `json:"n"`
}

type TestInputEric struct {
	X *ericDecimal.Big `json:"x"`
	N *ericDecimal.Big `json:"n"`
}

func benchmark(function func() any) int64 {
	iterations := ITERATIONS
	start := time.Now()
	for i := int64(0); i < iterations; i++ {
		_ = function()
	}
	return time.Since(start).Nanoseconds() // Return nanoseconds as integer
}

func test[T any](input T, expect string, function func(input T) any) TestResult {
	result := function(input)

	if result != expect {
		fmt.Println("Error: expected", expect, "but got", result)
	}

	return TestResult{
		Result:   result,
		Time:     benchmark(func() any { return function(input) }),
		Expected: expect,
	}
}

func main() {
	testcases := map[string]TestResult{
		"n12_shop": test(TestInputShop{
			X: shopDecimal.RequireFromString("0.1"),
			N: shopDecimal.RequireFromString("1000000000000"),
		}, "100000000000", func(input TestInputShop) any {
			return input.X.Mul(input.N).String()
		}),
		"n13_shop": test(TestInputShop{
			X: shopDecimal.RequireFromString("0.1"),
			N: shopDecimal.RequireFromString("10000000000000"),
		}, "1000000000000", func(input TestInputShop) any {
			return input.X.Mul(input.N).String()
		}),
		"n14_shop": test(TestInputShop{
			X: shopDecimal.RequireFromString("0.1"),
			N: shopDecimal.RequireFromString("100000000000000"),
		}, "10000000000000", func(input TestInputShop) any {
			return input.X.Mul(input.N).String()
		}),
		"n24_shop": test(TestInputShop{
			X: shopDecimal.RequireFromString("0.1"),
			N: shopDecimal.RequireFromString("1000000000000000000000000"),
		}, "100000000000000000000000", func(input TestInputShop) any {
			return input.X.Mul(input.N).String()
		}),
		"n25_shop": test(TestInputShop{
			X: shopDecimal.RequireFromString("0.1"),
			N: shopDecimal.RequireFromString("10000000000000000000000000"),
		}, "1000000000000000000000000", func(input TestInputShop) any {
			return input.X.Mul(input.N).String()
		}),
		"n26_shop": test(TestInputShop{
			X: shopDecimal.RequireFromString("0.1"),
			N: shopDecimal.RequireFromString("100000000000000000000000000"),
		}, "10000000000000000000000000", func(input TestInputShop) any {
			return input.X.Mul(input.N).String()
		}),
		"n12_eric": test(TestInputEric{
			X: ericDecimal.New(1, 1),
			N: ericDecimal.New(1, -12),
		}, "100000000000", func(input TestInputEric) any {
			ericRes := new(ericDecimal.Big)
			return fmt.Sprintf("%f", ericRes.Mul(input.X, input.N))
		}),
		"n13_eric": test(TestInputEric{
			X: ericDecimal.New(1, 1),
			N: ericDecimal.New(1, -13),
		}, "1000000000000", func(input TestInputEric) any {
			ericRes := new(ericDecimal.Big)
			return fmt.Sprintf("%f", ericRes.Mul(input.X, input.N))
		}),
		"n14_eric": test(TestInputEric{
			X: ericDecimal.New(1, 1),
			N: ericDecimal.New(1, -14),
		}, "10000000000000", func(input TestInputEric) any {
			ericRes := new(ericDecimal.Big)
			return fmt.Sprintf("%f", ericRes.Mul(input.X, input.N))
		}),
		"n24_eric": test(TestInputEric{
			X: ericDecimal.New(1, 1),
			N: ericDecimal.New(1, -24),
		}, "100000000000000000000000", func(input TestInputEric) any {
			ericRes := new(ericDecimal.Big)
			return fmt.Sprintf("%f", ericRes.Mul(input.X, input.N))
		}),
		"n25_eric": test(TestInputEric{
			X: ericDecimal.New(1, 1),
			N: ericDecimal.New(1, -25),
		}, "1000000000000000000000000", func(input TestInputEric) any {
			ericRes := new(ericDecimal.Big)
			return fmt.Sprintf("%f", ericRes.Mul(input.X, input.N))
		}),
		"n26_eric": test(TestInputEric{
			X: ericDecimal.New(1, 1),
			N: ericDecimal.New(1, -26),
		}, "10000000000000000000000000", func(input TestInputEric) any {
			ericRes := new(ericDecimal.Big)
			return fmt.Sprintf("%f", ericRes.Mul(input.X, input.N))
		}),
	}

	// Save as JSON (with numeric values)
	file, _ := json.MarshalIndent(testcases, "", "  ")
	_ = os.WriteFile("go_benchmark.json", file, 0o644)
	fmt.Println("Go benchmark saved to go_benchmark.json")
}
