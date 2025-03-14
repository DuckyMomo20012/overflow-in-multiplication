import { writeFileSync } from "fs";
import Decimal from "decimal.js";
import BigNumber from "bignumber.js";

const ITERATIONS = 100000;

// Benchmark function (single run, returns nanoseconds)
function benchmark(func, iter = ITERATIONS) {
  const start = process.hrtime.bigint();
  for (let i = 0; i < iter; i++) func();
  const end = process.hrtime.bigint();
  return Number(end - start); // Convert BigInt to number (nanoseconds)
}

function test(input, expected, func, iter = ITERATIONS) {
  const res = func(input);
  if (res !== expected) {
    console.error(`Error: expected ${expected}, but got ${res}`);
  }

  return {
    result: res,
    expected: expected,
    time: benchmark(() => func(input, iter)),
    iterations: iter,
    isMatch: res === expected,
  };
}

const decimalTestSuite = (
  x,
  n,
  expected,
  iter = ITERATIONS,
  precision_conf = expected.length
) => {
  Decimal.precision = precision_conf;

  const input = { x: new Decimal(x), n: new Decimal(n) };
  return test(input, expected, (input) => input.x.mul(input.n).toFixed(), iter);
};

const bigNumberTestSuite = (x, n, expected, iter = ITERATIONS) => {
  const input = { x: new BigNumber(x), n: new BigNumber(n) };
  return test(
    input,
    expected,
    (input) => input.x.times(input.n).toFixed(),
    iter
  );
};

const testcases = {
  js_big_decimal: decimalTestSuite(
    "987654321987654321987654321",
    "123456789123456789123456789",
    "121932631356500531591068431581771069347203169112635269"
  ),
  js_big_100_decimal: decimalTestSuite(
    "987654321987654321987654321",
    "123456789123456789123456789",
    "121932631356500531591068431581771069347203169112635269",
    100
  ),
  js_big_decimal: bigNumberTestSuite(
    "987654321987654321987654321",
    "123456789123456789123456789",
    "121932631356500531591068431581771069347203169112635269"
  ),
  js_n14_decimal: decimalTestSuite("0.1", "100000000000000", "10000000000000"),
  js_n14_bignumber: bigNumberTestSuite(
    "0.1",
    "100000000000000",
    "10000000000000"
  ),
  js_n26_decimal: decimalTestSuite(
    "0.1",
    "100000000000000000000000000",
    "10000000000000000000000000"
  ),
  js_n26_bignumber: bigNumberTestSuite(
    "0.1",
    "100000000000000000000000000",
    "10000000000000000000000000"
  ),
};

// Save results in nanoseconds
writeFileSync(
  `js_benchmark_${ITERATIONS}.json`,
  JSON.stringify(testcases, null, 2)
);
console.log(`JavaScript benchmark saved to js_benchmark_${ITERATIONS}.json`);
