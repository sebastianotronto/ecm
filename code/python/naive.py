#!/bin/env python

from sys import argv
from math import floor, sqrt

def naive_factor(n: int) -> int:
	for i in range(2,floor(sqrt(n))+1):
		if n%i == 0:
			return i
	else:
		return -1

N = int(argv[-1])
f = naive_factor(N)
if f == -1:
	print(N, "is prime")
else:
	print(N, "=", f, "*", N//f)
