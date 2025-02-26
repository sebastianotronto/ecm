#include "bigint.h"

#include <cstdint>
#include <iostream>

constexpr BigInt N(NUMBER);

BigInt<> find_factor() {
	for (BigInt i = 2; i*i < N; i += 1)
		if (N % i == 0)
			return i;
	return -1;
}

int main() {
	// N is a compile-time constant
	if (auto f = find_factor(); f > 1 && f < N)
		std::cout << N << " = " << f << " * " << N/f << std::endl;
	else
		std::cout << N << " is prime" << std::endl;

	return 0;
}
