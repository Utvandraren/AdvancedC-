#pragma once
#include <iostream>
#include <cassert>

#define CHECK assert(Invariant());

template<class T>
class Vector
{
	template<class X, int DIR>
	class VectorItt
	{
		friend class Vector<T>;
		X* _ptr;

	public:

#pragma region types
		using iterator_category = std::random_access_iterator_tag;
		using value_type = T;
		using difference_type = std::ptrdiff_t;
		using reference = X&;
		using pointer = X*;
#pragma endregion types


#pragma region Constructors && Assignment
		~VectorItt() = default;
		VectorItt(X* p) : _ptr(static_cast<T*>(const_cast<T*>(p))) { }
		VectorItt() = default;
		VectorItt(const VectorItt& other) : _ptr(other._ptr) {}
		VectorItt& operator= (const VectorItt& other) {
			_ptr = other._ptr;
			return *this;
		}

		template<class Y, class = std::enable_if_t<!std::is_const_v<Y>>>     //const_iterator(iterator&)
		VectorItt(const VectorItt<Y, DIR>& rhs) : _ptr(rhs._ptr) {}
		template<class Y, class = std::enable_if_t<!std::is_const_v<Y>>>     //const_iterator& operator=(iterator&)
		VectorItt& operator=(const VectorItt<Y, DIR>& rhs) { _ptr = rhs._ptr; }
#pragma endregion Constructors && Assignment

#pragma region element access
		X& operator* () const { return *_ptr; }
		X* operator-> () const { return _ptr; }
		X& operator[](size_t i) { return _ptr[i]; }
#pragma endregion element access


#pragma region Modifiers    
		VectorItt& operator++ () {  //more work to be done  here-----
			/*++_ptr;*/
			_ptr += DIR;
			return *this;
		}
		VectorItt& operator-- () {
			//--_ptr;
			_ptr -= DIR;
			return *this;
		}
		VectorItt operator++ (int) {
			auto temp(*this);
			operator++();
			return temp;
		}
		VectorItt operator-- (int) {
			auto temp(*this);
			operator--();
			return temp;
		}
		VectorItt& operator += (difference_type i) {
			_ptr += (DIR * i);
			return *this;
		}
		VectorItt operator+ (difference_type i) const { return _ptr + (DIR * i); }
		VectorItt operator- (difference_type i) const { return _ptr - (DIR * i); }
		difference_type operator- (const VectorItt& other)const { return _ptr - other._ptr; }
#pragma endregion Modifiers

#pragma region nonmembers
		/*friend bool operator ==(const VectorItt& lhs, const VectorItt& rhs) = default;
		friend bool operator !=(const VectorItt& lhs, const VectorItt& rhs) = default;*/
		friend auto operator<=>(const VectorItt& lhs, const VectorItt& rhs) {
			if (lhs._ptr < rhs._ptr)
				return std::strong_ordering::less;
			else if (lhs._ptr > rhs._ptr)
				return std::strong_ordering::greater;

			return std::strong_ordering::equivalent;
		}

		//friend bool operator ==(const VectorItt& lhs, const VectorItt& rhs) {
		//	return lhs._ptr == rhs._ptr;
		//}
		//friend bool operator !=(const VectorItt& lhs, const VectorItt& rhs) {
		//	return lhs._ptr != rhs._ptr;
		//}



		friend bool operator==(const VectorItt& lhs, const VectorItt& rhs) { return (lhs <=> rhs) == 0; }
		friend bool operator!=(const VectorItt& lhs, const VectorItt& rhs) { return (lhs <=> rhs) != 0; }

#pragma endregion nonmembers

	};

private:

public:
	T* _data;
	size_t _size = 0;
	size_t _maxSize = 8;

#pragma region typedef //only iterators really neccessary!
	using value_type = T;
	using pointer = T*;
	using difference_type = std::ptrdiff_t;
	using reference = T&;
	using const_reference = const T&;
	using pointer = T*;
	using const_pointer = const T*;
	using size_type = size_t;
	typedef const T* const_pointer;
	using iterator = VectorItt<T, 1>;
	using const_iterator = VectorItt<const T, 1>;
	using rev_iterator = VectorItt<T, -1>;
	using const_rev_iterator = VectorItt<const T, -1>;
#pragma endregion typedef

#pragma region Constructors && Assignment
	~Vector() = default;
	Vector() noexcept { //Have to fix?
		_data = new T[0];
		_size = 0;
		CHECK
	}
	//Vector() noexcept = default;

	Vector(const Vector& other) {
		_data = other._data;
		_size = other.size();
		CHECK
	}
	Vector(Vector&& other) noexcept {
		swap(*this, other);
		CHECK
	}
	Vector(const char* other) { //DEBUG    //More effective way to do this?
		_size = 0;
		for (size_t i = 0; other[i] != '\0'; i++)
		{
			++_size;
		}
		_data = new T[_size * 2];
		//reserve(size() * 2);
		for (size_t i = 0; other[i] != '\0'; i++)
		{
			_data[i] = other[i];
		}
		CHECK
	}
	Vector& operator=(const Vector& other) {   //copy assignment
		if (*this == other)
			return*this;
		_data = new T[other.size() * 2];
		for (size_t i = 0; i < other.size(); i++)
		{
			_data[i] = other._data[i];
		}
		_size = other.size();
		_maxSize = other.capacity();
		return *this;
	}
	Vector& operator=(Vector&& other) noexcept {
		if (*this == other)
			return*this;
		this->_data = other._data;
		_size = other.size();
		other._data = new T[0];          //do this some other way?
		other.resize(0);
		return *this;
	}
#pragma endregion Constructors && Assignment

#pragma region element access
	T& operator[](size_t i) { return _data[i]; }
	T& at(size_t i) {
		if (i > size() - 1)
			throw std::out_of_range("WORNG");
		return _data[i];
	}
	const T& operator[](size_t i) const { return _data[i]; }
	const T& at(size_t i) const {
		if (i > size() - 1)
			throw std::out_of_range("WRONG!");
		return _data[i];
	}
	T* data() noexcept { return _data; }
	const T* data()const noexcept { return const_cast<T*>(_data); }
#pragma endregion element access

#pragma region iterators
	iterator begin() noexcept { return iterator(_data); }
	iterator end() noexcept { return iterator(_data + _size); }
	const_iterator begin() const noexcept { return const_iterator(_data); }
	const_iterator end() const noexcept { return const_iterator(_data + _size); }
	const_iterator cbegin() const noexcept { return const_iterator(_data); }
	const_iterator cend() const noexcept { return const_iterator(_data + _size); }

	//alla ovanför med "iterator" bytt mot "reverse_iterator"
#pragma endregion iterators

#pragma region Capacity
	size_t size() const noexcept { return _size; }
	size_t capacity() const noexcept { return _maxSize; }
	void reserve(size_t n) { _maxSize = n; }
#pragma endregion Capacity

#pragma region Modifiers
	void shrink_to_fit() { _maxSize = _size; }
	void push_back(T c) {	//-----------------------Forttsätt här
		_data[size()] = c;
		++_size;
		if (size() > capacity()) {
			resize(capacity());
		}
	}
	void resize(size_t n) {
		if (n > size())
		{
			T* temp = new T[n]{ *_data };
			for (size_t i = 0; i < size(); i++)
			{
				temp[i] = _data[i];
			}
			_data = temp;
			//reserve(n * 2);
		}
		reserve(n * 4);
		_size = n;
	}
#pragma endregion Modifiers

#pragma region nonmembers
	friend auto operator<=>(const Vector& lhs, const Vector& rhs) {
		auto lIt = lhs.begin(); auto rIt = rhs.begin();
		for (; lIt != lhs.end() && rIt != rhs.end(); ++lIt, ++rIt)
			if (*lIt < *rIt)
				return std::strong_ordering::less;
			else if (*lIt > * rIt)
				return std::strong_ordering::greater;
		if (lIt == lhs.end())
			if (rIt == rhs.end())
				return std::strong_ordering::equivalent;
			else
				return std::strong_ordering::less;
		else
			return std::strong_ordering::greater;
	}


	friend bool operator==(const Vector& lhs, const Vector& rhs) { return (lhs <=> rhs) == 0; }
	friend bool operator!=(const Vector& lhs, const Vector& rhs) { return (lhs <=> rhs) != 0; }



#pragma endregion nonmembers

#pragma region TestFunktion
	void Check() const {
		assert(Invariant());
	}

	bool Invariant() const {  // Lägg till check såom kollar att inga nullptr hittas
		if (size() > capacity()) {
			return false;
		}
		return true;
	}


	friend std::ostream& operator<<(std::ostream& cout, const Vector& other) {
		for (size_t i = 0; i < other.size(); ++i)
			cout << other._data[i];
		return cout;
	}
#pragma endregion TestFunktion

};

template<class T>
static void swap(Vector<T>& lhs, Vector<T>& rhs) {
	auto temp = lhs._data;
	lhs._data = rhs._data;
	rhs._data = temp;

	size_t tempsize = lhs.size();
	lhs._size = rhs.size();
	rhs._size = tempsize;
}
