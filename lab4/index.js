let bigInt = require('big-integer');
let _ = require('lodash');

function generatePrimes(max) {
  return _(_.range(max))
    .filter(i => bigInt(i).isPrime())
    .value();
}

function generatePrimesFast(max) {
  let arr = new Array(max);

  for (let i = 0; i < arr.length; i++) {
    arr[i] = true;
  }

  for (let i = 2; i < arr.length; i++) {
    if (arr[i]) {
      for (let j = i * 2; j < arr.length; j += i) {
        arr[j] = false;
      }
    }
  }

  let res = [];

  for (let i = 2; i < arr.length; i++) {
    if (arr[i]) {
      res.push(i);
    }
  }

  return res;
}

function factor(n) {
  let r = Math.floor(Math.sqrt(n));
  let primes = generatePrimesFast(r);

  let divisors = _(primes)
    .filter(i => bigInt(n).mod(i) == 0)
    .value();

  return {
    p: divisors[0],
    q: n / divisors[0]
  };
}

function randomRange(min, max) {
  return Math.floor(Math.random() * (max - min)) + min;
}

function primeBenchmark() {
  let d1 = new Date();
  let primes2 = generatePrimesFast(1000000);
  console.log(`Fast algo: ${new Date() - d1}ms`);

  let d2 = new Date();
  let primes = generatePrimes(1000000);
  console.log(`Stupid algo: ${new Date() - d2}ms`);
}

function main() {
  let m = 778877;
  let n = 565570077646207;
  let e = 12341;
  console.log(`m: ${m}, n: ${n}, e: ${e}`);

  let p, q;
  ({p, q} = factor(n));
  console.log(`p: ${p}, q: ${q}`);

  let l = bigInt.lcm(p - 1, q - 1);
  console.log(`l: ${l}`);

  let d = bigInt(e).modInv(l);
  console.log(`d: ${d}`);

  let c = randomRange(0, n);
  console.log(`${c} : ${bigInt(c).modPow(e, n).modPow(d, n)}`);
}

main();
