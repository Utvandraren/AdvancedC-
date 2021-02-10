#pragma once
#include <iostream>
#include <cassert>

#define CHECK assert(Invariant());

template<class T>
class Vector
{
	class Proxy
	{
		char* _ptr;
	};

	template<class X>
	class VectorItt
	{
		friend class Vector<T>;
		T* _ptr;

	public:

#pragma region types
		using iterator_category = std::forward_iterator_tag;
		using value_type = T;
		using difference_type = std::ptrdiff_t;
		using reference = X&;
		using pointer = X*;
#pragma endregion types


#pragma region Constructors && Assignment
		~VectorItt() = default;
		VectorItt(X* p) : _ptr(static_cast<T*>(const_cast<T*>(p))) { }
		VectorItt() = default;
		VectorItt(const VectorItt& other) : _ptr(other._ptr){}
		VectorItt& operator= (const VectorItt& other) { _ptr = other._ptr; }

		template<class Y, class = std::enable_if_t<!std::is_const_v<Y>>>     //const_iterator(iterator&)
		VectorItt(const VectorItt<Y>& rhs) : _ptr(rhs._ptr) {}
		template<class Y, class = std::enable_if_t<!std::is_const_v<Y>>>     //const_iterator& operator=(iterator&)
		VectorItt& operator=(const VectorItt<Y>& rhs) { _ptr = rhs._ptr; }
#pragma endregion Constructors && Assignment

#pragma region element access
		X& operator* () { return *_ptr; }
		X* operator-> () { return &_ptr; }
		X& operator[](size_t i) { return _ptr[i]; }
#pragma endregion element access


#pragma region Modifiers    
		VectorItt& operator++ () {  //more work to be done  here-----
			++_ptr;
			return *this;
		}
		VectorItt& operator-- () {
			--_ptr;
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
		VectorItt operator+ (difference_type i) const;
		VectorItt operator- (difference_type i) const;
		difference_type operator- (const VectorItt& other) const;
#pragma endregion Modifiers

#pragma region nonmembers
		/*friend bool operator ==(const VectorItt& lhs, const VectorItt& rhs) = default;
		friend bool operator !=(const VectorItt& lhs, const VectorItt& rhs) = default;*/

		friend bool operator ==(const VectorItt& lhs, const VectorItt& rhs){
			return lhs._ptr == rhs._ptr;
		}
		friend bool operator !=(const VectorItt& lhs, const VectorItt& rhs){
			return lhs._ptr != rhs._ptr;
		}
		//Add all the other comparison operators here
#pragma endregion nonmembers

	};

private:
	size_t _size = 0;
	size_t _maxSize = 8;

public:
	T* _data;

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
	using iterator = VectorItt<T>;
	using const_iterator = VectorItt<const T>;
#pragma endregion typedef

#pragma region Constructors && Assignment
	~Vector() = default;
	Vector() noexcept {
		_data[4];
		CHECK
	}
	Vector(const Vector& other) {
		_data = other._data;
		_size = other.size();
	}
	Vector(Vector&& other) noexcept {
		
	}
	Vector(const char* other) { //DEBUG    //More effective way to do this?
		for (size_t i = 0; other[i] != '\0'; i++)
		{
			++_size;
		}
		_data = new T[_size * 2];
		_maxSize = _size * 2;
		for (size_t i = 0; other[i] != '\0'; i++)
		{
			_data[i] = other[i];
		}

	}
	Vector& operator=(const Vector& other) {
		_data = other._data;
		_size = other.size();
		return *this;
		
	}
	Vector& operator=(Vector&& other) noexcept {

	}
#pragma endregion Constructors && Assignment

#pragma region element access
	T& operator[](size_t i) { return _data[i]; }
	T& at(size_t i) { return _data[i]; }
	const T& operator[](size_t i) const { return _data[i]; }
	const T& at(size_t i) const { return _data[i]; }
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
	void reserve(size_t n) { _maxSize = n; } 
	size_t capacity() const noexcept { return _maxSize; }
#pragma endregion Capacity

#pragma region Modifiers
	void shrink_to_fit() {
		//TODO----------------------
	}
	void push_back(T c) {
		if (size() == capacity()) {
			resize(capacity() * 2);
		}
		_data[size()] = c;
		++_size;

	}
	void resize(size_t n) {
		T* temp = new T[n];
		for (size_t i = 0; i < size(); i++)
		{
			temp[i] = _data[i];
		}
		_size = n;
		reserve(n);
		_data = temp;
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

	template<class T>
	friend void swap(Vector<T>& lhs, Vector<T>& rhs) {}
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
