## Windbg Usage
- Windbg Usage
	- [Getting Started With Windbg](https://docs.microsoft.com/en-us/windows-hardware/drivers/debugger/getting-started-with-windbg)
		- Steps
			- Onlaunch vs Attach
			- `.sympath`
			- Procmon Callstack
			- `lm`: check symbol load
			- srcpath if private symbol didn't aim the correct src files
		- Notepad
			- How to examine the Process Information?
				- `!peb`
				- `dt ntdll!_PEB`
				- `dt ntdll!_PEB 00acf000`
			- How to set a Breakpoint at function entries?
				- `bp`/`bu`/`bm`: [document](https://docs.microsoft.com/en-us/windows-hardware/drivers/debugger/unresolved-breakpoints---bu-breakpoints-) , [bp vs bu](https://blog.csdn.net/WinGeek/article/details/4025475)
			- How to set a Breakpoint target writing memory?
				- `ba w4 0xAAAAAA`
			- How to viewing and edit Memory?
				- [Viewing and Editing Memory in WinDbg](https://docs.microsoft.com/en-us/windows-hardware/drivers/debugger/memory-window)
				- To open a Memory window, choose Memory from the View menu. (You can also press ALT+5 or click the Memory button)
				- `da` / `db` / `du`: [document](https://docs.microsoft.com/en-us/windows-hardware/drivers/debugger/d--da--db--dc--dd--dd--df--dp--dq--du--dw--dw--dyb--dyd--display-memor)
				- `eza`: [document](https://docs.microsoft.com/en-us/windows-hardware/drivers/debugger/e--ea--eb--ed--ed--ef--ep--eq--eu--ew--eza--ezu--enter-values-)
				- Demo:
				
						0:000> kP
						 # ChildEBP RetAddr  
						00 001bf5e0 7431a7a0 ntdll!KiUserCallbackDispatcher+0x42
						01 001bf61c 01066751 USER32!GetMessageW+0x30
						02 001bf638 01066822 CrashMe!AfxInternalPumpMessage(void)+0x18
						03 001bf644 0106d769 CrashMe!AfxPumpMessage(void)+0x1f
						04 001bf668 0106548b CrashMe!CWnd::RunModalLoop(
						            unsigned long dwFlags = 4)+0xc9
						05 001bf67c 010655f3 CrashMe!CWnd::CreateRunDlgIndirect(
						            struct DLGTEMPLATE * lpDialogTemplate = 0x0124b0d0, 
						            class CWnd * pParentWnd = 0x00000000, 
						            struct HINSTANCE__ * hInst = 0x01060000)+0x3d
						06 001bf6d4 011cca33 CrashMe!CDialog::DoModal(void)+0x148
						07 001bf7e8 011cc31f CrashMe!CCrashMeApp::InitInstance(void)+0x83
						08 001bf800 011ad7d3 CrashMe!AfxWinMain(
						            struct HINSTANCE__ * hInstance = 0x01060000, 
						            struct HINSTANCE__ * hPrevInstance = 0x00000000, 
						            wchar_t * lpCmdLine = 0x00442260 "", 
						            int nCmdShow = 0n1)+0x5f
						0:000> dx -r1 ((CrashMe!wchar_t *)0x442260)
						((CrashMe!wchar_t *)0x442260)                 : 0x442260 : "" [Type: wchar_t *]
						0:000> ezu 0x442260 "Hello,世界"
						0:000> dx -r1 ((CrashMe!wchar_t *)0x442260)
						((CrashMe!wchar_t *)0x442260)                 : 0x442260 : "Hello,世界" [Type: wchar_t *]
			- How to use debugger extensions?
				- [!heap](https://docs.microsoft.com/en-us/windows-hardware/drivers/debugger/-heap)
					- `!heap`
					- `!heap 0 0`
				- [pykd: dump heap strings](https://github.com/wu-wenxiang/Tool-Windbg-Pykd-Scripts/blob/master/scripts/memory-dump-heap-strings.py)
		- Customize Application
			- How to use `!analyze -v` in hang / crash scenarios?
			- Why my source code could not display with `!analyze -v`?
			- How to find error code of a win32 api that returns false?
				- `!teb` / `!gle` / `!error`
	- [Windbg Commands](https://docs.microsoft.com/en-us/windows-hardware/drivers/debugger/commands)
	- [Meta Commands](https://docs.microsoft.com/en-us/windows-hardware/drivers/debugger/meta-commands)
	- [Debugging Using WinDbg](https://docs.microsoft.com/en-us/windows-hardware/drivers/debugger/debugging-using-windbg)
	- [Standard Debugging Techniques](https://docs.microsoft.com/en-us/windows-hardware/drivers/debugger/standard-debugging-techniques)
- Windbg Advanced
	- How to set a Conditional Data Breakpoint?
		
			0:000> x notepad!*wrap*
			00007ff7`1a2e366c notepad!fWrap = <no type information>
			
			# go & break
			
			0:000> ba w4 00007ff7`1a2e366c
			breakpoint 0 redefined
			
			# turn on/off Word Wrap option
			ba w4 notepad!fWrap "j (poi(notepad!fWrap) == 1) '.echo the fWrap flag is true;g';'.echo the fWrap flag is false;g'"
			0:000> g
			the fWrap flag is false
			the fWrap flag is true
			the fWrap flag is false
			the fWrap flag is true
	- How to write a Simple Debugger Command Program?
		- `.foreach (addr {dp notepad+1000 notepad+1000+6f8}) {ln addr}`
		- `r @$t1=notepad`
		- `r @$t2=1000`
		- `r @$t1`
		- `.foreach (addr {dp @$t1+1000 notepad+1000+0x6f8}) {ln addr}`
	- How to build the SimpleLabExts Debugger Extension
		- `.chain`
		- `.unload ntsdexts`
		- `.load ntsdexts`
		- `.hh`
		- `!ntsdexts.help`
	- How to write scripts by pykd, or javascript?
- What's more?
	- About cdb
		- `cd C:\Program Files (x86)\Windows Kits\10\Debuggers\x86`
		- `cdb.exe C:\local\github-mine\Training-Debug-Windows-Public\src\CrashMe\Debug\CrashMe.exe`
		- `cdb.exe -p <PID>`
	- Remote Debug
		- Launch Debug Session: `.server tcp:port=8081`
		
				0:000> .server tcp:port=8081
				Server started.  Client can connect with any of these command lines
				0: <debugger> -remote tcp:Port=8081,Server=wenwaz2016
				0:000> wenwaz2016\pear (tcp 127.0.0.1:63141) connected at Wed Dec 19 06:23:07 2018
				
				C:\Users\pear>netstat -anbo | findstr 8081
				  TCP    0.0.0.0:8081           0.0.0.0:0              LISTENING       6768
				
				C:\Users\pear>tasklist | findstr 6768
				cdb.exe                       6768 RDP-Tcp#4                  3     21,880 K
		- Connect to Remote Session: `tcp:server=localhost, port=8081`
		
				Server started.  Client can connect with any of these command lines
				0: <debugger> -remote tcp:Port=8081,Server=wenwaz2016
				wenwaz2016\pear (tcp 127.0.0.1:63141) connected at Wed Dec 19 06:23:07 2018
	- About Mex
		- Demo: Mex Usage
	- About Windbg Preview
		- Time Travel Debugging Overview
	- About X64 Debug
		- Parameter Passing and Stack
		- Home Space and Optimized Code 
	- [Getting Started With Windbg Kernel Mode](https://docs.microsoft.com/en-us/windows-hardware/drivers/debugger/getting-started-with-windbg--kernel-mode-)
	- [Mex Extension](https://www.microsoft.com/en-us/download/details.aspx?id=53304): [Usage Blog](https://blogs.msdn.microsoft.com/luisdem/2016/07/19/mex-debugging-extension-for-windbg-2/)
		- `!mex.writemodule -a -p C:\testmodule`
		- `!mex.strings`
		- `!mex.runaway2`
		- `!mex.t`
		- `!mex.us -l`
	- [Debugging Using Windbg Preview](https://docs.microsoft.com/en-us/windows-hardware/drivers/debugger/debugging-using-windbg-preview)
	- [Time Travel Debugging Overview](https://docs.microsoft.com/en-us/windows-hardware/drivers/debugger/time-travel-debugging-overview)
		- [Time Travel Debugging - Record a trace](https://docs.microsoft.com/en-us/windows-hardware/drivers/debugger/time-travel-debugging-record)
		- [Time Travel Debugging - Replay a trace](https://docs.microsoft.com/en-us/windows-hardware/drivers/debugger/time-travel-debugging-replay)
		- [Time Travel Debugging - Working with Trace Files](https://docs.microsoft.com/en-us/windows-hardware/drivers/debugger/time-travel-debugging-trace-file-information)
		- [Time Travel Debugging - Sample App Walkthrough](https://docs.microsoft.com/en-us/windows-hardware/drivers/debugger/time-travel-debugging-walkthrough)
	- Scripts: [javascript](https://docs.microsoft.com/en-us/windows-hardware/drivers/debugger/time-travel-debugging-javascript-automation)
		- [Demo for IE](https://github.com/wu-wenxiang/Tool-Windbg-Pykd-Scripts/blob/master/javascripts/DumpHtmlElements.js)