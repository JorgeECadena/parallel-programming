#pragma once

#include <iostream>
#include <cmath>

/**
 * @brief Returns whether a number is prime or not
 *
 * @param int n Number to verify
 *
 * @return bool
 * */
bool isPrime(int n) {
	if (n < 2) return false;

	for (int i = 2; i <= std::pow(n, 0.5); i++) {
		if (n % i == 0) return false;
	}

	return true;
}
