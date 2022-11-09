#include "list.h"

#include <map>
#include <list>

int factorial(const int i) {
    return i == 0 ? 1 : i * factorial(i - 1);
}

int main(int, char *[]) {

	auto l0 = std::list<int, Allocator<int, 10>>{};
	for ( int i = 0; i < 10; ++i ) {
        l0.push_back(i);
    }

    // auto m1 = std::map<int, int>{};

    // for ( int i = 0; i < 10; ++i ) {
    //     m1[i] = factorial(i);
    // }

    // auto m2 = std::map<
	// 	int,
	// 	int,
	// 	std::less<int>,
	// 	Allocator<
	// 		std::pair<const int, int>, 10>>{};

	// for (int i = 0; i < 10; ++i) {
	// 	m2[i] = factorial(i);
	// }

	// for (int i = 0; i < 10; ++i) {
    //     std::cout << i << " " << m2[i] << std::endl;
	// }

    // List<int> l1;

	// for (int i = 0; i < 10; ++i) {
    //     l1.push_back(i);
	// }

	// for ( auto it = l1.begin(); it != l1.end(); ++it ) {
	// 	std::cout << *it << std::endl;
	// }

	// List<int, Allocator<int, 10>> l2;

	// for (int i = 1; i < 10; ++i) {
    //     l2.push_back(i);
	// }

	// for ( auto it = l2.begin(); it != l2.end(); ++it ) {
	// 	std::cout << *it << std::endl;
	// }
}