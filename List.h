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
			toInsert->_next = *this;	
			_prev = toInsert;
		}

		Node* Erase() {
			_next->_prev = _prev;
			_prev->_next = _next;
			return static_cast<Node*>(this);
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

		//~ListIter();   
		//ListIter(Node(T)* p)) { return *this; };  // Kopplat till iterator.begin?	
		ListIter(const Link* node = nullptr) :_ptr(static_cast<Node*>(const_cast<Link*>(node))) {}
		ListIter(const ListIter& other) = default;
		ListIter& operator=(const ListIter& other) = default;	

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

		friend bool operator==(const ListIter& lhs, const ListIter& rhs) {               //Kan användas istället om problem uppstår?
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
		while (_head._next != nullptr){
			pop_front();
		}
	}
	List() = default;
	List(const List& other) {
		const Link* source = other._head._next;
		Link** dest = &(_head._next);
		while (source != nullptr) {
			Link* node = new Node(static_cast<const Node*>(source)->_data);       //Remeber to clean this up since its stored on stack
			(*dest) = node;
			dest = &(node->_next);
			source = source->_next;
		}
	}
	List(const char* other) { //DEBUGGER TESTER
		for (const char* charPtr = other; *charPtr != '\0'; ++charPtr)
		{
			Node node(*charPtr);
		}
	}

#pragma endregion constructors and assignment

#pragma region element access

	T& front() { return static_cast<Node*>(_head._next)->_data; }
	const T& front() const { return static_cast<Node*>(_head._next)->_data; }

#pragma endregion element access

#pragma region iterators
	
	iterator begin() { return iterator(_head._next); }
	const_iterator begin() const { return const_iterator(_head._next); }
	const_iterator cbegin() const { return const_iterator(_head._next); } //samma, behövs ändras? cbegin ska använda eller är detta som menas noexcept?
	iterator end() { return iterator(nullptr); }
	const_iterator end()const { return const_iterator(nullptr); }
	const_iterator cend()const { return const_iterator(nullptr); }   

#pragma endregion iterators

#pragma region capacity

	bool empty() const  noexcept{ return begin() == end(); }
	size_t Count() const noexcept { return std::distance(cbegin(), cbegin()); } //check the number of hops? vad är hops? hopp i minnet?

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

	//TODO pushback, pushfront, popback,  popfront

	void pop_front(){}
	void pop_back(){}


#pragma endregion modifiers


#pragma region testfunktioner 

	bool Invariant() const { 
		Link* currentNode = &_head;
		Link* nextNode = currentNode->_next;

		while (currentNode->_next != &_head) 
		{
			if (currentNode->_next != nextNode)
				return false;
			if (nextNode->_prev != currentNode)
				return false;

			Link* temp = nextNode;
			//nextNode = currentNode;
			currentNode = temp->_next;
		}
		
		//for (auto p = &_head; p != nullptr; p = p->_next) 
		//{
		//	//Ha två pekare som pekar från till den adre framför den 
		//	//flytta vbbåda fram ett steg eftewr varje loop

		//	
		//}

		return true;


		
	}

#pragma endregion testfunktioner






	//TODO friend assignmenets och jämförelser här 

};

