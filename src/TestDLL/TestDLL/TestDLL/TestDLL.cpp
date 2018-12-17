// TestDLL.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"

#include "TestDLL.h"

int Add_Internal(int a, int b)
{
	return (a + b);
}

int Add(int a, int b)
{
	int ret;
	ret = Add_Internal(a, b);
	return ret;
}

int Sub(int a, int b)
{
	return (a-b);
}
