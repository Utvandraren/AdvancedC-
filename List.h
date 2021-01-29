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

		Node* Erase() {
			_next->_prev = _prev;
			_prev->_next = _next;
			return static_cast<Node*>(this);
		}

		void popFront() {
			this->_next = this->_next->_next;
			this->_next->_prev = this;
		}

		void popBack() {          //Fix these functions to erase the right wway
			this->_prev = this->_prev->_prev;
			this->_prev->_prev = this;
		}
	};

	class Node : public Link {
		friend class List;
		T _data;
	public:
		Node(const T& data) :_data(data) {};
	};

	template<class X> //X is T or const T
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
		//ListIter(Node<T>* p) { return *this; }  // Kopplat till iterator.begin?	
		ListIter(const Link* node = nullptr) :_ptr(static_cast<Node*>(const_cast<Link*>(node))) {}
		ListIter(const ListIter&) = default;  //varför behöver man inte ha en identivierarer för variabeln i parametern?
		ListIter& operator=(const ListIter&) = default;

#pragma endregion constructors/assignment

#pragma region element access

		T& operator*() { return _ptr->_data; }
		T* operator->() { return &_ptr->_data; }

#pragma endregion element access

#pragma region modifiers

		ListIter& operator++() {
			_ptr = static_cast<Node*>(_ptr->_next);
			return *this;
		}

		ListIter operator++(int) {         //what is int used for? saving how much the variable is boing to be reduced by?
			auto temp(*this);
			operator++();
			return temp;
		}

		ListIter& operator--() {
			_ptr = static_cast<Node>(_ptr.prev);
			return *this;
		}

		ListIter operator--(int) {
			auto temp(*this);
			operator--;
			return temp;
		}

#pragma endregion modifiers


#pragma region nonmembers

		/*friend bool operator==(const ListIter& lhs, const ListIter& rhs) = default;
		friend bool operator!=(const ListIter& lhs, const ListIter& rhs) = default;*/

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
			source = source->_next;
		}
		CHECK
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
	iterator end() { return iterator(_head._prev); }                      
	const_iterator end()const { return const_iterator(_head._prev); }
	const_iterator cend()const { return const_iterator(_head._prev); }

#pragma endregion iterators

#pragma region capacity

	bool empty() const  noexcept { return begin() == end(); }
	size_t Size() const noexcept { return std::distance(cbegin(), cbegin()); }
	size_t Count()  noexcept {
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
		pos._ptr->Insert(new Node(value));
		return pos._ptr->_next;
	}

	iterator erase(const iterator& pos) {
		iterator temp = pos;
		temp._ptr->Erase();
		++temp;
		delete pos._ptr;
		return temp;
	}

	void push_front(const T& toInsert) {
		_head.Insert(new Node(toInsert));
	}

	void push_back(const T& toInsert) {
		_head.InsertBack(new Node(toInsert));	
	}

	void pop_front() {
		_head.popFront();
	}

	void pop_back() {
		_head.popBack();
	}


#pragma endregion modifiers


#pragma region testfunktioner 

	bool Invariant() const {

		for (auto ptr = _head._next; ptr != &_head; ptr = ptr->_next)
		{
			if (ptr != ptr->_next->_prev)
				return false;
		}

		//auto currentNode = &_head;
		//auto nextNode = currentNode->_next;

		//while (currentNode->_next != &_head)
		//{
		//	/*if (currentNode->_next != nextNode)
		//		return false;*/
		//	if (nextNode->_prev != currentNode)
		//		return false;

		//	auto temp = nextNode;
		//	currentNode = nextNode;
		//	nextNode = temp->_next;
		//}

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



};

