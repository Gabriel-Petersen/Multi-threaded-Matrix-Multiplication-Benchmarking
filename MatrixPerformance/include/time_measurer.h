#pragma once
#include <chrono>
#include <iostream>

class TimeMeasurer
{
public:
	template<typename Func, typename... Args>
	double measure(bool print, Func func, Args... args)
	{
		auto start = std::chrono::high_resolution_clock::now();

		if (print)
			std::cout << "Funcion output: " << func(args...) << "\n";
		else
			func(args...);

		auto end = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double, std::milli> time = end - start;
		return time.count();
	}
};