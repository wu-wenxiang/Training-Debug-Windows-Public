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

#pragma once
#include <afxmt.h>


// CCrashMeDlg dialog
class CCrashMeDlg : public CDialog
{
public:
	CCrashMeDlg(CWnd* pParent = NULL);	// standard constructor
	enum { IDD = IDD_ERAS_ME_DIALOG };

protected:
	HICON m_hIcon;
	
	HANDLE m_hHeap;
	int m_nn;				// -> for BA (break on access)
	VOID* m_pMemOnHeap;		// -> for HeapAlloc
	BYTE* m_pByteBuffer;	// -> for new
	HANDLE m_hThread;		// -> 
	CCriticalSection m_cs;	// ->
	BOOL m_bDebugBreak;

    
	BOOL CheckForDebugger();
	void AllocateALotOfStack(int nn);


	// Generated message map functions
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()

	afx_msg void OnBnClicked_ExecuteBreakPoint();
	afx_msg void OnBnClicked_ExecuteInt3();
	afx_msg void OnBnClicked_RaiseExcpt();
	afx_msg void OnBnClicked_ThrowCppExcpt();	
	afx_msg void OnBnClicked_DivisionByNull();
	afx_msg void OnBnClicked_StackOverflow();
	afx_msg void OnBnClicked_StackOverflowRecursion();

	afx_msg void OnBnClicked_HeapAlloc();
	afx_msg void OnBnClicked_HeapDelloc();
	afx_msg void OnBnClicked_OperatorNew();
	afx_msg void OnBnClicked_OperatorDelete();
	afx_msg void OnBnClicked_HeapCreate();
	afx_msg void OnBnClicked_HeapDestroy();
	afx_msg void OnBnClicked_MemoryLeak();
	
	afx_msg void OnBnClicked_LoadDll();
	afx_msg void OnBnClicked_UnloadDll();
	afx_msg void OnBnClicked_CreateThread();
	afx_msg void OnBnClicked_EndThread();

	afx_msg void OnBnClicked_EnterCs();
	afx_msg void OnBnClicked_AccessTestVariable();
	afx_msg void OnBnClicked_CallingConvention();
	afx_msg void OnBnClicked_InvalidHandle();
	afx_msg void OnBnClicked_SetlastError();
	afx_msg void OnBnClicked_CheckForDebugger();

	afx_msg void  OnBnClicked_TestArrays();

	afx_msg void OnBnClicked_DumpMeOnException();
	afx_msg void OnBnClicked_WER_ReportFault();
	afx_msg void OnBnClicked_ChkOverrideUhandledExcpFilter();
	afx_msg void OnBnClicked_ChkDebugBreakOnAction();
public:
	afx_msg void OnBnClickedBtnNestedExceptions();
};