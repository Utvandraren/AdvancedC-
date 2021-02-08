#pragma once
#include <iostream>
#include <cassert>

#define CHECK assert(Invariant());

template<class T>
class Vector
{
	template<class X>
	class VectorItt
	{
	private:

	public:
#pragma region types
		using iterator_category = std::forward_iterator_tag;
		using value_type = T;
		using difference_type = std::ptrdiff_t;
		using reference = X&;
		using pointer = X*;
#pragma endregion types


#pragma region Constructors && Assignment
		VectorItt(T* p) {};
		VectorItt();
		VectorItt(const VectorItt& other);
		VectorItt& operator= (const VectorItt& other);
		/*const_iterator(iterator&);
		const_iterator& operator=(iterator&);*/
#pragma endregion Constructors && Assignment

#pragma region element access
		T& operator* ();
		T* operator-> ();
#pragma endregion element access


#pragma region Modifiers
		VectorItt& operator++ ();
		VectorItt& operator-- ();
		VectorItt operator++ (int);
		VectorItt operator-- (int);
		VectorItt operator+ (difference_type i) const;
		VectorItt operator- (difference_type i) const;
		difference_type operator- (const VectorItt& other) const;
#pragma endregion Modifiers

#pragma region nonmembers
		friend bool operator ==(const VectorItt& lhs, const VectorItt& rhs);
		friend bool operator !=(const VectorItt& lhs, const VectorItt& rhs);

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
	//using iterator = FlistIter<T>;
	////using const_iterator = FlistIter<const T>;
#pragma endregion typedef

#pragma region Constructors && Assignment
	~Vector() = default;
	Vector() noexcept {
		_data[4];
		CHECK
	}
	Vector(const Vector& other) {

	}
	Vector(Vector&& other) noexcept {

	}
	Vector(const char* other) { //DEBUG    //More effective way to do this?
		for (size_t i = 0; other[i] != '\0'; i++)
		{
			++_size;
		}
		_data = new T[_size*2];
		_maxSize = _size * 2;
		for (size_t i = 0; other[i] != '\0'; i++)
		{
			_data[i] = other[i];
		}

	}
	Vector& operator=(const Vector& other) {

	}
	Vector& operator=(Vector&& other) noexcept {

	}
#pragma endregion Constructors && Assignment

#pragma region element access
	T& operator[](size_t i);
	T& at(size_t i);
	const T& operator[](size_t i) const;
	const T& at(size_t i) const;
#pragma endregion element access

#pragma region iterators
	//iterator begin() noexcept;
	//iterator end() noexcept;
	//const_iterator begin() const noexcept;
	//const_iterator end() const noexcept;
	//const_iterator cbegin() const noexcept;
	//const_iterator cend() const noexcept;
	//alla ovanför med "iterator" bytt mot "reverse_iterator"
#pragma endregion iterators

#pragma region Capacity
	size_t size() const noexcept { return _size; }
	void reserve(size_t n) { _maxSize = n; }
	size_t capacity() const noexcept { return _maxSize; }
#pragma endregion Capacity

#pragma region Modifiers
	void shrink_to_fit();
	void push_back(T c);
	void resize(size_t n);
#pragma endregion Modifiers

#pragma region nonmembers
	friend bool operator==(const Vector& lhs, const Vector& other) {
		for (size_t i = 0; i < lhs.size(); i++)
		{
			if (lhs._data[i] != other._data[i]) {
				return false;
			}
		}
		return true;
	}
	// implement all other comparisonoperators
#pragma endregion nonmembers

#pragma region TestFunktion
	void Check() const {
		assert(Invariant());
	}

	bool Invariant() const {
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

	template< class T>
	void swap(Vector<T>& lhs, Vector<T>& rhs);

};
