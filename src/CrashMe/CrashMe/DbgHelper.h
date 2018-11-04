#pragma once
#include <Windows.h>
#include <dbghelp.h>


int GenerateDump(EXCEPTION_POINTERS* pExceptionPointers, MINIDUMP_TYPE eDumpType);
LONG WINAPI MyReportFault( struct _EXCEPTION_POINTERS * pExceptionPointers );


