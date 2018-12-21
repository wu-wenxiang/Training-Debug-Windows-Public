## Sysinternals Suite
- How to find which process locked the files?

		>>> aFile = open('a.txt')
		>>> aFile.fileno()
		3
		
		# find it from ProcExp
		
		`handle.exe | findstr /i /R a.txt`
- How to release the file from the process?
		
		# 找ProcExp中找到这个文件，然后释放它，就可以删除了。
		
		>>> aFile.read()
		Traceback (most recent call last):
		  File "<stdin>", line 1, in <module>
		OSError: [Errno 9] Bad file descriptor 
- How to check the owner about a popup window?
	- ProcExp
- How to monitor memory/cpu/io by procexp?
	- Memory: Virtual Memory / Working Set / Private Bytes
	- `!address -summary`
	- Thread Stack
		- `!teb`
		- `dt ntdll!_TEB 0xAAAAAAAA`: StackBase / StackLimit
	- Image
		- `!dh notepad`
- How to check the callstack for a high cpu process?
	- ProcExp
- How to check the strings from heap?
	- ProcExp
- How to check the PATH environment about the process?
	- ProcExp / Procmon
- How to monitor file/registry/network/process/thread?
	- Procmon
		- `aFile = open(r'C:\Users\pear\Desktop\a.txt')`
		- `aFile.read()`
	- Procmon Filter
	- Symbol / CallStack
- How to capture a full memory dump?
	- Procdump
	- DebugDiag
	- `.dump /ma C:\crashdump.dmp`
- How to capture dumps based on conditions?
	- Procdump
	- DebugDiag
- How to analyze dumps automatically?
	- DebugDiag