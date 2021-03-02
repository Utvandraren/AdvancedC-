#include <vector>

template<class CONT>
auto Sum(const CONT& c) {
	typename std::remove_const<std::remove_reference<decltype(*c.begin())>::type>::type sum{};
	//decltype(*std::begin(c)) x = (*std::begin(c));
	
	for (auto i : c)
		sum += i;
	return sum;
}