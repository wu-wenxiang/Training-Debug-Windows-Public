## About Symbol
- What & Why
	- Demo: Dump call stack without & with symbol
	- Doc: [Debugging with Symbols](https://docs.microsoft.com/en-us/windows/desktop/dxtecharts/debugging-with-symbols)
		- What does the symbol path syntax mean?
		- Build server
		- Set environment variable: `_NT_SYMBOL_PATH`
- Tools
	- Demo: Search symbol path in target assembly with Python ???
	- Demo: Search source path in symbol file with Python ???
	- Demo: [Dumpbin](https://msdn.microsoft.com/en-us/library/c1h23y6c.aspx?f=255&MSPPError=-2147217396)
		- `dumpbin /symbols CrashMe.exe`
		- `dumpbin /PDBPATH:VERBOSE CrashMe.exe`
		- `dumpbin /headers CrashMe.exe`
	- Demo: [pdbcopy](https://docs.microsoft.com/en-us/windows-hardware/drivers/debugger/using-pdbcopy)
		- `pdbcopy CrashMe.pdb public.pdb -p`
		- `dv` & "source lines" differences
	- Demo: [chkmatch](http://www.debuginfo.com/tools/chkmatch.html)：
		- `chkmatch -c ExeFile DebugInfoFile`
		- `chkmatch -m ExeFile DebugInfoFile`
	- Demo: [Force Load Symbol](http://ntcoder.com/bab/2012/03/06/how-to-force-symbol-loading-in-windbg/): 
		- `.symopt+ 0x40`
		- `.reload /f /i test.exe`
		- [`.reload /f /i MyDll.dll=77777777`](https://stackoverflow.com/questions/10979418/forcing-windbg-to-load-symbols-of-an-unloaded-module)
	- Demo: [Symchk](https://docs.microsoft.com/en-us/windows-hardware/drivers/debugger/using-symchk)
		- `"c:\Program Files\Debugging Tools for Windows\symchk" testing.dll /s`
- Symbol Hacker
	- Demo: Export functions ???
		- `dumpbin /exports xxx.dll`
		- Static call ???
		- Dynamic call ??? [GetProcAddress](https://docs.microsoft.com/en-us/windows/desktop/api/libloaderapi/nf-libloaderapi-getprocaddress)
		- Python call ??? mydebgger
		
				from ctypes import *
				msvcrt = cdll.msvcrt
				msg = b"Hello\n"
				msvcrt.printf(b"aStr: %s", msg)
	- Demo: Call internal functions by [SymFromName](https://docs.microsoft.com/en-us/windows/desktop/api/dbghelp/nf-dbghelp-symfromname)
		- Real case: linkinfo ???
		- Real case: scan ???
- Others
	- What the order when windbg load symbols?
		- Demo: Procmon ???
	- What if without symbol?
		- COM/COM+/DCOM ???
