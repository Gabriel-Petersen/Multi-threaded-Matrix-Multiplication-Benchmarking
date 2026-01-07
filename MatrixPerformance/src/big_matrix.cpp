#include <big_matrix.h>

#include <thread>

namespace bigmatrices 
{
	static constexpr int min(int a, int b) {
		return a < b ? a : b;
	}

	static void multiplyTrivial(BigSquareMatrix& a, BigSquareMatrix& b, BigSquareMatrix& output)
	{
		if (a.size() != b.size())
			return;

		int s = a.size();
		for (int i = 0; i < s; ++i)
			for (int j = 0; j < s; ++j)
				for (int k = 0; k < s; ++k)
				{
					output.set(i, j, output.get(i, j) + a.get(i, k) * b.get(k, j));
				}
	}

	static void multiplyTransposedTrivial(BigSquareMatrix& a, BigSquareMatrix& b, BigSquareMatrix& output)
	{
		if (a.size() != b.size())
			return;

		int s = a.size();
		BigSquareMatrix tb = b.transposed();
		for (int i = 0; i < s; ++i)
			for (int j = 0; j < s; ++j) 
			{
				double cont = 0;
				for (int k = 0; k < s; ++k)
					cont += a.get(i, k) * tb.get(j, k);
				output.set(i, j, cont);
			}
	}

	static void multiplyBlocking(BigSquareMatrix& a, BigSquareMatrix& b, BigSquareMatrix& output)
	{
		if (a.size() != b.size())
			return;

		int s = a.size();
		constexpr int blockSize = 64;

		for (int bx = 0; bx < s; bx += blockSize)
			for (int by = 0; by < s; by += blockSize)
				for (int bz = 0; bz < s; bz += blockSize)
				{
					for (int i = bx; i < min(bx + blockSize, s); ++i)
						for (int k = bz; k < min(bz + blockSize, s); ++k)
						{
							double tempA = a.get(i, k);
							for (int j = by; j < min(by + blockSize, s); ++j)
								output.set(i, j, output.get(i, j) + tempA * b.get(k, j));
						}
				}
	}

	static void multiplyTransposedBlocking(BigSquareMatrix& a, BigSquareMatrix& b, BigSquareMatrix& output)
	{
		if (a.size() != b.size())
			return;

		int s = a.size();
		constexpr int blockSize = 64;
		BigSquareMatrix tb = b.transposed();

		for (int bx = 0; bx < s; bx += blockSize)
			for (int by = 0; by < s; by += blockSize)
				for (int bz = 0; bz < s; bz += blockSize)
				{
					for (int i = bx; i < min(bx + blockSize, s); ++i)
						for (int j = by; j < min(by + blockSize, s); ++j)
						{
							double cont = 0;
							for (int k = bz; k < min(bz + blockSize, s); ++k)
								cont += a.get(i, k) * tb.get(j, k);
							output.set(i, j, output.get(i, j) + cont);
						}
				}
	}

	static void multiplyParallel(BigSquareMatrix& a, BigSquareMatrix& b, BigSquareMatrix& output)
	{
		if (a.size() != b.size())
			return;

		int s = a.size();
		int numThreads = static_cast<int>(std::thread::hardware_concurrency());
		std::vector<std::thread> threads;

		for (int t = 0; t < numThreads; ++t)
		{
			threads.emplace_back([t, numThreads, s, &a, &b, &output]() 
				{
					for (int i = t; i < s; i += numThreads)
						for (int k = 0; k < s; ++k) 
						{
							double tempA = a.get(i, k);
							for (int j = 0; j < s; ++j)
								output.set(i, j, output.get(i, j) + tempA * b.get(k, j));
						}
				}
			);
		}
		for (auto& th : threads) th.join();
	}

	static void multiplyTransposedParallel(BigSquareMatrix& a, BigSquareMatrix& b, BigSquareMatrix& output)
	{
		if (a.size() != b.size())
			return;

		int s = a.size();
		int numThreads = static_cast<int>(std::thread::hardware_concurrency());
		std::vector<std::thread> threads;
		BigSquareMatrix tb = b.transposed();

		for (int t = 0; t < numThreads; ++t) 
		{
			threads.emplace_back([t, numThreads, s, &a, &tb, &output]() 
			{
				for (int i = t; i < s; i += numThreads)
					for (int j = 0; j < s; ++j) 
					{
						double sum = 0;
						for (int k = 0; k < s; ++k) 
							sum += a.get(i, k) * tb.get(j, k);

						output.set(i, j, sum);
					}
			});
		}
		for (auto& th : threads) th.join();
	}


	int multiplyMatrices(BigSquareMatrix& a, BigSquareMatrix& b, BigSquareMatrix& output, MultiplicationMode mode)
	{
		switch (mode) {
		case MultiplicationMode::Trivial:
			multiplyTrivial(a, b, output);
			break;
		case MultiplicationMode::TransposedTrivial:
			multiplyTransposedTrivial(a, b, output);
			break;
		case MultiplicationMode::Blocking:
			multiplyBlocking(a, b, output);
			break;
		case MultiplicationMode::TransposedBlocking:
			multiplyTransposedBlocking(a, b, output);
			break;
		case MultiplicationMode::Parallel:
			multiplyParallel(a, b, output);
			break;
		case MultiplicationMode::TransposedParallel:
			multiplyTransposedParallel(a, b, output);
			break;
		}
		return 0;
	}
}