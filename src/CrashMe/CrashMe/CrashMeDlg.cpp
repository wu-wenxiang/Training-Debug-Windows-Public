////////////////////////////////////////////////////////////////////////
// Module name: CrashMeDlg.cpp
//
// Implements some crash and debug scenarios. 
// Use the code on your own risk.
//
// Notice: If this code works as expected, it was written 
//         by Robert Kuster. Else, I don't know who wrote it. :)
//
////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CrashMe.h"
#include "CrashMeDlg.h"
#include "DbgHelper.h"
#include ".\crashmedlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



#define OneKilo	0x400
#define OnePage 0x1000
#define OneMega	0x100000

// CCrashMeDlg dialog


CCrashMeDlg::CCrashMeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCrashMeDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_nn = 0;
	m_hHeap = NULL;
	m_pMemOnHeap = NULL;
	m_pByteBuffer = NULL;
	m_hThread = NULL;
	m_bDebugBreak = 0;
}


BOOL CCrashMeDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	return TRUE;  // return TRUE  unless you set the focus to a control
}




BEGIN_MESSAGE_MAP(CCrashMeDlg, CDialog)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_BREAKPOINT, &CCrashMeDlg::OnBnClicked_ExecuteBreakPoint)
	ON_BN_CLICKED(IDC_BTN_INT3, &CCrashMeDlg::OnBnClicked_ExecuteInt3)	
	ON_BN_CLICKED(IDC_BTN_RAISE_EXCPT, &CCrashMeDlg::OnBnClicked_RaiseExcpt)
	ON_BN_CLICKED(IDC_BTN_THROW_CPP, &CCrashMeDlg::OnBnClicked_ThrowCppExcpt)	
	ON_BN_CLICKED(IDC_BTN_STACK_OVERFLOW, &CCrashMeDlg::OnBnClicked_StackOverflow)	
	ON_BN_CLICKED(IDC_BTN_DIV_BY_NULL, &CCrashMeDlg::OnBnClicked_DivisionByNull)
	ON_BN_CLICKED(IDC_BTN_STACK_OVERFLOW_RECURSION, &CCrashMeDlg::OnBnClicked_StackOverflowRecursion)	
	ON_BN_CLICKED(IDC_BTN_NESTED_EXCEPTIONS, &CCrashMeDlg::OnBnClickedBtnNestedExceptions)	
	
	ON_BN_CLICKED(IDC_BTN_HEAPALLOC, &CCrashMeDlg::OnBnClicked_HeapAlloc)
	ON_BN_CLICKED(IDC_BTN_HEAPDEALLOC, &CCrashMeDlg::OnBnClicked_HeapDelloc)
	ON_BN_CLICKED(IDC_BTN_OPERATOR_NEW, &CCrashMeDlg::OnBnClicked_OperatorNew)
	ON_BN_CLICKED(IDC_BTN_OPERATOR_DELETE, &CCrashMeDlg::OnBnClicked_OperatorDelete)
	ON_BN_CLICKED(IDC_BTN_HEAP_CREATE, &CCrashMeDlg::OnBnClicked_HeapCreate)
	ON_BN_CLICKED(IDC_BTN_HEAP_DESTROY, &CCrashMeDlg::OnBnClicked_HeapDestroy)
	ON_BN_CLICKED(IDC_BTN_MEMORY_LEAK, &CCrashMeDlg::OnBnClicked_MemoryLeak)

	ON_BN_CLICKED(IDC_BTN_LOAD_DLL, &CCrashMeDlg::OnBnClicked_LoadDll)
	ON_BN_CLICKED(IDC_BTN_UNLOAD_DLL, &CCrashMeDlg::OnBnClicked_UnloadDll)
	ON_BN_CLICKED(IDC_BTN_CREATE_THREAD, OnBnClicked_CreateThread)
	ON_BN_CLICKED(IDC_BTN_END_THREAD, OnBnClicked_EndThread)

	ON_BN_CLICKED(IDC_BTN_ACCESS_TEST_VARIABLE, &CCrashMeDlg::OnBnClicked_AccessTestVariable)
	ON_BN_CLICKED(IDC_BTN_CHECK_FOR_DEBUGGER, &CCrashMeDlg::OnBnClicked_CheckForDebugger)
	ON_BN_CLICKED(IDC_BTN_ENTER_CS, OnBnClicked_EnterCs)
	ON_BN_CLICKED(IDC_BTN_CALLING_CONVENTION, &CCrashMeDlg::OnBnClicked_CallingConvention)
	ON_BN_CLICKED(IDC_BTN_INVALID_HANDLE, OnBnClicked_InvalidHandle)	
	ON_BN_CLICKED(IDC_BTN_SETLAST_ERROR, &CCrashMeDlg::OnBnClicked_SetlastError)

	ON_BN_CLICKED(IDC_BTN_TEST_ARRAYS, &CCrashMeDlg::OnBnClicked_TestArrays)	

	ON_BN_CLICKED(IDC_BTN_DUMP_ME_ON_EXCEPTION, &CCrashMeDlg::OnBnClicked_DumpMeOnException)	
	ON_BN_CLICKED(IDC_BTN_REPORT_FAULT, &CCrashMeDlg::OnBnClicked_WER_ReportFault)
	ON_BN_CLICKED(IDC_CHK_OVERRIDE_UHANDLED_EXCP_FILTER, &CCrashMeDlg::OnBnClicked_ChkOverrideUhandledExcpFilter)
	ON_BN_CLICKED(IDC_CHK_DEBUG_BREAK_ON_ACTION, &CCrashMeDlg::OnBnClicked_ChkDebugBreakOnAction)
END_MESSAGE_MAP()



// ==============================================================
// CCrashMeDlg -> our debug test routines
// ==============================================================


// ==========================================================================================
// ==========================================================================================
// ==========================================================================================


/*!***************************************************************
* OnBnClicked_ExecuteBreakPoint() 
*
* -> Execute a breakpoint (INT 3).
* If there is a debugger, it will debug break.
* If not the PostMortem debuger will be started.
*
*****************************************************************/
void CCrashMeDlg::OnBnClicked_ExecuteBreakPoint()
{
	DebugBreak();
}


/*!***************************************************************
* OnBnClicked_ExecuteInt3() 
*
* -> Same as OnBnClicked_ExecuteBreakPoint.
*
*****************************************************************/
void CCrashMeDlg::OnBnClicked_ExecuteInt3()
{
	__asm int 3;
}


/*!***************************************************************
* OnBnClicked_RaiseExcpt() 
*
* -> Raise a breakpoint exception. 
* -> The results are similar to calling DebugBreak() directly. 
*
*****************************************************************/
void CCrashMeDlg::OnBnClicked_RaiseExcpt()
{	 
	RaiseException( EXCEPTION_BREAKPOINT, 0, 0, NULL );
}


/*!***************************************************************
* OnBnClicked_ThrowCppExcpt() 
*
* -> Throw C++ exception.
*
*****************************************************************/
void CCrashMeDlg::OnBnClicked_ThrowCppExcpt()
{
	throw( 5 );
}


/*!***************************************************************
* OnBnClicked_DivisionByNull() 
*
* -> will throw a EXCEPTION_INT_DIVIDE_BY_ZERO(0xC0000094)
*
*****************************************************************/
void CCrashMeDlg::OnBnClicked_DivisionByNull()
{
	int nn = 5;
	nn /= 0;
}


/*!***************************************************************
* OnBnClicked_StackOverflow() 
*
* -> allocate a lot of stack
*
*****************************************************************/
void CCrashMeDlg::OnBnClicked_StackOverflow()
{
	// -> try to allocate 1MB on the stack
	// -> this should throw a stack-overflow exception
	//
	// From MSDN:
	// Each new thread receives its own stack space, consisting of both committed and reserved memory. 
	// By default, each thread uses 1 Mb of reserved memory, and one page of committed memory. 
	// The system will commit one page block from the reserved stack memory as needed." 
	// (see MSDN CreateThread > dwStackSize > "Thread Stack Size)"
	BYTE bTemp[OneKilo * 1024];

	bTemp[0] = 0;
}



/*!***************************************************************
* OnBnClicked_StackOverflowRecursion() 
*
* -> allocate stack with recursion until we get an stack overflow
*
*****************************************************************/
void CCrashMeDlg::AllocateALotOfStack(int nn)
{
	// -> try to allocate 1MB in 200KB steps
	// -> the 5th iteration (we break the 1MB limit) should throw a stack-overflow exception
	//
	// From MSDN:
	// Each new thread receives its own stack space, consisting of both committed and reserved memory. 
	// By default, each thread uses 1 Mb of reserved memory, and one page of committed memory. 
	// The system will commit one page block from the reserved stack memory as needed." 
	// (see MSDN CreateThread > dwStackSize > "Thread Stack Size)"
	BYTE bTemp[OnePage * 49]; bTemp[0] = 0;
	if (nn++ < 260)
		AllocateALotOfStack( nn );
}


void CCrashMeDlg::OnBnClicked_StackOverflowRecursion()
{
	AllocateALotOfStack( 0 );
}





void CCrashMeDlg::OnBnClickedBtnNestedExceptions()
{
	try
	{
		throw L"In handler for exception 1\n\n";
	}
	catch( LPCWSTR str1 )
	{
		OutputDebugString( str1 );
		
		try 
		{			
			throw L"In handler for exception 2\n\n";
		}
		catch( LPCWSTR str2 )
		{
			OutputDebugString( str2 );
			
			try 
			{			
				throw L"In handler for exception 3\n\n";
			}
			catch( LPCWSTR str3 )
			{
				OutputDebugString( str3 );
				DebugBreak();
			}
		}
	}
}



// ==========================================================================================
// ==========================================================================================
// ==========================================================================================



/*!***************************************************************
* OnBnClicked_HeapAlloc() 
*
* -> Allocate 10mb on the global heap
*
*****************************************************************/
void CCrashMeDlg::OnBnClicked_HeapAlloc()
{
	if ( m_pMemOnHeap == NULL )
	{
		DWORD dwMemSize = OneMega * 10;
		HANDLE hProcHeap = GetProcessHeap();
		m_pMemOnHeap = HeapAlloc( hProcHeap, HEAP_ZERO_MEMORY, dwMemSize);

		WCHAR wcTmp[500];
		swprintf( wcTmp, 500, L"\n\nHeapAlloc( 0x%p, 8, dwBytes =0x%p ) -->> 0x%p;\n\n\n", hProcHeap, dwMemSize, m_pMemOnHeap );
		OutputDebugString( wcTmp );
		if (m_bDebugBreak == TRUE) 
		{
			DebugBreak();
		}
	}
	else
	{
		MessageBox( L"Heap already allocated", L"Heap already allocated" );
	}
}


/*!***************************************************************
* OnBnClicked_HeapDelloc() 
*
* -> Deallocate m_pMemOnHeap 
*
*****************************************************************/
void CCrashMeDlg::OnBnClicked_HeapDelloc()
{
	if ( m_pMemOnHeap != NULL )
	{		
		HANDLE hProcHeap = GetProcessHeap();
		HeapFree( hProcHeap, HEAP_ZERO_MEMORY, m_pMemOnHeap);

		WCHAR wcTmp[500];
		swprintf( wcTmp, 500, L"\n\nHeapFree( 0x%p, 8, m_pHeap = 0x%p);\n\n\n", hProcHeap, m_pMemOnHeap );
		OutputDebugString( wcTmp );

		m_pMemOnHeap = NULL;
		if (m_bDebugBreak == TRUE) 
		{
			DebugBreak();
		}	
	}
	else
	{
		MessageBox( L"Heap already deallocated", L"Heap already deallocated" );
	}
}


/*!***************************************************************
* OnBnClicked_OperatorNew() 
*
* -> new -> m_pByteBuffer 
*
*****************************************************************/
void CCrashMeDlg::OnBnClicked_OperatorNew()
{
	if ( m_pByteBuffer == NULL )
	{		
		DWORD dwMemSize = OneMega * 10;
		m_pByteBuffer = new BYTE[ dwMemSize ];

		WCHAR wcTmp[500];
		swprintf( wcTmp, 500, L"\n\nnew BYTE[ 0x%p ] -->> 0x%p;\n\n\n", dwMemSize, m_pByteBuffer );
		OutputDebugString( wcTmp );
		if (m_bDebugBreak == TRUE) 
		{
			DebugBreak();
		}
	}
	else
	{
		MessageBox( L"new already called", L"new already called" );
	}
}


/*!***************************************************************
* OnBnClicked_OperatorDelete() 
*
* -> delete -> m_pByteBuffer 
*
*****************************************************************/
void CCrashMeDlg::OnBnClicked_OperatorDelete()
{
	if ( m_pByteBuffer != NULL )
	{
		delete[] m_pByteBuffer;

		WCHAR wcTmp[500];
		swprintf( wcTmp, 500, L"\n\ndelete[] 0x%p; \n\n\n", m_pByteBuffer );	
		OutputDebugString( wcTmp );

		m_pByteBuffer = NULL;
		if (m_bDebugBreak == TRUE) 
		{
			DebugBreak();
		}
	}
	else
	{
		MessageBox( L"delete already called", L"delete already called" );
	}
}


/*!***************************************************************
* OnBnClicked_HeapCreate() 
*
* -> CreateHeap -> m_hHeap 
*
*****************************************************************/
void CCrashMeDlg::OnBnClicked_HeapCreate()
{
#define MY_HEAP_GLAGS	(HEAP_GROWABLE | HEAP_ZERO_MEMORY /*| HEAP_MAKE_TAG_FLAGS*/)

	if ( m_hHeap == NULL )
	{
		m_hHeap = HeapCreate( MY_HEAP_GLAGS, 0,  0);

		WCHAR wcTmp[500];
		swprintf( wcTmp, 500, L"\n\nHeapCreate( 0x%p, 0, 0 ) -->> 0x%p;\n\n\n", MY_HEAP_GLAGS, m_hHeap );
		OutputDebugString( wcTmp );
		if (m_bDebugBreak == TRUE) 
		{
			DebugBreak();
		}
	}
	else
	{
		MessageBox( L"Heap already created", L"Heap already created" );
	}
}


/*!***************************************************************
* OnBnClicked_HeapDestroy() 
*
* -> Destroy( m_hHeap )
*
*****************************************************************/
void CCrashMeDlg::OnBnClicked_HeapDestroy()
{
	if ( m_hHeap != NULL )
	{
		HeapDestroy( m_hHeap );

		WCHAR wcTmp[500];
		swprintf( wcTmp, 500, L"\n\nHeapDestroy( 0x%p );\n\n\n", m_hHeap );
		OutputDebugString( wcTmp );

		m_hHeap = NULL;
		if (m_bDebugBreak == TRUE) 
		{
			DebugBreak();
		}
	}
	else
	{
		MessageBox( L"Heap already destroyed", L"Heap already destroyed" );
	}
}


/*!***************************************************************
* OnBnClicked_MemoryLeak() 
*
* -> Allocate 500 x 1MB ~ 500 MB on the heap
*
*****************************************************************/
void CCrashMeDlg::OnBnClicked_MemoryLeak()
{

	for (int c=0; c<257; c++)
	{
		//BYTE* pByte = new BYTE[OneMega];

		HANDLE hProcHeap = GetProcessHeap();
		BYTE* pByte = (BYTE*) HeapAlloc( hProcHeap, HEAP_ZERO_MEMORY, OneMega);

		// -> RegionUsageIsVAD
		// -> !heap doesn't return infor about VirtualAlloc
		//BYTE* pByte = (BYTE*) VirtualAlloc( NULL, OneMega, MEM_COMMIT, PAGE_READWRITE );


		pByte[0] = 1;
		pByte[1] = 2;

		pByte[OneMega-1] = 3;
	}
}


// ==========================================================================================
// ==========================================================================================
// ==========================================================================================


/*!***************************************************************
* OnBnClicked_LoadDll() 
*
* -> LoadLibrary( "samlib.dll" )
*
*****************************************************************/
void CCrashMeDlg::OnBnClicked_LoadDll()
{
	LoadLibraryW( L"samlib.dll" );
	
	if (m_bDebugBreak == TRUE) 
	{
		DebugBreak();
	}
}


/*!***************************************************************
* OnBnClicked_UnloadDll() 
*
* -> FreeLibrary( "samlib.dll" )
*
*****************************************************************/
void CCrashMeDlg::OnBnClicked_UnloadDll()
{
	HMODULE hMod = GetModuleHandleW( L"samlib.dll" );
	if ( hMod != NULL)
		::FreeLibrary( hMod );
		
	if (m_bDebugBreak == TRUE) 
	{
		DebugBreak();
	}
}



/*!***************************************************************
* OnBnClicked_CreateThread() 
*
* -> Create our thread
*
*****************************************************************/
DWORD WINAPI ThreadProc(LPVOID pbDebugBreak)
{
	if (*((int*)pbDebugBreak) == TRUE) 
	{
		DebugBreak();
	}
		
	while (TRUE)
		Sleep( 500 );

	return 0;
}

void CCrashMeDlg::OnBnClicked_CreateThread()
{
	if ( m_hThread == NULL)
	{
		m_hThread = ::CreateThread( NULL, NULL, &ThreadProc, LPVOID(&m_bDebugBreak), NULL, NULL );
	}
	else
	{
		MessageBox( L"Thread already created", L"Thread already created" );
	}

}


/*!***************************************************************
* OnBnClicked_EndThread() 
*
* -> End our thread
*
*****************************************************************/
void CCrashMeDlg::OnBnClicked_EndThread()
{
	if (m_hThread != NULL)
	{
		if (m_bDebugBreak == TRUE) 
		{
			DebugBreak();
		}
		
		DWORD dwExitCode = NULL;
		::GetExitCodeThread( m_hThread, &dwExitCode );

		if (dwExitCode == STILL_ACTIVE)
		{
			TerminateThread( m_hThread, 10000 );
		}
	}
	else
	{
		MessageBox( L"There is no thread", L"There is no thread" );
	}

	m_hThread = NULL;	
}


// ==========================================================================================
// ==========================================================================================
// ==========================================================================================


/*!***************************************************************
* OnBnClicked_AccessTestVariable() 
*
* -> our "ba" in WinDBG should be hit
*
*****************************************************************/
void CCrashMeDlg::OnBnClicked_AccessTestVariable()
{
	m_nn++;
}


/*!***************************************************************
* CheckForDebugger() 
*
* -> A litle trick with the int 3 exception.
* -> If there is a debuger it will presumabely handle the exception (gH).
*    If not (gN) or if there is no Debugger, then our exception handler
*	 will be executed.
*
*****************************************************************/
BOOL CCrashMeDlg::CheckForDebugger()
{
	__try 
	{
		__asm int 3;	// == DebugBreak();
	}
	__except( (GetExceptionCode() == EXCEPTION_BREAKPOINT) ? EXCEPTION_EXECUTE_HANDLER  : EXCEPTION_CONTINUE_SEARCH) 
	{
		// -> this is our exception handler block
		// -> it is executed if the filter expression returns EXCEPTION_EXECUTE_HANDLER

		// No debugger is attached, so return FALSE and continue.
		return FALSE;
	}
	return TRUE;
}

void CCrashMeDlg::OnBnClicked_CheckForDebugger()
{
	BOOL bDebugerPresent = CheckForDebugger();
	if (bDebugerPresent)
	{
		::MessageBox( this->GetSafeHwnd(), _T("Debuger Present"), _T("Debuger Present"), MB_OK);
	}
	else
	{
		::MessageBox( this->GetSafeHwnd(), _T("No Debuger Present"), _T("No Debuger Present"), MB_OK);
	}
}


/*!***************************************************************
* OnBnClicked_EnterCs() 
*
* -> lock critical section and examine it with WinDBG
*
*****************************************************************/
void CCrashMeDlg::OnBnClicked_EnterCs()
{
	m_cs.Lock();
	if (m_bDebugBreak == TRUE) 
	{
		DebugBreak();
	}
	m_cs.Unlock();
}


/*!***************************************************************
* OnBnClicked_CallingConvention() 
*
* -> OnBnClicked_CallingConvention 
*		-> Function1 -> Function2 -> Function3
*
*****************************************************************/
int __cdecl Function3( int nParam1, int nParam2, int nParam3 )
{
	int nLocal1 = 11;
	int nLocal2 = 12;
	int nLocal3 = 13;
	
	if (nParam1 == 9)
	{
		::MessageBox( NULL, L"You have succeeded to set nParam1=9.", L"Jupi, you hacked me", MB_OK );
	}
	return nParam1 + nParam2 + nParam3;
}

int __fastcall Function2( int nParam1, int nParam2, int nParam3 )
{
	return Function3( nParam1, nParam2, nParam3 );
}


int __stdcall Function1( int nParam1, int nParam2, int nParam3 )
{
	return Function2( nParam1, nParam2, nParam3 );
}


void CCrashMeDlg::OnBnClicked_CallingConvention()
{
	if (m_bDebugBreak == TRUE) 
	{
		DebugBreak();
	}

	int nn = 0;
	nn = Function1( 1,2,3 );	
}



/*!***************************************************************
* OnBnClicked_InvalidHandle() 
*
* -> Access invalid handle
*
*****************************************************************/
void CCrashMeDlg::OnBnClicked_InvalidHandle()
{
	if (m_bDebugBreak == TRUE) 
	{
		DebugBreak();
	}
	
	DWORD dwDummy = 0;
	HANDLE hFile = (HANDLE) 0x01;
	::WriteFile( hFile, L"123456789", 10, &dwDummy, NULL);
}


/*!***************************************************************
* OnBnClicked_SetlastError() 
*
* -> SetLastError
*
*****************************************************************/
void CCrashMeDlg::OnBnClicked_SetlastError()
{
	if (m_bDebugBreak == TRUE) 
	{
		DebugBreak();
	}
	::SetLastError( ERROR_FILE_NOT_FOUND );
}




// ==========================================================================================
// ==========================================================================================
// ==========================================================================================

void CCrashMeDlg::OnBnClicked_TestArrays()
{
	int myInt[2] = { 1,2 };
	if (m_bDebugBreak == TRUE) 
	{
		DebugBreak();
	}
}



// ==========================================================================================
// ==========================================================================================
// ==========================================================================================



/*!***************************************************************
* OnBnClicked_DumpMeOnException() 
* 
* -> generade a memory dump
*
****************************************************************/
void CCrashMeDlg::OnBnClicked_DumpMeOnException()
{
	__try
	{
		// -> the clean way would be to 
		int *pBadPtr = NULL;
		*pBadPtr = 0;
	}
	__except( GenerateDump(GetExceptionInformation(), MiniDumpWithFullMemory) )
	{
	}
}


/*!***************************************************************
* OnBnClicked_WER_ReportFault() 
* 
*	-> generate report
*
*****************************************************************/
void CCrashMeDlg::OnBnClicked_WER_ReportFault()
{
	// -> ReportFault

	__try
	{
		int *pBadPtr = NULL;
		*pBadPtr = 0;
	}
	__except( MyReportFault(GetExceptionInformation()) )
	{
	}
}


/*!***************************************************************
* OnBnClicked_ChkOverrideUhandledExcpFilter() 
* 
*	-> override top-level exception handler
*
*****************************************************************/
int MyTopLevelFrameBasedExceptionFilter(EXCEPTION_POINTERS* pExceptionPointers)
{

	WCHAR* text = L"An unhandled exception occurred. All threads created by Win32 functions have a top-level frame-based (__try:__except) exception filter and handler."
		L"\nThe default Windows defined exception filter starts a Postmortem debugger specified in HKLM\\Software\\Microsoft\\WindowsNT\\CurrentVersion\\AeDebug (i.e. Dr. Watson, WinDBG, VisualStudio)."
		L"\n\nWe have overridden the default filter by calling SetUnhandledExceptionFilter( &MyTopLevelFrameBasedExceptionFilter ) which simply displays this MessageBox."
		L"\nAlternatively we could:"
		L"\n\t a) Create a crash dump here"
		L"\n\t b) Restore the stack of our faulty thread and return EXCEPTION_CONTINUE_EXECUTION"
		L"\n\nBecause our filter-function returns EXCEPTION_EXECUTE_HANDLER the default frame based-handler will be called as we return. It will terminate the process.";

	::MessageBoxW(NULL , text, L"UnhandledException occured -> MyTopLevelFrameBasedExceptionFilter called", NULL);
	return EXCEPTION_EXECUTE_HANDLER;
	//return EXCEPTION_CONTINUE_SEARCH;
}

void CCrashMeDlg::OnBnClicked_ChkOverrideUhandledExcpFilter()
{
	UINT btnState = IsDlgButtonChecked( IDC_CHK_OVERRIDE_UHANDLED_EXCP_FILTER );
	if (btnState == BST_CHECKED)
	{
		SetUnhandledExceptionFilter( (LPTOP_LEVEL_EXCEPTION_FILTER)&MyTopLevelFrameBasedExceptionFilter );
	}
	else
	{
		SetUnhandledExceptionFilter( NULL );
	}
}



void CCrashMeDlg::OnBnClicked_ChkDebugBreakOnAction()
{
	m_bDebugBreak = IsDlgButtonChecked( IDC_CHK_DEBUG_BREAK_ON_ACTION ) == BST_CHECKED;
}