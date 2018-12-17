## About Symbol
- What & Why
	- [Generate symbol files for a C++ project](https://docs.microsoft.com/en-us/visualstudio/debugger/how-to-set-debug-and-release-configurations?view=vs-2017)
	- Demo: Dump call stack without & with symbol
	- Doc: [Debugging with Symbols](https://docs.microsoft.com/en-us/windows/desktop/dxtecharts/debugging-with-symbols)
		- What does the symbol path syntax mean?
		- Build server
		- Set environment variable: `_NT_SYMBOL_PATH`
- Tools
	- Demo: Search symbol path in target assembly with Python
	- Demo: Search source path in symbol file with Python
	- Demo: [Dumpbin](https://msdn.microsoft.com/en-us/library/c1h23y6c.aspx?f=255&MSPPError=-2147217396)
		- Developer Command Prompt for VS2017
		- `dumpbin /symbols CrashMe.exe`
		- `dumpbin /PDBPATH:VERBOSE CrashMe.exe`
		- `dumpbin /headers CrashMe.exe`
	- Demo: [pdbcopy](https://docs.microsoft.com/en-us/windows-hardware/drivers/debugger/using-pdbcopy)
		- `copy "C:\Program Files (x86)\Windows Kits\10\Debuggers\x64\pdbcopy.exe" .`
		- `pdbcopy CrashMe.pdb public.pdb -p`
		- `dir`
		- `dv` & "source lines" differences
		- Private PDB
			- Public symbols (typically all functions, static and global variables)
			- A list of object files that are responsible for sections of code in the executable
			- Frame pointer optimization information (FPO)
			- Name and type information for local variables and data structures
			- Source file and line number information
		- Stripped PDB
			- Public symbols (typically only non-static functions and global variables)
			- A list of object files that are responsible for sections of code in the executable
			- Frame pointer optimization information (FPO)
	- Demo: [chkmatch](http://www.debuginfo.com/tools/chkmatch.html)：
		- `chkmatch -c ExeFile DebugInfoFile`
		- `chkmatch -m ExeFile DebugInfoFile`
	- Demo: [Force Load Symbol](http://ntcoder.com/bab/2012/03/06/how-to-force-symbol-loading-in-windbg/): 
		- `.symopt+ 0x40`
		- `.reload /f /i test.exe`
		- [`.reload /f /i MyDll.dll=77777777`](https://stackoverflow.com/questions/10979418/forcing-windbg-to-load-symbols-of-an-unloaded-module)
	- Demo: [Symchk](https://docs.microsoft.com/en-us/windows-hardware/drivers/debugger/using-symchk)
	
			"c:\Program Files\Debugging Tools for Windows\symchk" testing.dll /s
			SYMCHK: FAILED files = 0
			SYMCHK: PASSED + IGNORED files = 1  
- Symbol Hacker
	- Demo: [Export functions](https://github.com/wu-wenxiang/Training-Debug-Windows-Public/tree/master/src/TestDLL/TestDLL)
		- `dumpbin /exports xxx.dll`
		- [Static call](https://github.com/wu-wenxiang/Training-Debug-Windows-Public/tree/master/src/TestDLL/Demo-01-StaticLoad)
		- [Dynamic call](https://github.com/wu-wenxiang/Training-Debug-Windows-Public/tree/master/src/TestDLL/Demo-02-DynamicLoad): [GetProcAddress](https://docs.microsoft.com/en-us/windows/desktop/api/libloaderapi/nf-libloaderapi-getprocaddress)
		- Python call: mydebgger
		
				from ctypes import *
				msvcrt = cdll.msvcrt
				msg = b"Hello\n"
				msvcrt.printf(b"aStr: %s", msg)
	- [Demo: Call internal functions](https://github.com/wu-wenxiang/Training-Debug-Windows-Public/tree/master/src/TestDLL/Demo-03-InternalFunction) by [SymFromName](https://docs.microsoft.com/en-us/windows/desktop/api/dbghelp/nf-dbghelp-symfromname)
		- Real case: linkinfo
		- Real case: scan
- Others
	- What the order when windbg load symbols
		- Ordersa:
			- The location that is specified inside the DLL or the executable file.
			- PDBs that may be present in the same folder as the DLL or executable file.
			- Any local symbol cache folders.
			- Any local network file share symbol servers.
			- Any Internet symbol servers, such as the Microsoft symbol server.
		- Demo: Procmon
	- What if without symbol
		- COM/COM+/DCOM
