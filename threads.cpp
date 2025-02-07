/*
 * Speed up = (5374.03 / 1096.66) = 4.9003610964
 * Efficency = (4.9003610964 / 10) = .49003610964
 * */
#include <iostream>
#include <chrono>
#include <thread>
#include "utils.h"

#define SIZE 5000000
#define N 10
#define THREADS 10

using namespace std::chrono;

void sumPrimes(int start, int end, long *count) {
	for (int i = start; i < end; i++) {
		*count += isPrime(i) ? i : 0;
	}
}

int main() {
	long count;
	long counts[THREADS] = {0};

	high_resolution_clock::time_point start, end;
	double timeElapsed;

	std::thread threads[THREADS];

	// Variables to iterate over the counts array
	int blockSize, remainder;
	int startIdx, endIdx;

	blockSize = SIZE / THREADS;
	remainder = SIZE % THREADS;

	for (int j = 0; j < N; j++) {
		start = high_resolution_clock::now();

		startIdx = 0;
		for (int i = 0; i < THREADS; i++) {
			endIdx = startIdx + blockSize + ((i < remainder) ? 1 : 0);
			threads[i] = std::thread(sumPrimes, startIdx, endIdx, &counts[i]);
			startIdx = endIdx;
		}

		for (int i = 0; i < THREADS; i++) {
			threads[i].join();
		}

		end = high_resolution_clock::now();
		timeElapsed += duration<double, std::milli>(end - start).count();
	}

	count = 0;
	for (int i = 0; i < THREADS; i++) {
		count += counts[i];
	}
	count /= N;

	std::cout << "Sum: " << count << "\n";
	std::cout << "Average execution time: " << timeElapsed / N << "\n";

	return 0;
}
