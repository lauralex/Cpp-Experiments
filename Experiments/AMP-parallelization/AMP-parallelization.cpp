#include <algorithm>
#include <amp.h>
#include <chrono>
#include <iostream>
#include <vector>

const auto size = 10;

int main()
{
	std::vector<float> vector1(size);
	std::vector<float> vector2(size);
	//std::vector<float> sum_vector(size);
	std::generate(vector1.begin(), vector1.end(), []() { return rand() % 100; });
	std::generate(vector2.begin(), vector2.end(), []() { return rand() % 100; });
	const concurrency::array_view<float, 1> sum_view(size);
	const concurrency::array_view<const float, 1> vector1_view(size, vector1);
	const concurrency::array_view<const float, 1> vector2_view(size, vector2);

	sum_view.discard_data();

	const auto t1 = std::chrono::high_resolution_clock::now();

	parallel_for_each(
		sum_view.extent,
		[=](const concurrency::index<1> idx) restrict(amp)
		{
			sum_view[idx] = vector1_view[idx] + vector2_view[idx];
		}
	);

	const auto t2 = std::chrono::high_resolution_clock::now();


	for (auto i = 0; i < size; i++)
	{
		std::cout << "Sum [" << i << "]: " << sum_view[i] << std::endl;
	}

	std::cout << std::endl;
	std::cout << "Duration: " << std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1).count() <<
		std::endl;

	return 0;
}
