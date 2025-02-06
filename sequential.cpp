#include <iostream>
#include <chrono>
#include "utils.h"

#define SIZE 5000000
#define N 10

using namespace std::chrono;

void sumPrimes(long &count) {
	for (int i = 0; i < SIZE; i++) {
		count += isPrime(i) ? i : 0;
	}
}

int main() {
	long count;

	high_resolution_clock::time_point start, end;
	double timeElapsed;

	count = 0;
	for (int j = 0; j < N; j++) {
		start = high_resolution_clock::now();

		sumPrimes(count);

		end = high_resolution_clock::now();
		timeElapsed += duration<double, std::milli>(end - start).count();
	}
	count /= 10;

	std::cout << "Sum: " << count << "\n";
	std::cout << "Average execution time: " << timeElapsed / 10 << "ms\n";

	return 0;
}
