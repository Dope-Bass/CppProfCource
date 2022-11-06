#include "alloc.h"
#include <map>

int factorial(const int i) {
    return i == 0 ? 1 : i * factorial(i - 1);
}

int main(int, char *[]) {

	// auto v = std::vector<int, Allocator<int, 10>>{};
	// // v.reserve(5);
	// for (int i = 0; i < 6; ++i) {
	// 	std::cout << "vector size = " << v.size() << std::endl;
	// 	v.emplace_back(i);
	// 	std::cout << std::endl;
	// }

    auto m1 = std::map<int, int>{};

    for ( int i = 0; i < 10; ++i ) {
        m1[i] = factorial(i);
    }

    auto m2 = std::map<
		int,
		int,
		std::less<int>,
		Allocator<
			std::pair<int, int>, 10>>{};

	for (int i = 0; i < 10; ++i) {
		m2[i] = factorial(i);
	}

	for (int i = 0; i < 10; ++i) {
        std::cout << i << " " << m2[i] << std::endl;
	}

    int i = 1;
}