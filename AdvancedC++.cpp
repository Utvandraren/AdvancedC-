// AdvancedC++.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#ifdef _DEBUG
#ifndef DBG_NEW
#include <stdlib.h>
#include <crtdbg.h>
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#endif
#endif  // _DEBUG

#include <iostream>
#include "List.h" 

using std::cout;

#include "TestLevel.h"

void TestBasic();
void TestList();
void TestListIter();


int main()
{
#ifdef DBG_NEW
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
   
    //template class List<char>;  //Hur gjorde man detta testet?
    //The test get stuck somewhere
    TestBasic();
    /*TestList();
    TestListIter();*/
    std::cout << "There should be one memory leak!";
    std::cin.get();

}

