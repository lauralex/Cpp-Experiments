/*
 * COMPILER OPTIONS IN VISUAL STUDIO
 * /permissive- /GS /Qpar /GL /W3 /Gy /Zc:wchar_t /Zi /Gm- /O2 /sdl /Fd"x64\Release\vc142.pdb" /Zc:inline /fp:precise /D "NDEBUG" /D "_CONSOLE" /D "_UNICODE" /D "UNICODE" /errorReport:prompt /WX- /Zc:forScope /arch:AVX /Gd /Oi /MD /FC /Fa"x64\Release\" /EHsc /nologo /Fo"x64\Release\" /Fp"x64\Release\ShallowCopy.pch" /diagnostics:column /Qpar-report:2
 */


#include <algorithm>
#include <array>
#include <iostream>
#include <vector>
#include <chrono>

const int Size = 1000000;

int main()
{
	std::vector<int> array1(Size), array2(Size);
	std::vector<int> array3(Size);
	std::generate(array1.begin(), array1.end(), []() { return rand() % 100; });
	std::generate(array2.begin(), array2.end(), []() { return rand() % 100; });

	const auto t1 = std::chrono::high_resolution_clock::now();

#pragma loop(ivdep)  // This loop will probably NOT be parallelized (if you add the following line, the parallelization will be forced)
#pragma loop(hint_parallel(4)) // This will use 4 threads
	for (auto i = 0; i < Size; i++)
	{
		array3[i] = array1[i] * static_cast<int>(sqrt(array1[i])) + static_cast<int>(sqrt(array2[i] * 5));
	}

	const auto t2 = std::chrono::high_resolution_clock::now();

	/*for (size_t i = 0; i < size; i++)
	{
		std::cout << array3[i] << std::endl;
	}*/

	std::cout << "Duration: " << std::chrono::duration_cast<std::chrono::duration<float>>(t2 - t1).count() << std::endl;
	return 0;
}
