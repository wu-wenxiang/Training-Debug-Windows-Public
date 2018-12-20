## About Scenarios
- About Crash
	- How to debug Access Violations?
		- Load dump
		- Check error from the front lines, see AV exception
		- Check thread, switch to it: `!mex.grep 1F69 ~`, `~146s`
		- [`.ecxr`](https://docs.microsoft.com/en-us/windows-hardware/drivers/debugger/-ecxr--display-exception-context-record-)
		- `kP` see Exception callstack
		- `.cxr` switch to current callstack
		- `kP` see Current callstack
		- `!teb` / `!gle` => 'File not found'
		- Next Step:
			- 1. Procmon to monitor which file
			- 2. Check code which found in current/exception callstack
			- 3. Dump heap string, maybe also could find string name
	- Demo
		
			from ctypes import *
			msvcrt = cdll.msvcrt
			msg = b"Hello\n"
			msvcrt.printf(b"aStr: %s", msg)
			input('Press any key to raise AV exception.')
			msvcrt.printf(45, msg)
			
			# !address <xxxx>
			
			#include "pch.h"
			#include <iostream>
			
			int main()
			{
			    std::cout << "Hello World!\n"; 
				char *gpszTestString = new char[15];
				strcpy_s(gpszTestString, 15, "Hello World!!\n");
				delete[] gpszTestString;
				printf("%s", gpszTestString);
				system("pause");
				return 0;
			}
	- How to debug Heap Corruption?
		- `*cp++ = 'a'`
	- How to debug Stack Corruption?
		- Capture a correct dump, use procdump/DebugDiag if WER not works
		- Open the crash dump with windbg, check exception at the front lines
		- If StackOverflow
			- `kd`: show stack in raw styles
			- `k`: callstack
		- Find which functions raise the stack overflow, then uf it
		- Check assembly code or source code 
- About Hang
	- Common hang scenarios
		- Wait for Lock
		- Wait for Web Services
		- Wait for DB
	- How to debug a Spinning Thread?
- About Leak
	- How to debug a Native Memory Leak?
	- Finding COM Leaks Using Extensions
	- VMMap
	- RAMMap
	- LeakTrack extension
- About HighCPU
	- Live Debug with HighCPU
		- Demo: Badguy
	- Dump Analysis with HighCPU
	- Common HighCPU scenarios
		- Regex
		- Parallel race
		- Blank loop
		- GC

## [CrashMe Project](http://windbg.info/apps/46-crashme.html)
- 01-breakpoint
	- Tools: [WER Dump](https://docs.microsoft.com/zh-cn/windows/desktop/wer/wer-settings)
	- Tools: [Procdump](https://docs.microsoft.com/en-us/sysinternals/downloads/procdump)
	- EventLog
	- Dump first check
	- `!analyze -v`
- 02-asm-int-3
	- Faulting IP
	- Error Code
- 03-raise-exception
	- kernelbase!RaiseException
- 04-throw
	- `.dump /ma C:\test.dmp`
	- `kP`
	- `dd <pExceptionObject>`
- 05-stackoverflow
	- [`windbg -I`](https://docs.microsoft.com/en-us/windows-hardware/drivers/debugger/windbg-command-line-options)
	- [enabling-postmortem-debugging](https://docs.microsoft.com/en-us/windows-hardware/drivers/debugger/enabling-postmortem-debugging)
	- [How to disable or enable Dr. Watson for Windows](https://support.microsoft.com/en-us/help/188296/how-to-disable-or-enable-dr-watson-for-windows)
	- [`kd`](https://docs.microsoft.com/en-us/windows-hardware/drivers/debugger/k--kb--kc--kd--kp--kp--kv--display-stack-backtrace-)
	- `uf <Address>`
	- `.exr -1`, `.exr`, `.cxr`, `.ecxr`
	- `.lastevent`
- 06-Division-By-NULL
	- `xor  ecx,ecx`
	- `!analyze -v`
- 07-StackOverflow-with-recursion
	- `kd`
	- `!analyze -v`
- 08-Nested-Exceptions
	- `.exr -1`
	- `dv`
- 09-Access-Test-Variable
	- Locals Window
	- `this` -> `eax`
- 10-check-for-debugger
	- Modify memory
- 11-Enter-Critical-Section
	- `!locks -v`
- 12-test-Calling-Conventions
	- `t`
- 13-Invalid-Handles 
	- `!handle 0`
- 14-set-last-error
	- `!gle`
- 15-HeapAlloc
	- `!heap`
	- `!address`
- 16-HeapDealloc

## Demo
- [Crash](https://msdn.microsoft.com/library/windows/desktop/ee416349)
- [Hang](https://blogs.msdn.microsoft.com/benjaminperkins/2013/01/08/debugging-a-hung-application-with-windbg/)
- [Hang2](https://blogs.msdn.microsoft.com/msdnts/2006/11/24/how-to-debug-application-crashhang-in-production-environment/)
- [Native Memory Leak](https://docs.microsoft.com/en-us/windows-hardware/drivers/debugger/using-umdh-to-find-a-user-mode-memory-leak)
- [Managed Memory Leak](https://blogs.msdn.microsoft.com/paullou/2011/06/28/debugging-managed-code-memory-leak-with-memory-dump-using-windbg/)
- [CPU High](https://blogs.msdn.microsoft.com/ntdebugging/2008/05/15/how-to-track-down-high-cpu-in-user-mode-applications-a-live-debug/) 
	- `!runaway`
- Scripts: [pykd](https://github.com/wu-wenxiang/Tool-Windbg-Pykd-Scripts)
- Tools: [DebugDiag](https://www.microsoft.com/en-us/download/details.aspx?id=49924)
	- Demo: [How to use the Debug Diagnostics tool to troubleshoot a process that has stopped responding in IIS](https://support.microsoft.com/en-us/help/919792/how-to-use-the-debug-diagnostics-tool-to-troubleshoot-a-process-that-h)
	- Demo: [How to use the Debug Diagnostics Tool to troubleshoot high CPU usage by a process in IIS](https://support.microsoft.com/en-us/help/919791/how-to-use-the-debug-diagnostics-tool-to-troubleshoot-high-cpu-usage-b)
