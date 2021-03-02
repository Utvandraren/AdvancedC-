
template<class T>
struct RAC {
	using type = T;
};

//template<class T>
//using RAC_t = typename RAC<T>::type;

template<class T>
struct RAC<T const> {
	using type = RAC<T>::type;
};

template<class T>
struct RAC<T* const> {
	using type = RAC<T*>::type;
};

template<class T>
struct RAC<T*> {
	using type = RAC<T*>::type;
};

