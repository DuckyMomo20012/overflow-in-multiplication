import decimal
import json
import gmpy2
from mpmath import mp
from time import perf_counter_ns

# Setup decimal precision
decimal.getcontext().prec = 50
mp.dps = 50  # Set precision for mpmath
gpmy2Ctx = gmpy2.get_context() # type: ignore
gpmy2Ctx.precision = 100 # Set precision for gpmy2

ITERATIONS = 100000

# Benchmark function (returns time in nanoseconds)
def benchmark(func):
    start = perf_counter_ns()
    for _ in range(ITERATIONS):
        func()
    end = perf_counter_ns()
    return end - start  # Single run

def test(input, expected, func):
    res = func(input)
    if res != expected:
        print("Error: expected " + expected + " but got " + res)

    return {
        "result": res,
        "time": benchmark(lambda: func(input)),
        "expected": expected
    }


# Multiplication functions
def decimal_mult(input):
    res = input["x"] * input["n"]
    return str(res.quantize(decimal.Decimal("1")))

def mpmath_mult(input):
    res = (input["x"] * input["n"])
    return str(res)

def gmpy2_mult(input):
    res = (input["x"] * input["n"])

    return "{:.1f}".format(res)

# Run benchmarks
testcases = {
    "n12_decimal": test({
        "x": decimal.Decimal("0.1"),
        "n": decimal.Decimal("1000000000000")
    }, "100000000000", decimal_mult),
    "n13_decimal": test({
        "x": decimal.Decimal("0.1"),
        "n": decimal.Decimal("10000000000000")
    }, "1000000000000", decimal_mult),
    "n14_decimal": test({
        "x": decimal.Decimal("0.1"),
        "n": decimal.Decimal("100000000000000")
    }, "10000000000000", decimal_mult),
    "n24_decimal": test({
        "x": decimal.Decimal("0.1"),
        "n": decimal.Decimal("1000000000000000000000000")
    }, "100000000000000000000000", decimal_mult),
    "n25_decimal": test({
        "x": decimal.Decimal("0.1"),
        "n": decimal.Decimal("10000000000000000000000000")
    }, "1000000000000000000000000", decimal_mult),
    "n26_decimal": test({
        "x": decimal.Decimal("0.1"),
        "n": decimal.Decimal("100000000000000000000000000")
    }, "10000000000000000000000000", decimal_mult),
    "n12_mpmath": test({
        "x": mp.mpf("0.1"),
        "n": mp.mpf("1000000000000")
    }, "100000000000.0", mpmath_mult),
    "n13_mpmath": test({
        "x": mp.mpf("0.1"),
        "n": mp.mpf("10000000000000")
    }, "1000000000000.0", mpmath_mult),
    "n14_mpmath": test({
        "x": mp.mpf("0.1"),
        "n": mp.mpf("100000000000000")
    }, "10000000000000.0", mpmath_mult),
    "n24_mpmath": test({
        "x": mp.mpf("0.1"),
        "n": mp.mpf("1000000000000000000000000")
    }, "100000000000000000000000.0", mpmath_mult),
    "n25_mpmath": test({
        "x": mp.mpf("0.1"),
        "n": mp.mpf("10000000000000000000000000")
    }, "1000000000000000000000000.0", mpmath_mult),
    "n26_mpmath": test({
        "x": mp.mpf("0.1"),
        "n": mp.mpf("100000000000000000000000000")
    }, "10000000000000000000000000.0", mpmath_mult),
    "n12_gmpy2": test({
        "x": gmpy2.mpfr("0.1"), # type: ignore
        "n": gmpy2.mpfr("1000000000000") # type: ignore
    }, "100000000000.0", gmpy2_mult),
    "n13_gmpy2": test({
        "x": gmpy2.mpfr("0.1"), # type: ignore
        "n": gmpy2.mpfr("10000000000000") # type: ignore
    }, "1000000000000.0", gmpy2_mult),
    "n14_gmpy2": test({
        "x": gmpy2.mpfr("0.1"), # type: ignore
        "n": gmpy2.mpfr("100000000000000") # type: ignore
    }, "10000000000000.0", gmpy2_mult),
    "n24_gmpy2": test({
        "x": gmpy2.mpfr("0.1"), # type: ignore
        "n": gmpy2.mpfr("1000000000000000000000000") # type: ignore
    }, "100000000000000000000000.0", gmpy2_mult),
    "n25_gmpy2": test({
        "x": gmpy2.mpfr("0.1"), # type: ignore
        "n": gmpy2.mpfr("10000000000000000000000000") # type: ignore
    }, "1000000000000000000000000.0", gmpy2_mult),
    "n26_gmpy2": test({
        "x": gmpy2.mpfr("0.1"), # type: ignore
        "n": gmpy2.mpfr("100000000000000000000000000") # type: ignore
    }, "10000000000000000000000000.0", gmpy2_mult),
}

# Save results to JSON
with open("python_benchmark.json", "w") as f:
    json.dump(testcases, f, indent=2)

print("Python benchmark saved to python_benchmark.json")
