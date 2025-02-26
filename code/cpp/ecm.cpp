#include "bigint.h"
#include "zmodn.h"

#include <cstdint>
#include <iostream>
#include <variant>

constexpr BigInt N(NUMBER);

class Point {
public:
	bool is_infinity;
	Zmod<N> x;
	Zmod<N> y;

	Point(Zmod<N> a, Zmod<N> b) : is_infinity{false}, x{a}, y{b} {}
	Point(bool inf) : is_infinity{inf}, x{0}, y{0} {}
};

std::variant<Point, BigInt<>>
sum_or_factor(BigInt<> a, Point p, Point q) {
	if (p.is_infinity) return q;
	if (q.is_infinity) return p;
	if (p.x == q.x && p.y + q.y == Zmod<N>(0)) return Point(true);

	Zmod<N> l(0);
	if (p.x != q.x)
		if (auto inv_x = (p.x-q.x).inverse(); inv_x.has_value())
			l = (p.y-q.y) * inv_x.value();
		else
			return std::get<0>(extended_gcd(N, (p.x-q.x).toint()));
	else
		if (auto inv_y = (p.y+q.y).inverse(); inv_y.has_value())
			l = (Zmod<N>(3) * p.x * p.x + a) * inv_y.value();
		else
			return std::get<0>(extended_gcd(N, (p.y+q.y).toint()));

	auto x = l*l - p.x - q.x;
	auto y = l*(p.x-x) - p.y;

	return Point(x, y);
}

std::variant<Point, BigInt<>>
product_or_factor(BigInt<> a, std::variant<Point, BigInt<>> pi, BigInt<> m) {
	if (std::holds_alternative<BigInt<>>(pi))
		return std::get<BigInt<>>(pi);

	auto p = std::get<Point>(pi);

	if (m == 0)
		return Point(true); // Anything multiplied by 0 is 0

	// Divide-and-conquer multiplication (power) algorithm
	if (m % 2 == 0) {
		return product_or_factor(a, sum_or_factor(a, p, p), m/2);
	} else {
		auto pp = product_or_factor(a, p, m-1);
		if (std::holds_alternative<BigInt<>>(pp))
			return std::get<BigInt<>>(pp);
		return sum_or_factor(a, p, std::get<Point>(pp));
	}
}

BigInt<> find_factor_ecm() {
	// Find a factor of the integer N.
	// If N is prime, this method goes into an infinite loop.

	while (true) {
		BigInt a = BigInt<>::random(N);
		Point p(BigInt<>::random(N), BigInt<>::random(N));
		for (BigInt m = 2;
		    (m < 256 || m*m*m*m*m*m*m*m < N) && !p.is_infinity; m += 1) {
			auto x = product_or_factor(a, p, m);
			if (std::holds_alternative<BigInt<>>(x))
				return std::get<BigInt<>>(x);
			else
				p = std::get<Point>(x);
		}
	}
}

int main() {
	// N is a compile-time constant
	if (BigInt f = find_factor_ecm(); f > 1 && f < N)
		std::cout << N << " = " << f << " * " << N/f << std::endl;
	else
		std::cout << N << " is prime" << std::endl;

	return 0;
}
