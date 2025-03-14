import decimal
import json
import gmpy2
from mpmath import mp
from time import perf_counter_ns

ITERATIONS = 100000

# Benchmark function (returns time in nanoseconds)
def benchmark(func, iter = ITERATIONS):
    start = perf_counter_ns()
    for _ in range(iter):
        func()
    end = perf_counter_ns()
    return end - start  # Single run

def test(input, expected, func, iter = ITERATIONS):
    res = func(input)
    if res != expected:
        print("Error: expected " + expected + " but got " + res)

    return {
        "result": res,
        "expected": expected,
        "time": benchmark(lambda: func(input), iter),
        "iterations": iter,
        "isMatch": res == expected
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

def decimalTestSuite(x, n, expected, iter = ITERATIONS, precision_conf = None):
    if (precision_conf is None):
        precision_conf = len(expected)

    # Setup decimal precision
    decimal.getcontext().prec = precision_conf

    input = {
        "x": decimal.Decimal(x),
        "n": decimal.Decimal(n)
    }
    return test(input, expected, decimal_mult, iter)

def mpmathTestSuite(x, n, expected, iter = ITERATIONS, precision_conf = None):
    if (precision_conf is None):
        precision_conf = len(expected)

    mp.prec = precision_conf  # Set precision for mpmath
    mp.dps = precision_conf  # Set precision for mpmath

    input = {
        "x": mp.mpf(x),
        "n": mp.mpf(n)
    }
    return test(input, expected, mpmath_mult, iter)

def gmpy2TestSuite(x, n, expected, iter = ITERATIONS, precision_conf = 53):
    gmpy2Ctx = gmpy2.get_context() # type: ignore
    gmpy2Ctx.precision = precision_conf # Set precision for gpmy2

    input = {
        "x": gmpy2.mpfr(x), # type: ignore
        "n": gmpy2.mpfr(n) # type: ignore
    }
    return test(input, expected, gmpy2_mult, iter)

# Run benchmarks
testcases = {
    "py_big_decimal": decimalTestSuite("987654321987654321987654321", "123456789123456789123456789", "121932631356500531591068431581771069347203169112635269"),
    "py_big_100_decimal": decimalTestSuite("987654321987654321987654321", "123456789123456789123456789", "121932631356500531591068431581771069347203169112635269", precision_conf=100),
    "py_big_mpmath": mpmathTestSuite("987654321987654321987654321", "123456789123456789123456789", "121932631356500531591068431581771069347203169112635269.0"),
    "py_big_100_mpmath": mpmathTestSuite("987654321987654321987654321", "123456789123456789123456789", "121932631356500531591068431581771069347203169112635269.0", precision_conf=100),
    "py_big_gmpy2": gmpy2TestSuite("987654321987654321987654321", "123456789123456789123456789", "121932631356500531591068431581771069347203169112635269.0", precision_conf=256),
    "py_n14_decimal": decimalTestSuite("0.1", "100000000000000", "10000000000000"),
    "py_n14_mpmath": mpmathTestSuite("0.1", "100000000000000", "10000000000000.0"),
    "py_n14_gmpy2": gmpy2TestSuite("0.1", "100000000000000", "10000000000000.0", precision_conf=128),
    "py_n26_decimal": decimalTestSuite("0.1", "100000000000000000000000000", "10000000000000000000000000"),
    "py_n26_mpmath": mpmathTestSuite("0.1", "100000000000000000000000000", "10000000000000000000000000.0"),
    "py_n26_gmpy2": gmpy2TestSuite("0.1", "100000000000000000000000000", "10000000000000000000000000.0", precision_conf=128),
}

# Save results to JSON
with open(f"python_benchmark_{ITERATIONS}.json", "w") as f:
    json.dump(testcases, f, indent=2)

print(f"Python benchmark saved to python_benchmark_{ITERATIONS}.json")
