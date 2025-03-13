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

// Setup test numbers
const a = new Decimal("12345.6789");
const b = new Decimal("98765.4321");

// Run benchmarks
const results = {
  "decimal.js": benchmark(() => a.mul(b)),
  "bignumber.js": benchmark(() => new BigNumber(a).times(b)),
};

// Save results in nanoseconds
writeFileSync("js_benchmark.json", JSON.stringify(results, null, 2));
console.log("JavaScript benchmark saved to js_benchmark.json");
