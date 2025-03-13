import decimal
import json
import gmpy2
from mpmath import mp
from time import perf_counter_ns

# Setup decimal precision
decimal.getcontext().prec = 50
mp.dps = 50  # Set precision for mpmath

# Test values
a = decimal.Decimal("12345.6789")
b = decimal.Decimal("98765.4321")
mpA = mp.mpf("12345.6789")
mpB = mp.mpf("98765.4321")
gmpA = gmpy2.mpfr("12345.6789")
gmpB = gmpy2.mpfr("98765.4321")

ITERATIONS = 100000

# Benchmark function (returns time in nanoseconds)
def benchmark(func):
    start = perf_counter_ns()
    for _ in range(ITERATIONS):
        func()
    end = perf_counter_ns()
    return end - start  # Single run

# Multiplication functions
def decimal_mult():
    return a * b

def mpmath_mult():
    return mpA * mpB

def gmpy2_mult():
    return gmpA * gmpB

# Run benchmarks
result = {
    "decimal": benchmark(decimal_mult),
    "mpmath": benchmark(mpmath_mult),
    "gmpy2": benchmark(gmpy2_mult),
}

# Save results to JSON
with open("python_benchmark.json", "w") as f:
    json.dump(result, f, indent=2)

print("Python benchmark saved to python_benchmark.json")
