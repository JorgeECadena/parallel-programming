/*
 * Speed up = (22153.4 / 0.0136203) = 1,626,498.6821141972
 * Efficiency = (1,626,498.6821141972 / 512) = 3,176.7552385043
 * */
#include <iostream>
#include <chrono>
#include <cmath>
#include <cuda_runtime.h>

#define SIZE 5000000
#define N 10
#define THREADS 512
#define BLOCKS std::min(4, ((SIZE / THREADS) + 1))

using namespace std::chrono;

__device__ bool isPrime(int n) {
	if (n < 2) return false;

	for (int i = 2; i <= sqrtf(n); i++) {
		if (n % i == 0) return false;
	}

	return true;
}

__global__ void sumPrimes(long *counts) {
	__shared__ long cache[THREADS];

	int index = threadIdx.x + (blockIdx.x * blockDim.x);
	int cacheIdx = threadIdx.x;

	long acum;
	
	acum = 0;
	while (index < SIZE) {
		acum += isPrime(index) ? index : 0;
		index += blockDim.x * gridDim.x;
	}

	cache[cacheIdx] = acum;
	__syncthreads();

	int i = blockDim.x / 2;
	while (i > 0) {
		if (cacheIdx < i) {
			cache[cacheIdx] += cache[cacheIdx + i];
		}

		__syncthreads();
		i /= 2;
	}

	if (cacheIdx == 0) {
		counts[blockIdx.x] = cache[cacheIdx];
	}
}

int main() {
	long count;
	long counts[BLOCKS] = {0};
	long *deviceCounts;

	high_resolution_clock::time_point start, end;
	double timeElapsed;

	cudaMalloc((void**) &deviceCounts, BLOCKS * sizeof(long));

	timeElapsed = 0;
	for (int j = 0; j < N; j++) {
		start = high_resolution_clock::now();

		sumPrimes<<<BLOCKS, THREADS>>>(deviceCounts);

		end = high_resolution_clock::now();
		timeElapsed += duration<double, std::milli>(end - start).count();
	}

	cudaMemcpy(counts, deviceCounts, BLOCKS * sizeof(long), cudaMemcpyDeviceToHost);

	count = 0;
	for (int i = 0; i < BLOCKS; i++) {
		count += counts[i];
	}

	std::cout << "Sum: " << count << "\n";
	std::cout << "Average execution time: " << timeElapsed / N << "ms\n";

	cudaFree(deviceCounts);

	return 0;
}
