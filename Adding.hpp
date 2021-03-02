
static int AddFun(int x, int y) {
	if (x == 0)
		return y;
	else
		return AddFun(x - 1, y + 1);
}

template <int X, int Y>
struct Adding {
	static const int value = Adding<X - 1, Y + 1>::value;
};

template<int Y>
struct Adding<0, Y> {
	static const int value = Y;
};
