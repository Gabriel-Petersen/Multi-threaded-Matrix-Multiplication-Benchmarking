#pragma once
#include <vector>
#include <random>

namespace bigmatrices 
{
	enum class MultiplicationMode {
		Trivial, TransposedTrivial, Blocking, TransposedBlocking, Parallel, TransposedParallel
	};

	class BigSquareMatrix 
	{
		std::vector<double> data;
		int ss;

	public:
		BigSquareMatrix(int size) :
			ss(size), data(std::vector((size* size), (double)0)) {
		}

		void fillWithRand() {
			for (int i = 0; i < data.size(); ++i)
				data[i] = rand();
		}

		void zero() {
			std::fill(data.begin(), data.end(), 0);
		}

		double get(int i, int j) const {
			return data[i * ss + j];
		}
		void set(int i, int j, double newValue) {
			data[i * ss + j] = newValue;
		}

		int size() const {
			return ss;
		}

		BigSquareMatrix transposed()
		{
			BigSquareMatrix result(ss);
			for (int i = 0; i < ss; ++i)
				for (int j = 0; j < ss; ++j)
					result.set(j, i, this->get(i, j));
			return result;
		}
	};

	int multiplyMatrices(BigSquareMatrix& a, BigSquareMatrix& b, BigSquareMatrix& output, MultiplicationMode mode);
}