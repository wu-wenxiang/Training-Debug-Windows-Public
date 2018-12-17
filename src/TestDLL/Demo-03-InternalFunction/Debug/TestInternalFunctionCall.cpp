// Demo-03-InternalFunction.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#define UNICODE
#define DBGHELP_TRANSLATE_TCHAR
#include <windows.h>
#include <iostream>
#include <dbghelp.h>
using namespace std;
typedef int(*AddFunc)(int a, int b);

int testFun(int a, int b)
{
	return (a*b);
}

int main()
{
    std::cout << "Hello World!\n"; 
	HMODULE hDll = LoadLibrary(L"TestDLL.dll");
	if (hDll != NULL)
	{
		BOOL bret;
		HANDLE hp = GetCurrentProcess();
		bret = SymInitialize(hp, NULL, true);
		if (bret)
		{
			SymSetOptions(SYMOPT_EXACT_SYMBOLS);
			ULONG64 buffer[(sizeof(SYMBOL_INFO) + MAX_SYM_NAME * sizeof(TCHAR) + sizeof(ULONG64) - 1) / sizeof(ULONG64)];
			PSYMBOL_INFO pSymbol = (PSYMBOL_INFO)buffer;
			// bret = SymFromName(hp, L"TestDLL!Sub", pSymbol);
			// bret = SymFromName(hp, L"TestInternalFunctionCall!testFun", pSymbol);
			bret = SymFromName(hp, L"TestDLL!Add_Internal", pSymbol);
			if (bret)
			{
				AddFunc fnPointer = (AddFunc)pSymbol->Address;
				if (fnPointer)
				{
					int sum = (fnPointer)(3, 5);
					cout << "sum = " << sum << endl;
				}
			}
			else
			{
				DWORD ret = GetLastError();
				cout << "ret = " << ret << endl;
			}
		}

		FreeLibrary(hDll);
	}

	cout << "end" << endl;
	return 0;
}
