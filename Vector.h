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
		const_iterator(iterator&);
		const_iterator& operator=(iterator&);
#pragma endregion Constructors && Assignment

#pragma region element access
		T& operator* ();
		T * operator-> (); 
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
		friend bool operator ==(const VectorItt& lhs,const VectorItt& rhs);
		friend bool operator !=(const VectorItt& lhs, const VectorItt& rhs);

		//Add all the other comparison operators here
#pragma endregion nonmembers

	};

public:

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
	using iterator = FlistIter<T>;
	using const_iterator = FlistIter<const T>;
#pragma endregion typedef

#pragma region Constructors && Assignment
	~Vector() = default;
	Vector() = default noexcept;
	Vector(const Vector& other) {

	}
	Vector(Vector&& other) noexcept {

	}
	Vector(const char* other) { //DEBUG

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
	iterator begin() noexcept;
	iterator end() noexcept;
	const_iterator begin() const noexcept;
	const_iterator end() const noexcept;
	const_iterator cbegin() const noexcept;
	const_iterator cend() const noexcept;
	//alla ovanför med "iterator" bytt mot "reverse_iterator"
#pragma endregion iterators

#pragma region Capacity
	size_t size() const noexcept;
	void reserve(size_t n);
	size_t capacity() const noexcept;
#pragma endregion Capacity

#pragma region Modifiers
	void shrink_to_fit();
	void push_back(T c);
	void resize(size_t n);
#pragma endregion Modifiers

#pragma region nonmembers
	friend bool operator==(const Vector& lhs, const Vector& other);
	// implement all other comparisonoperators
#pragma endregion nonmembers

#pragma region TestFunktion
	void Check() const {
		assert(Invariant());
	}

	bool Invariant() const {
		//If the list is circular answers false, otherwis true
		size_t i = 0;
		for (auto p = &_head; p != nullptr; p = p->_next)
			if (++i == std::numeric_limits<size_t>::max())
				return false;
		return true;
	}

	friend std::ostream& operator<< (std::ostream& cout
		, const Vector& other) {
		for (size_t i = 0; i < other.size(); ++i)
			cout << other[i];
		return cout;
	}
#pragma endregion TestFunktion

	template< class T>
	void swap(Vector<T>& lhs, Vector<T>& rhs);

};
