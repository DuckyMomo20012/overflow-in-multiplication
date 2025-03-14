import { writeFileSync } from "fs";
import Decimal from "decimal.js";
import BigNumber from "bignumber.js";

const ITERATIONS = 100000;

// Benchmark function (single run, returns nanoseconds)
function benchmark(func) {
  const start = process.hrtime.bigint();
  for (let i = 0; i < ITERATIONS; i++) func();
  const end = process.hrtime.bigint();
  return Number(end - start); // Convert BigInt to number (nanoseconds)
}

function test(input, expected, func) {
  const res = func(input);
  if (res !== expected) {
    console.error(`Error: expected ${expected}, but got ${res}`);
  }

  return {
    result: res,
    time: benchmark(() => func(input)),
    expected: expected,
  };
}

const testcases = {
  n12_decimal: test(
    { x: new Decimal("0.1"), n: new Decimal("1000000000000") },
    "100000000000",
    (input) => input.x.mul(input.n).toFixed()
  ),
  n13_decimal: test(
    { x: new Decimal("0.1"), n: new Decimal("10000000000000") },
    "1000000000000",
    (input) => input.x.mul(input.n).toFixed()
  ),
  n14_decimal: test(
    { x: new Decimal("0.1"), n: new Decimal("100000000000000") },
    "10000000000000",
    (input) => input.x.mul(input.n).toFixed()
  ),
  n24_decimal: test(
    { x: new Decimal("0.1"), n: new Decimal("1000000000000000000000000") },
    "100000000000000000000000",
    (input) => input.x.mul(input.n).toFixed()
  ),
  n25_decimal: test(
    { x: new Decimal("0.1"), n: new Decimal("10000000000000000000000000") },
    "1000000000000000000000000",
    (input) => input.x.mul(input.n).toFixed()
  ),
  n26_decimal: test(
    { x: new Decimal("0.1"), n: new Decimal("100000000000000000000000000") },
    "10000000000000000000000000",
    (input) => input.x.mul(input.n).toFixed()
  ),
  n12_bignumber: test(
    { x: new BigNumber("0.1"), n: new BigNumber("1000000000000") },
    "100000000000",
    (input) => input.x.times(input.n).toFixed()
  ),
  n13_bignumber: test(
    { x: new BigNumber("0.1"), n: new BigNumber("10000000000000") },
    "1000000000000",
    (input) => input.x.times(input.n).toFixed()
  ),
  n14_bignumber: test(
    { x: new BigNumber("0.1"), n: new BigNumber("100000000000000") },
    "10000000000000",
    (input) => input.x.times(input.n).toFixed()
  ),
  n24_bignumber: test(
    { x: new BigNumber("0.1"), n: new BigNumber("1000000000000000000000000") },
    "100000000000000000000000",
    (input) => input.x.times(input.n).toFixed()
  ),
  n25_bignumber: test(
    { x: new BigNumber("0.1"), n: new BigNumber("10000000000000000000000000") },
    "1000000000000000000000000",
    (input) => input.x.times(input.n).toFixed()
  ),
  n26_bignumber: test(
    {
      x: new BigNumber("0.1"),
      n: new BigNumber("100000000000000000000000000"),
    },
    "10000000000000000000000000",
    (input) => input.x.times(input.n).toFixed()
  ),
};

// Save results in nanoseconds
writeFileSync("js_benchmark.json", JSON.stringify(testcases, null, 2));
console.log("JavaScript benchmark saved to js_benchmark.json");
