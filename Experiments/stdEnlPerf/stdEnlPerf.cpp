/*
 * A program to test the performance loss when using std::endl to print a new line
 */


#ifdef _WIN32
#pragma comment ( lib, "Shlwapi.lib" )
#ifdef _DEBUG
#pragma comment ( lib, "benchmarkd.lib" )
#else
#pragma comment ( lib, "benchmark.lib" )
#endif
#endif


#include <iostream>
#include <benchmark/benchmark.h>

static void with_std_endl(benchmark::State& state)
{
	for (auto _ : state)
	{
		std::cout << "Test!" << std::endl;
	}
}

BENCHMARK(with_std_endl);


static void without_std_endl(benchmark::State& state)
{
	for (auto _ : state)
	{
		std::cout << "Test!\n";
	}
}

BENCHMARK(without_std_endl);

BENCHMARK_MAIN();
