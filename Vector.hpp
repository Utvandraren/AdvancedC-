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
		VectorItt& operator++ () {  
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
		friend auto operator<=>(const VectorItt& lhs, const VectorItt& rhs) {
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
	~Vector() {	
		for (size_t i = 0; i < size(); ++i) {
			_data[i].~T();
		}
		_dAlloc.deallocate(_data, capacity());
		
	}

	Vector() noexcept { 
		_data = _dAlloc.allocate(_maxSize);         //The right way to do this?
		CHECK
	}

	template<class Titer>
	Vector(size_t newCapacity, Titer begin, Titer end) {

	}

	Vector(const Vector& other) : Vector() {
		for (size_t i = 0; i != other.size(); i++)
		{
			push_back(other[i]);
		}
		CHECK
	}
	Vector(Vector&& other)  noexcept : Vector() {
		swap(*this, other);
		CHECK
	}
	Vector(const char* other) : Vector() { //DEBUG   		
		for (size_t i = 0; other[i] != '\0'; i++)
		{
			push_back(other[i]);
		}
		CHECK
	}
	Vector& operator=(const Vector& other) {   //copy assignment
		if (*this == other)
			return*this;
		//delete[] _data;
		//_data = new T[other.size() * 2];
		_dAlloc.deallocate(_data, capacity());
		_data = _dAlloc.allocate(other.size() * 2);     //problem
		_size = 0;
		_maxSize = other.size() * 2;
		for (size_t i = 0; i < other.size(); i++)
		{
			push_back(other[i]);
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
	reverse_iterator rbegin() noexcept { return reverse_iterator(_data); }
	reverse_iterator rend() noexcept { return reverse_iterator(_data + _size); }
	const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(_data); }
	const_reverse_iterator rend() const noexcept { return const_reverse_iterator(_data + _size); }
	const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(_data); }
	const_reverse_iterator crend() const noexcept { return const_reverse_iterator(_data + _size); }
#pragma endregion iterators

#pragma region Capacity
	size_t size() const noexcept { return _size; }
	size_t capacity() const noexcept { return _maxSize; }
	void reserve(size_t n) { 
		if (n > capacity())
		{
			//T* temp = new T[n];
			T* temp = _dAlloc.allocate(n);
			for (size_t i = 0; i < size(); i++)
			{
				temp[i] = _data[i];
			}
			/*for (size_t i = size(); i < n; i++)
			{
				temp[i] = T();
			}*/
			//delete[] _data; 
			_dAlloc.deallocate(_data, capacity());
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
		_maxSize = _size;
		//T* temp = new T[_size];
		T* temp = _dAlloc.allocate(_maxSize);
		for (size_t i = 0; i < capacity(); i++)
		{
			temp[i] = _data[i];
		}
		//delete[] _data;
		_dAlloc.deallocate(_data, capacity());
		_data = temp;
	}
	void push_back(T c) {	
		if (size() >= capacity()) {
			reserve(capacity() * 2);
		}
		_data[size()] = c;
		++_size;	
		CHECK
	}
	void resize(size_t n) { //fixa rezise allokaera bara om cap är mindre än N *Done
		if (n >= size())
		{
			if (capacity() < n) {
				//T* temp = new T[n];
				T* temp = _dAlloc.allocate(n);
				for (size_t i = 0; i < size(); i++)
				{
					temp[i] = _data[i];
				}
				//delete[] _data;
				_dAlloc.deallocate(_data, capacity());
				_data = temp;
			}
			for (size_t i = size(); i < n; i++)
			{
				_data[i] = T();
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

	bool Invariant() const {  // Lägg till check såom kollar att inga nullptr hittas kollar att om data är nullåtr ska vap vara 0
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
	std::swap(lhs._data, rhs._data);
	std::swap(lhs._size, rhs._size);
	std::swap(lhs._maxSize, rhs._maxSize);
}
