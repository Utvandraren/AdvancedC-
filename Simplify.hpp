#pragma region simplifyTypeAssignment

template<class T> 
struct SimplifyType {
	using type = T;
};

template<class T>
using SimplifyType_t = typename SimplifyType<T>::type;

template<class T>
struct SimplifyType<T*> {
	using type = T;
};

template<class T>
struct SimplifyType<T const> {
	using type = T;
};

template<class T>
struct SimplifyType<T&> {
	using type = T;
};

template<class T>
struct SimplifyType<T[]> {
	using type = T;
};

#pragma endregion

#pragma region BaseTypeAssignment

template<class T>
struct BaseType {
	using type = T;
};

template<class T>
using BaseType_t = typename BaseType<T>::type;

template<class T>
struct BaseType<T*> {
	using type = BaseType_t<T>;
};

template<class T>
struct BaseType<T const> {
	using type = BaseType_t<T>;
};

template<class T>
struct BaseType<T&> {
	using type = BaseType_t<T>;
};

template<class T>
struct BaseType<T[]> {
	using type = BaseType_t<T>;
};

template<class T, size_t  N>
struct BaseType<T[N]> {
	using type = BaseType_t<int>;
};

#pragma endregion

