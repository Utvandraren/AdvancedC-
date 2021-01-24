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

		//Lägg in funktioner för Insert och Erase här, enklast att hantera i Link samt splice för VG

		void Insert(Node* toInsert) {
			toInsert->_prev = _prev;
			toInsert->_next = this;
			_prev = toInsert;
		}

		void InsertBack(Node* toInsert) {
			Link* tempPrev = _head->_prev;
			_head->_prev = toInsert;
			toInsert->_prev = tempPrev;
			toInsert->_next = _head;

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

		void popBack() {
			this->prev = this->_prev->_prev;
			this->prev->_prev = this;
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
		Link** dest = &(_head._next);
		while (source != nullptr) {
			Link* node = new Node(static_cast<const Node*>(source)->_data);       //Remember to clean this up since its stored on stack
			(*dest) = node;
			dest = &(node->_next);
			source = source->_next;
		}
		CHECK
	}
	//List(const char* other) : List() { //DEBUGGER TESTER
	//	for (const char* charPtr = other; *charPtr != '\0'; ++charPtr)
	//	{
	//		Node node(*charPtr);
	//	}
	//	CHECK
	//}

	List(const char* str) :List() { //debug
		const char* ptr = str;
		while (*ptr != '\0')
			++ptr;
		for (; ptr >= str; --ptr)
			push_front(*ptr);
		CHECK
	}

#pragma endregion constructors and assignment

#pragma region element access

	T& front() { return static_cast<Node*>(_head._next)->_data; }
	const T& front() const { return static_cast<Node*>(_head._next)->_data; }

#pragma endregion element access

#pragma region iterators

	iterator begin() { return iterator(_head._next); }
	const_iterator begin() const { return const_iterator(_head._next); }
	const_iterator cbegin() const { return const_iterator(_head._next); }
	iterator end() { return iterator(nullptr); }
	const_iterator end()const { return const_iterator(nullptr); }
	const_iterator cend()const { return const_iterator(nullptr); }

#pragma endregion iterators

#pragma region capacity

	bool empty() const  noexcept { return begin() == end(); }
	size_t Size() const noexcept { return std::distance(cbegin(), cbegin()); }
	size_t Count()  noexcept {

		int j = 0;
		for (iterator i = begin(); i != end(); ++i)      //---------------------Fix here 
		{
			++j;
		}

		return j;
		/*const auto* currentNode = &_head;
		const auto* nextNode = currentNode->_next;
		int i = 0;

		while (currentNode->_next != &_head)
		{
			++i;
			const auto* temp = nextNode;
			currentNode = nextNode;
			nextNode = temp->_next;
		}

		return i;*/
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
		const auto* currentNode = &_head;
		const auto* nextNode = currentNode->_next;

		while (currentNode->_next != &_head)
		{
			if (currentNode->_next != nextNode)
				return false;
			if (nextNode->_prev != currentNode)
				return false;

			const auto* temp = nextNode;
			currentNode = nextNode;         //Something wrong here?
			nextNode = temp->_next;
		}

		//for (auto p = &_head; p != nullptr; p = p->_next) 
		//{
		//	//Ha två pekare som pekar från till den adre framför den 
		//	//flytta vbbåda fram ett steg eftewr varje loop

		//	
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

	void Print(std::ostream& cout) {
		cout << *this;
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
	friend bool operator<(const List& lhs, const List& rhs) {
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


	//TODO friend assignmenets och jämförelser här 

};

