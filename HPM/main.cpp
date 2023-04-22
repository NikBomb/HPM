#include <vector>
#include <utility>
#include <type_traits>
#include "Matrix.h"
#define PICOBENCH_IMPLEMENT_WITH_MAIN
#include "picobench.h"


template <typename T>
void matrixIteration(bombace::Matrix<T>& m, size_t _size) {

	for (auto col = bombace::ColIterator(m).begin(); col != bombace::ColIterator(m).end(); ++col) {
		for (int i = 0; i < _size; i++) {
			col.at(i) += rand();
		}
	}

}

template <typename T>
void matrixIteration(std::vector<T>& m, size_t _size) {
	for (size_t i = 0; i < _size * _size; ++i) {
		m[i] += rand();
	}

}

constexpr size_t size = 300;

// Benchmarking function written by the user:
static void bombace_Matrix(picobench::state& s)
{
	std::vector<int> v(size * size, 0);
	std::generate(v.begin(), v.end(), rand);
	bombace::Matrix<int> A{ std::move(v), size, size };

	for (auto _ : s)
	{
		matrixIteration(A, size);
	}
}
PICOBENCH(bombace_Matrix); // Register the above function with picobench


static void vector_Matrix(picobench::state& s)
{
	std::vector<int> v(size * size, 0);
	std::generate(v.begin(), v.end(), rand);
	for (auto _ : s)
	{
		matrixIteration(v, size);
	}
}
PICOBENCH(vector_Matrix); // Register the above function with picobench
