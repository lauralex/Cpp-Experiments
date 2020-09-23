#ifdef _WIN32
#pragma comment ( lib, "Shlwapi.lib" )
#ifdef _DEBUG
#pragma comment ( lib, "benchmarkd.lib" )
#else
#pragma comment ( lib, "benchmark.lib" )
#endif
#endif

#include <iostream>
#include <numeric>
#include <valarray>
#include <benchmark/benchmark.h>

static void valarr(benchmark::State& state)
{
	std::valarray<int> xs = { 1,3,-5 };
	std::valarray<int> ys = {4, -2, -1};

	for (auto _ : state)
	{
		auto res = (xs * ys).sum();
		benchmark::DoNotOptimize(res);
	}
}

BENCHMARK(valarr);

static void inner(benchmark::State& state)
{
	int a[] = { 1, 3, -5 };
	int b[] = { 4, -2, -1 };
	
	for (auto _ : state)
	{
		auto cocco = std::inner_product(a, a + sizeof(a) / sizeof(a[0]), b, 0);
		benchmark::DoNotOptimize(cocco);
	}
}

BENCHMARK(inner);

BENCHMARK_MAIN();
