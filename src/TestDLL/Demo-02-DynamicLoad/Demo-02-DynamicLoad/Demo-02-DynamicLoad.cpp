// Demo-02-DynamicLoad.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <windows.h>
#include <iostream>
using namespace std;
typedef int(*AddFunc)(int a, int b);

int main()
{
    std::cout << "Hello World!\n"; 
	// HMODULE hDll = LoadLibrary(L"vcruntime140.dll");
	HMODULE hDll = LoadLibrary(L"TestDLL.dll");
	if (hDll != NULL)
	{
		// AddFunc add = (AddFunc)GetProcAddress(hDll, "wcsstr");
		// AddFunc add = (AddFunc)GetProcAddress(hDll, "printf");
		AddFunc add = (AddFunc)GetProcAddress(hDll, "Add");
		if (add != NULL)
		{
			cout << "Found!" << endl;
			cout << add(2, 3) << endl;
		}
		else
		{
			cout << "Not found!" << endl;
		}
		FreeLibrary(hDll);
	}
	else 
	{
		cout << "Load failed!" << endl;
	}
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
