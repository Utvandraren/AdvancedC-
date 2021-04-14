// AdvancedC++.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#ifndef DBG_NEW
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#endif
// _DEBUG

#include <cassert>

#include <iostream>
#include "SharedPtr.hpp"

int main()
{
#ifdef DBG_NEW
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	assert(!testPtr);
	SharedPtr<std::string> testPtr = SharedPtr<std::string>();
	assert(testPtr);
	new int;


	std::cout << "Code finished!" << std::endl;

}

