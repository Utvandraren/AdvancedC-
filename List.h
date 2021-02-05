#pragma once
#include <iostream>
#include <cassert>
#define CHECK assert(Invariant());


template<class T>
class List
{
	class Node;

	class Link {
		friend class List<T>;
		Link* _next, * _prev;
		Link() :_next(this), _prev(this) {}
		Link& operator =(const Link&) = delete;      
		Link(const Link&) = delete;

		void Insert(Node* toInsert) {
			toInsert->_next = _next;
			toInsert->_prev = this;
			_next->_prev = toInsert;
			_next = toInsert;
		}

		void InsertBack(Node* toInsert) {
			toInsert->_prev = _prev;
			toInsert->_next = this;
			_prev->_next = toInsert;
			_prev = toInsert;
		}

		Node* EraseNextNode() {
			Link* toDelete = this->_next;
			_next = _next->_next;
			_next->_prev = this;
			return static_cast<Node*>(toDelete);
		}

		void Splice(Link* beforeFirst, Link* atLast) {
			if (beforeFirst == atLast)
				return; 

			Link* beforeFirstafterSplice = beforeFirst->_prev;
			Link* atLastaftesplice = atLast->_next;

			beforeFirst->_prev = _prev;
			_prev->_next = beforeFirst;
			atLast->_prev->_next = this;
			_prev = atLast->_prev;

			beforeFirstafterSplice->_next = atLast;
			atLast->_prev = beforeFirstafterSplice;		 
		}
	};

	class Node : public Link {
		friend class List;
		T _data;
	public:
		Node(const T& data) :_data(data) {};
	};

	template<class X> 
	class ListIter {
		friend class List;

		Node* _ptr;

	public:

#pragma region types
		using iterator_category = std::forward_iterator_tag;
		using value_type = T;
		using difference_type = std::ptrdiff_t;
		using reference = X&;
		using pointer = X*;
#pragma endregion types

#pragma region constructors/assignment

		~ListIter() = default;
		ListIter(const Link* node = nullptr) :_ptr(static_cast<Node*>(const_cast<Link*>(node))) {}
		ListIter(const ListIter&) = default;
		ListIter& operator=(const ListIter&) = default;

		//Detta kommer att tas upp senare på kursen
		template<class Y, class = std::enable_if_t<!std::is_const_v<Y>>>
		ListIter(const ListIter<Y>& rhs) : _ptr(rhs._ptr) {}
		template<class Y, class = std::enable_if_t<!std::is_const_v<Y>>>
		ListIter& operator=(const ListIter<Y>& rhs) {
			_ptr = rhs._ptr;
		}

#pragma endregion constructors/assignment

#pragma region element access

		X& operator*() { return _ptr->_data; }
		X* operator->() { return &_ptr->_data; }

#pragma endregion element access

#pragma region modifiers

		ListIter& operator++() {
			_ptr = static_cast<Node*>(_ptr->_next);
			return *this;
		}

		ListIter operator++(int) {
			auto temp(*this);
			operator++();
			return temp;
		}

		ListIter& operator--() {
			_ptr = static_cast<Node*>(_ptr->_prev);
			return *this;
		}

		ListIter operator--(int) {
			auto temp(*this);
			operator--();
			return temp;
		}

#pragma endregion modifiers


#pragma region nonmembers

		friend bool operator==(const ListIter& lhs, const ListIter& rhs) {
			return lhs._ptr == rhs._ptr;
		}
		friend bool operator!=(const ListIter& lhs, const ListIter& rhs) {
			return lhs._ptr != rhs._ptr;
		}

#pragma endregion nonmembers


	};

private:
	Link _head;

public:
#pragma region typeDef //apparently only iterators neccessary

	using iterator = ListIter<T>;
	using const_iterator = ListIter<const T>;

#pragma endregion typeDef

#pragma region constructors and assignment

	~List() {
		while (_head._next != &_head) {
			pop_front();
		}
		CHECK
	}

	List() = default;

	List(const List& other) :List() {
		const Link* source = other._head._next;
		while (source != &other._head) {
			push_back(static_cast<const Node*>(source)->_data);
			source = source->_next; // sy på huvudet i slutet istället för hela tiden
		}
		CHECK
	}

	List& operator=(const List& other) {  //fel blir en recursiv function i coombination med swap()
		List temp(other);
		temp.swap(*this);
		return *this;
	}

	List(const char* str) :List() { //debug 
		for (const char* ptr = str; *ptr != '\0'; ptr++)
		{
			push_back(*ptr);
		}
		CHECK
	}

#pragma endregion constructors and assignment

#pragma region element access

	T& front() { return static_cast<Node*>(_head._next)->_data; }
	const T& front() const { return static_cast<Node*>(_head._next)->_data; }

	T& back() { return static_cast<Node*>(_head._prev)->_data; }
	const T& back() const { return static_cast<Node*>(_head._prev)->_data; }

#pragma endregion element access

#pragma region iterators

	iterator begin() { return iterator(_head._next); }
	const_iterator begin() const { return const_iterator(_head._next); }
	const_iterator cbegin() const { return const_iterator(_head._next); }
	iterator end() { return iterator(&_head); }
	const_iterator end()const { return const_iterator(&_head); }
	const_iterator cend()const { return const_iterator(&_head); }

#pragma endregion iterators


#pragma region capacity

	bool empty() const  noexcept { return begin() == end(); }
	//size_t Size() const noexcept { return std::distance(cbegin(), cbegin()); }
	size_t Count() const noexcept {
		int j = 0;
		for (auto ptr = _head._next; ptr != &_head; ptr = ptr->_next)
		{
			++j;
		}
		return j;
	}

#pragma endregion capacity


#pragma region modifiers

	iterator insert(const iterator& pos, const T& value) {
		pos._ptr->InsertBack(new Node(value));
		return pos._ptr->_prev;
	}

	iterator erase(const iterator& pos) {
		iterator temp = pos;
		--temp;
		delete (temp._ptr->EraseNextNode());
		++temp;
		return temp;
	}

	void push_front(const T& toInsert) { _head.Insert(new Node(toInsert)); }
	void push_back(const T& toInsert) { _head.InsertBack(new Node(toInsert)); }
	void pop_front() { delete(_head.EraseNextNode()); }
	void pop_back() { delete(_head._prev->_prev->EraseNextNode()); }

	void swap(List<T>& rhs) {

		Link* tempNext = _head._next;
		Link* tempPrev = _head._prev;

		if (_head._next == &_head && rhs._head._next == &rhs._head)
			return;

		if (_head._next == &_head) {
			_head._next = rhs._head._next;
			_head._prev = rhs._head._prev;
			_head._next->_prev = &_head;
			_head._prev->_next = &_head;

			rhs._head._next = &rhs._head;
			rhs._head._prev = &rhs._head;

			return;
		}
		if (rhs._head._next == &rhs._head) {
			rhs._head._next = tempNext;
			rhs._head._prev = tempPrev;
			tempNext->_prev = &rhs._head;
			tempPrev->_next = &rhs._head;

			_head._next = &_head;
			_head._prev = &_head;
			return;
		}

		//lhs
		_head._next = rhs._head._next;
		_head._prev = rhs._head._prev;
		_head._next->_prev = &_head;
		_head._prev->_next = &_head;

		//rhs
		rhs._head._next = tempNext;
		rhs._head._prev = tempPrev;
		tempNext->_prev = &rhs._head;
		tempPrev->_next = &rhs._head;

		CHECK
	}

	friend void swap(List<T>& lhs, List<T>& rhs) { lhs.swap(rhs); }
	
	void splice(const_iterator pos, List<T>& other, const_iterator first, const_iterator last) { pos._ptr->Splice(first._ptr, last._ptr); }

#pragma endregion modifiers


#pragma region testfunktioner 

	bool Invariant() const {
		for (auto ptr = _head._next; ptr != &_head; ptr = ptr->_next)
		{
			if (ptr != ptr->_next->_prev)
				return false;
		}
		return true;
	}

	friend static
		std::ostream& operator<<(std::ostream& cout, const List& l) {
		for (auto it = l.begin(); it != l.end(); ++it)
		{
			cout << *it << " ";
		}
		cout << std::endl;
		return cout;
	}

	std::ostream& Print(std::ostream& cout) {
		for (auto ptr = _head._next; ptr != &_head; ptr = ptr->_next)
			cout << static_cast<Node*>(ptr)->_data << " ";
		cout << std::endl;
		return cout;
	}

#pragma endregion testfunktioner

#pragma region Comparisons

	friend bool operator==(const List& lhs, const List& rhs) {
		auto lIt = lhs.begin(); auto rIt = rhs.begin();
		for (; lIt != lhs.end() && rIt != rhs.end(); ++lIt, ++rIt)
			if (*lIt != *rIt)
				return false;
		return (lIt == lhs.end()) && (rIt == rhs.end());  //both ended => equal
	}
	friend bool operator!=(const List& lhs, const List& rhs) {
		return !(lhs == rhs);
	}
	friend bool operator<(const List& lhs, const List& rhs) {          //Wrong? might have to be checked later
		auto lIt = lhs.begin(); auto rIt = rhs.begin();
		for (; lIt != lhs.end() && rIt != rhs.end(); ++lIt, ++rIt)
			if (*lIt < *rIt)
				return true;
			else if (*lIt > * rIt)
				return false;
		return (rIt != rhs.end());  //if lhs shorter it is less
	}
	friend bool operator>(const List& lhs, const List& rhs) {
		return (rhs < lhs);
	}
	friend bool operator<=(const List& lhs, const List& rhs) {
		return !(rhs < lhs);
	}
	friend bool operator>=(const List& lhs, const List& rhs) {
		return !(lhs < rhs);
	}

#pragma endregion Comparisons


};

