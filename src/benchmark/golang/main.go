package main

import (
	"encoding/json"
	"fmt"
	"os"
	"time"

	ericDecimal "github.com/ericlagergren/decimal"
	shopDecimal "github.com/shopspring/decimal"
)

var ITERATIONS int = 100000

type TestResult struct {
	Label      string `json:"label"`
	Result     any    `json:"result"`
	Expected   string `json:"expected"`
	Time       int64  `json:"time"`
	Iterations int    `json:"iterations"`
	IsMatch    bool   `json:"isMatch"`
}

type TestInputShop struct {
	X shopDecimal.Decimal `json:"x"`
	N shopDecimal.Decimal `json:"n"`
}

type TestInputEric struct {
	X *ericDecimal.Big `json:"x"`
	N *ericDecimal.Big `json:"n"`
}

func benchmark(function func() any, iter int) int64 {
	start := time.Now()
	for i := 0; i < iter; i++ {
		_ = function()
	}
	return time.Since(start).Nanoseconds() // Return nanoseconds as integer
}

func test[T any](label string, input T, expect string, function func(input T) any, iter int) TestResult {
	result := function(input)

	if result != expect {
		fmt.Println("Error: expected", expect, "but got", result)
	}

	return TestResult{
		Label:      label,
		Result:     result,
		Expected:   expect,
		Time:       benchmark(func() any { return function(input) }, iter),
		Iterations: iter,
		IsMatch:    result == expect,
	}
}

func shopTestSuite(label, x, n, expected string, iter *int) TestResult {
	if iter == nil {
		iter = &ITERATIONS
	}

	input := TestInputShop{
		X: shopDecimal.RequireFromString(x),
		N: shopDecimal.RequireFromString(n),
	}
	return test(label, input, expected, func(input TestInputShop) any {
		return input.X.Mul(input.N).String()
	}, *iter)
}

func ericTestSuite(label, x, n, expected string, iter *int, precision_conf *int) TestResult {
	if iter == nil {
		iter = &ITERATIONS
	}

	if precision_conf == nil {
		len_expected := len(expected)
		precision_conf = &len_expected
	}

	ericRes := ericDecimal.WithPrecision(*precision_conf)
	ericX, isOk := ericDecimal.WithPrecision(*precision_conf).SetString(x)
	if !isOk {
		panic("Failed to set X")
	}
	ericN, isOk := ericDecimal.WithPrecision(*precision_conf).SetString(n)
	if !isOk {
		panic("Failed to set N")
	}
	input := TestInputEric{
		X: ericX,
		N: ericN,
	}

	return test(label, input, expected, func(input TestInputEric) any {
		return fmt.Sprintf("%f", ericRes.Mul(input.X, input.N))
	}, *iter)
}

func main() {
	precisionTest := 100

	testcases := []TestResult{
		shopTestSuite("go_big_shop", "987654321987654321987654321", "123456789123456789123456789", "121932631356500531591068431581771069347203169112635269", nil),
		ericTestSuite("go_big_eric", "987654321987654321987654321", "123456789123456789123456789", "121932631356500531591068431581771069347203169112635269", nil, nil),
		ericTestSuite("go_big_100_eric", "987654321987654321987654321", "123456789123456789123456789", "121932631356500531591068431581771069347203169112635269", nil, &precisionTest),
		shopTestSuite("go_n14_shop", "0.1", "100000000000000", "10000000000000", nil),
		ericTestSuite("go_n14_eric", "0.1", "100000000000000", "10000000000000.0", nil, nil),
		shopTestSuite("go_n26_shop", "0.1", "100000000000000000000000000", "10000000000000000000000000", nil),
		ericTestSuite("go_n26_eric", "0.1", "100000000000000000000000000", "10000000000000000000000000.0", nil, nil),
	}

	// Save as JSON (with numeric values)
	file, _ := json.MarshalIndent(testcases, "", "  ")
	_ = os.WriteFile(fmt.Sprintf("go_benchmark_%d.json", ITERATIONS), file, 0o644)
	fmt.Printf("Go benchmark saved to go_benchmark_%d.json\n", ITERATIONS)
}
