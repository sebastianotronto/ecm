#! /bin/env python

from sys import argv
from random import randint
from math import sqrt
from dataclasses import dataclass

@dataclass
class Point:
	x: int = 0
	y: int = 0
	is_zero: bool = False

@dataclass
class FactorFound(Exception):
	factor: int = 0

# Returns gcd(a, b) and x, y such that ax + by = gcd(a,b)
def extended_gcd(a: int, b: int) -> (int, int, int):
	if b == 0:
		return a, 1, 0
	g, x, y = extended_gcd(b, a % b)
	return g, y, x - y * (a // b)

def inverse_modulo(a: int, n: int) -> int:
	g, x, y = extended_gcd(a, n)
	if g != 1:
		raise FactorFound(g)
	return x

def ec_sum(p: Point, q: Point, A: int, n: int) -> Point:
	if p.is_zero:
		return q
	if q.is_zero:
		return p
	if (p.x - q.x) % n == 0 and (p.y + q.y) % n == 0:
		return Point(is_zero = True)

	if (p.x - q.x) % n != 0:
		k = ((p.y - q.y) * inverse_modulo(p.x - q.x, n)) % n
	else:
		k = ((3 * p.x**2 + A) * inverse_modulo(p.y + q.y, n)) % n

	x = (k**2 - p.x - q.x) % n
	y = (k * (p.x - x) - p.y) % n

	return Point(x = x, y = y)

def ec_mul(M: int, p: Point, A: int, n: int) -> Point:
	if M == 0:
		return Point(is_zero = True)
	if M % 2 == 0:
		return ec_mul(M // 2, ec_sum(p, p, A, n), A, n)
	return ec_sum(p, ec_mul(M - 1, p, A, n), A, n)

# Elliptic curve factorization method
# If n is prime, this method goes into an infinite loop
def find_factor(n: int) -> int:
	bound = max(int(sqrt(sqrt(sqrt(n)))), 256)

	while True:
		A = randint(0,n)
		p = Point(x = randint(0,n), y = randint(0,n))
		M = 2
		while M < bound and not p.is_zero:
			try:
				p = ec_mul(M, p, A, n)
			except FactorFound as ff:
				#print("Found with A =", A, "M =", M, "and P =", p)
				return ff.factor
			M += 1

N = int(argv[-1])
f = find_factor(N)
print(N, "=", f, "*", N//f)
