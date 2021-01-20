#pragma once
#include <iostream>


template<class T>
class List
{
	class Node;

	class Link {
		friend class List<T>;
		Link* _next, * _prev;
		Link() :_next(this), _prev(this) {}

		//Lägg in funktioner för Insert och Erase här, enklast att hantera i Link samt splice för VG
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
		Link* _ptr;

	public:

		//Och här kommer alla funktioner i ListIter
	};

public:
	List() = default;
	List(const char* other) {
		for (const char* charPtr = other; *charPtr != '\0'; ++charPtr)
		{			
			Node node(*charPtr);
		}
	}
private:
	Link _head;

	//Och här kommer alla funktioner i List
};

