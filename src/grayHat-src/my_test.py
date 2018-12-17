from ctypes import *
from ctypes import wintypes
import my_debugger
from my_debugger_defines import *

debugger = my_debugger.debugger()

## OnLaunch
# debugger.load(b'C:\\Windows\\System32\\calc.exe')
# import time
# time.sleep(30)
# calc.exe exit after this process exit.

## Attach
# pid = input("Enter the PID of the process to attach to: ")
# debugger.attach(int(pid))
# tlist = debugger.enumerate_threads()
# for t in tlist:
#     tContext = debugger.get_thread_context(t)
#     print('Thread ID: 0x%08x' % t)
#     print('EAX: 0x%08x' % tContext.Eax)

# print('Detach now...')
# debugger.detach()

## Resolve
input("Press any key to resolve... ")
# Must load before call!!!
cdll.LoadLibrary(r'C:\local\github-mine\Training-Debug-Windows\src\TestDLL\TestDLL\x64\Release\TestDLL.dll')
address = debugger.func_resolve(r'C:\local\github-mine\Training-Debug-Windows\src\TestDLL\TestDLL\x64\Release\TestDLL.dll', b'Sub')
print("[*] Address of function: 0x%08x" % address)
proto = WINFUNCTYPE(wintypes.INT, wintypes.INT, wintypes.INT)
func = proto(address)
ret = func(4, 3)
print(ret)

address = debugger.func_resolve(r'msvcrt.dll', b'printf')
# address = debugger.func_resolve(r'C:\Windows\System32\ucrtbase.dll', b'__conio_common_vcprintf')
# address = debugger.func_resolve(r'C:\Users\pear\AppData\Local\Programs\Python\Python37\vcruntime140.dll', b'memcmp')
proto = WINFUNCTYPE(wintypes.INT, wintypes.LPCSTR)
func = proto(address)
func(b'hello, world!\n')

## call exported function from dll
input("Press any key to call exported function... ")
# mydll = cdll.LoadLibrary(r'C:\Windows\System32\ucrtbase.dll')
# https://blogs.msdn.microsoft.com/yizhang/2018/01/20/calling-c-functions-from-python-part-1-using-ctypes/
# WindowsError: [Error 193] %1 is not a valid Win32 application, just because 64bit python call 32bit lib
mydll = cdll.LoadLibrary(r'C:\local\github-mine\Training-Debug-Windows\src\TestDLL\TestDLL\x64\Release\TestDLL.dll')
print(mydll.Sub)
print(mydll.Sub(5, 4))

# printf(b'hahatest')
# debugger.bp_set_mem(printf,10)
# debugger.run()