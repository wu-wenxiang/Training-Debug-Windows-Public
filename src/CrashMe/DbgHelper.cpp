#include "DbgHelper.h"
#include <Windows.h>
#include <errorrep.h>
#include <shellapi.h>
#include <shlobj.h>
#include <strsafe.h>


/*!*************************************************
* The following code was taken from:
* https://winqual.microsoft.com/help/developers_guide_to_wer.htm
*
****************************************************/
static WCHAR szPath[MAX_PATH+1];
static WCHAR szFr[] = L"\\System32\\FaultRep.dll";


WCHAR* GetFullPathToFaultrepDll(void)
{
	TCHAR *lpRet = NULL;
	UINT rc;

	rc = GetSystemWindowsDirectoryW(szPath, ARRAYSIZE(szPath));
	if (rc == 0 || rc > ARRAYSIZE(szPath) - ARRAYSIZE(szFr) - 1)
		return NULL;

	//wcscat_s(szPath, MAX_PATH, szFr);
	wcscat_s(szPath, MAX_PATH, szFr);
	return szPath;
}


LONG WINAPI MyReportFault( struct _EXCEPTION_POINTERS * pExceptionPointers )
{
	LONG lRet = EXCEPTION_CONTINUE_SEARCH;
	TCHAR * psz = GetFullPathToFaultrepDll();
	if ( psz != NULL )
	{
		HMODULE hFaultRepDll = LoadLibrary( psz ) ;
		if ( hFaultRepDll )
		{
			pfn_REPORTFAULT pfn = (pfn_REPORTFAULT)GetProcAddress( hFaultRepDll, "ReportFault" ) ;
			if ( pfn )
			{
				EFaultRepRetVal rc = pfn( pExceptionPointers, 0) ;
				lRet = EXCEPTION_EXECUTE_HANDLER;
			}
			FreeLibrary(hFaultRepDll );
		}
	}
	return lRet ;
}



/*!*************************************************
* The following code was taken from:
* http://msdn2.microsoft.com/en-us/library/bb204861.aspx
*
* .. with a few minor modifications.
****************************************************/
int GenerateDump(EXCEPTION_POINTERS* pExceptionPointers, MINIDUMP_TYPE eDumpType)
{
	BOOL bMiniDumpSuccessful;
	WCHAR szPath[MAX_PATH]; 
	WCHAR szFileName[MAX_PATH]; 
	WCHAR* szAppName = L"CrashMeDumps";
	WCHAR* szVersion = L"v1.0";
	DWORD dwBufferSize = MAX_PATH;
	HANDLE hDumpFile;
	SYSTEMTIME stLocalTime;
	MINIDUMP_EXCEPTION_INFORMATION ExpParam;

	GetLocalTime( &stLocalTime );
	GetModuleFileName( NULL, szPath, dwBufferSize );
	*(wcsrchr(szPath, L'\\')+1) = L'\0';

	StringCchPrintf( szFileName, MAX_PATH, L"%s%s", szPath, szAppName );
	CreateDirectory( szFileName, NULL );

	StringCchPrintf( szFileName, MAX_PATH, L"%s%s\\%s-%04d%02d%02d-%02d%02d%02d-%ld-%ld.dmp", 
		szPath, szAppName, szVersion, 
		stLocalTime.wYear, stLocalTime.wMonth, stLocalTime.wDay, 
		stLocalTime.wHour, stLocalTime.wMinute, stLocalTime.wSecond, 
		GetCurrentProcessId(), GetCurrentThreadId());
	hDumpFile = CreateFile(szFileName, GENERIC_READ|GENERIC_WRITE, 
		FILE_SHARE_WRITE|FILE_SHARE_READ, 0, CREATE_ALWAYS, 0, 0);

	ExpParam.ThreadId = GetCurrentThreadId();
	ExpParam.ExceptionPointers = pExceptionPointers;
	ExpParam.ClientPointers = TRUE;

	bMiniDumpSuccessful = MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), 
		hDumpFile, MiniDumpWithFullMemory , &ExpParam, NULL, NULL);

	CloseHandle( hDumpFile );
	return EXCEPTION_EXECUTE_HANDLER;
}