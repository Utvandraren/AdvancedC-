

template<int M, int N> //m > 0  n > 0
struct Ackermann {
	static const int value = Ackermann<M - 1, Ackermann<M, N - 1>::value>::value;
};