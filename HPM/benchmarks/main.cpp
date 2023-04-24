#include <utility>
#include <vector>
#include <type_traits>
#include <benchmark/benchmark.h>
#include "Matrix.h"




template <typename T>
void matrixIteration(bombace::Matrix<T>& m, size_t _size) {
	for (auto dim = m.dimItBegin(); dim != m.dimItEnd(); ++dim) {
		for (auto el = dim.vecItBegin(); el != dim.vecItEnd(); ++el) {
			*(el) += rand();
		}
	}
}

template <typename T>
void matrixIteration(std::vector<T>& m, size_t _size) {
	for (size_t i = 0; i < _size * _size; ++i) {
		m[i] += rand();
	}

}

constexpr size_t size = 500;

static void bombace_Matrix(benchmark::State& s)
{
	std::vector<int> v(size * size, 0);
	std::generate(v.begin(), v.end(), rand);
	bombace::Matrix<int> A{ std::move(v), size, size };

	for (auto _ : s)
	{
		matrixIteration(A, size);
	}
}
BENCHMARK(bombace_Matrix); // Register the above function with picobench


static void vector_Matrix(benchmark::State& s)
{
	std::vector<int> v(size * size, 0);
	std::generate(v.begin(), v.end(), rand);
	for (auto _ : s)
	{
		matrixIteration(v, size);
	}
}
BENCHMARK(vector_Matrix); // Register the above function with picobench



//#include <benchmark/benchmark.h>
//static void BM_StringCreation(benchmark::State& state) {
//
//  for (auto _ : state)
//    std::string empty_string;
//}
//// Register the function as a benchmark
//
//BENCHMARK(BM_StringCreation);// Define another benchmark
//
//static void BM_StringCopy(benchmark::State& state) {
//  std::string x = "hello";
//  for (auto _ : state)
//    std::string copy(x);
//}
//BENCHMARK(BM_StringCopy);

BENCHMARK_MAIN();