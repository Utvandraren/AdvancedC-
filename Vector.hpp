#pragma once
#include <iostream>
#include <cassert>
#include "Dalloc.hpp"

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
		VectorItt(X* p) noexcept : _ptr(static_cast<T*>(const_cast<T*>(p)))  { }
		VectorItt() noexcept = default;
		VectorItt(const VectorItt& other) noexcept : _ptr(other._ptr) {}
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
		X& operator* () const noexcept { return *_ptr; }
		X* operator-> () const noexcept { return _ptr; }
		X& operator[](size_t i) const noexcept { return _ptr[i]; }
#pragma endregion element access

#pragma region Modifiers    
		VectorItt& operator++ ()noexcept {
			_ptr += DIR;
			return *this;
		}
		VectorItt& operator-- () noexcept {
			_ptr -= DIR;
			return *this;
		}
		VectorItt operator++ (int) noexcept {
			auto temp(*this);
			operator++();
			return temp;
		}
		VectorItt operator-- (int) noexcept {
			auto temp(*this);
			operator--();
			return temp;
		}
		VectorItt& operator +=  (difference_type i) noexcept  {
			_ptr += (DIR * i);
			return *this;
		}
		VectorItt operator+ (difference_type i) const noexcept { return _ptr + (DIR * i); }
		VectorItt operator- (difference_type i) const noexcept { return _ptr - (DIR * i); }
		difference_type operator- (const VectorItt& other)const noexcept { return _ptr - other._ptr; }
#pragma endregion Modifiers

#pragma region nonmembers
		friend auto operator<=>(const VectorItt& lhs, const VectorItt& rhs) noexcept {
			if (lhs._ptr < rhs._ptr)
				return std::strong_ordering::less;
			else if (lhs._ptr > rhs._ptr)
				return std::strong_ordering::greater;

			return std::strong_ordering::equivalent;
		}
		friend bool operator==(const VectorItt& lhs, const VectorItt& rhs) { return (lhs <=> rhs) == 0; }
		friend bool operator!=(const VectorItt& lhs, const VectorItt& rhs) { return (lhs <=> rhs) != 0; }
#pragma endregion nonmembers
	};

public:
	T* _data;
	size_t _size = 0;
	size_t _maxSize = 8;
	Dalloc<T> _dAlloc;

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
	using reverse_iterator = VectorItt<T, -1>;
	using const_reverse_iterator = VectorItt<const T, -1>;
#pragma endregion typedef

#pragma region Constructors && Assignment
	~Vector() noexcept {
		for (size_t i = 0; i < size(); ++i) {
			_data[i].~T();
		}
		_dAlloc.deallocate(_data, capacity());	
	}

	Vector() noexcept { 
		_data = _dAlloc.allocate(_maxSize);        
		CHECK
	}

	template<class Titer>
	Vector(size_t newCapacity, const Titer& begin, const Titer& end) { 
		_size = 0;
		auto rhs_data = begin;
		if (newCapacity == 0) {
			_data = _dAlloc.allocate(2);
			_maxSize = 2;
		}

		else {
			_data = _dAlloc.allocate(newCapacity);
			_maxSize = newCapacity;
		}
		try {
			for (; (rhs_data + _size) != end; ++_size)
			{
				new (_data + _size) T(rhs_data[_size]);
			}
			CHECK
		}
		catch (...) {

			for (int k = 0; k <= _size; ++k) {
				_data[k].~T();
			}
			_dAlloc.deallocate(_data, newCapacity);
			throw;
		}		
	}

	Vector(const Vector& other) : Vector(other.size(), other.begin(), other.end()) {}

	Vector(Vector&& other)  noexcept : Vector() {
		swap(*this, other);
		CHECK
	}

	Vector(const char* other) : Vector(std::strlen(other), other, other + std::strlen(other)) {}

	Vector& operator=(const Vector& other) {   //copy assignment
		if (*this == other)
			return*this;
		//delete[] _data;
		//_data = new T[other.size() * 2];
		_dAlloc.deallocate(_data, capacity());
		_data = _dAlloc.allocate(other.size() * 2);     
		_size = 0;
		_maxSize = other.size() * 2;
		try {
			for (; _size < other.size(); _size++)
			{
				new (_data + _size)T(other[_size]);
			}
		}
		catch (...) {
			for (size_t i = 0; i < size(); ++i) {
				_data[i].~T();
			}
			_dAlloc.deallocate(_data, capacity());
			throw;
		}	
		return *this;
	}
	Vector& operator=(Vector&& other) noexcept { 
		if (*this == other)
			return*this;
		swap(*this, other);
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
	reverse_iterator rbegin() noexcept { return reverse_iterator(_data + _size); }
	reverse_iterator rend() noexcept { return reverse_iterator(_data); }
	const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(_data + _size); }
	const_reverse_iterator rend() const noexcept { return const_reverse_iterator(_data); }
	const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(_data + _size); }
	const_reverse_iterator crend() const noexcept { return const_reverse_iterator(_data); }
#pragma endregion iterators

#pragma region Capacity
	size_t size() const noexcept { return _size; }
	size_t capacity() const noexcept { return _maxSize; }
	void reserve(size_t n) { 
		if (n > capacity())
		{
			T* temp = _dAlloc.allocate(n);   
			int tempSize = 0;
			try {
				for (; tempSize <= size(); tempSize++)
				{
					new (temp + tempSize)T(_data[tempSize]);
				}
			}
			catch (...) {
				for (size_t i = 0; i < tempSize; ++i) {
					temp[i].~T();
				}
				_dAlloc.deallocate(temp, capacity());
				throw;
			}				
			_dAlloc.deallocate(_data, _maxSize);
			_data = temp;
		}
		else if(n <= capacity())
		{
			return;
		}
		_maxSize = n; 
	}
#pragma endregion Capacity

#pragma region Modifiers
	void shrink_to_fit() { 
		if (size() == capacity()) {
			return;
		}
		T* temp = _dAlloc.allocate(_size);
		int tempSize = 0;
		try {
			for (; tempSize <= size(); tempSize++)
			{
				new (temp + tempSize)T(_data[tempSize]);
			}
		}
		catch (...) {
			for (size_t i = 0; i < tempSize; ++i) {
				_data[i].~T();
			}
			_dAlloc.deallocate(_data, capacity());
			throw;
		}	
		_dAlloc.deallocate(_data, _maxSize);
		_maxSize = _size;
		_data = temp;
	}
	void push_back(T c) {	
		if (size() >= capacity()) {
			reserve(capacity() * 2);
		}
		try {
			new (_data + _size)T(c);
		}
		catch (...) {
			_data[_size].~T();
			throw;
		}

		++_size;	
		CHECK
	}
	void resize(size_t n) { 
		if (n >= _size)
		{
			if (capacity() < n) {
				T* temp = _dAlloc.allocate(n);
				int tempSize = 0;
				try {
					for (; tempSize <= _size; tempSize++)
					{
						new (temp + tempSize)T(_data[tempSize]);
					}
					for (; tempSize < n; tempSize++)
					{
						new (temp + tempSize)T();
					}
				}
				catch (...) {
					for (size_t i = 0; i < tempSize; ++i) {
						_data[i].~T();
					}
					_dAlloc.deallocate(_data, capacity());
					throw;
				}
				_dAlloc.deallocate(_data, capacity());
				_data = temp;
				_maxSize = n;

			}
		}	
		reserve(n * 2); 
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

	friend bool operator==(const Vector& lhs, const Vector& rhs) {
		if (lhs.size() != rhs.size()) {
			return false;
		}
		return (lhs <=> rhs) == 0; 
	}   
	friend bool operator!=(const Vector& lhs, const Vector& rhs) {
		if (lhs.size() != rhs.size()) {
			return true;
		} return (lhs <=> rhs) != 0;
	}
#pragma endregion nonmembers

#pragma region TestFunktion
	void Check() const {
		assert(Invariant());
	}

	bool Invariant() const {  
		if (size() > capacity()) {
			return false;
		}
		for (auto p = begin(); p != end(); p++)
		{
			if (p == nullptr)
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
	std::swap(lhs._data, rhs._data);
	std::swap(lhs._size, rhs._size);
	std::swap(lhs._maxSize, rhs._maxSize);
}
