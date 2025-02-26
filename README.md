# Elliptic curve factorization method

Slides and code for the a presentation about
[Lenstra's elliptic-curve factorization](https://en.wikipedia.org/wiki/Lenstra_elliptic-curve_factorization).

See also [this blog post](https://sebastiano.tronto.net/blog/2025-02-27-ecm).

## Abstract

Elliptic curves are mathematical objects that have both a geometric and an
arithmetic side. They turn out to be useful for real-world applications
because they sit in a sweet spot: they are complicated enough to have
interesting and useful arithmetic properties, but simple enough to be
implemented in software in an efficient way. For example, they are used in
cryptographic schemes, such as the Elliptic-curve Diffie-Hellman scheme,
to obtain greater security with smaller keys.

After introducing elliptic curves and modular arithmetic, we will take a
look at the elliptic curve factorization method (ECM), one of the most
efficient method to find the prime factors of an integer number. We
will see in practice how much faster this method is compared to a naive
algorithm, and we'll see that the implementation of this method is not
that hard at all.

## Slides

The slides are a single html file, `index.html`. They rely on a couple
of external JavaScript libraries. They are also hosted
[here](https://sebastiano.tronto.net/talks/ecm).

## Code

The folder `code/python` contains two files:

* `ecm.py`: an implementation of the ECM algorithm.
* `naive.py`: an implementation of the simple O(√n) algorithm for finding
  a factor of a number, for comparison.

To use any of the two, pass the number to factor as a command-line argument,
for example:

```
$ ./ecm.py 255000007030000033
255000007030000033 = 510000011 * 500000003
```

Some benchmarks (note: the ECM is randomized, the time can vary a lot):

```
$ time ./ecm.py 255000007030000033
255000007030000033 = 510000011 * 500000003
    0m01.45s real     0m01.43s user     0m00.01s system
$ time ./naive.py 255000007030000033
255000007030000033 = 500000003 * 510000011
    0m26.62s real     0m26.50s user     0m00.01s system
```

### C++ code (experimental)

The folder `code/cpp` contains an experimental implementation of the ECM
algorithm in C++. It works, but it is very slow: it requires suppport
for compile-time big integers, which I implement in an inefficient way. I
may optimize this code in the future.
