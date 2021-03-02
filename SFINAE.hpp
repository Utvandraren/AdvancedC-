#include <iostream>

template<typename T, class = std::enable_if_t<std::is_nothrow_copy_constructible<T>::value>>
bool NoThrowCopyConstructible(T value){
	return true;
}

template<typename T, class = std::enable_if_t<!std::is_nothrow_copy_constructible<T>::value>>
bool NoThrowCopyConstructible(T& value){
	return false;
}

