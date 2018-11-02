## 准备工作、工具、环境和Demo
- 加入微信群：微信 -> 通讯录 -> 新的朋友 -> 添加朋友 -> 面对面建群
- [工具、环境和Demo](https://github.com/wu-wenxiang/Training-Debug-Windows-Public/blob/master/doc/Debug-Environment.md)

## 内容
- Native C++ Desktop Rendering Introduction & Best Practice
	- [Overview of the Windows Graphics Architecture](https://docs.microsoft.com/en-us/windows/desktop/learnwin32/overview-of-the-windows-graphics-architecture)
	- [OpenGL](https://learnopengl.com/Getting-started/OpenGL) & [Unity](https://link.zhihu.com/?target=http%3A//unity3d.com/learn/tutorials/modules) Introduction
	- [Direct2D in action](https://docs.microsoft.com/en-us/windows/desktop/learnwin32/your-first-direct2d-program)
	- Howto: [Improving the performance of Direct2D apps](https://docs.microsoft.com/en-us/windows/desktop/direct2d/improving-direct2d-performance)
- Performance Tunning
	- [Debugging Tips for the Windows Display Driver Model](https://docs.microsoft.com/en-us/windows-hardware/drivers/display/debugging-tips-for-the-windows-vista-display-driver-model)
	- Tools: [Sysinternals](https://docs.microsoft.com/en-us/sysinternals) (Procmon / ProcExp / Handle, etc)
	- [Profiling native C++ rendering apps](https://docs.microsoft.com/en-us/windows/desktop/direct2d/profiling-directx-applications)
		- [Xperf](https://blogs.msdn.microsoft.com/ntdebugging/2008/04/03/windows-performance-toolkit-xperf/) 
		- [GPUView](https://docs.microsoft.com/en-us/windows-hardware/drivers/display/using-gpuview)
- Windbg Usage & Demo
	- [Debugging with Symbols](https://docs.microsoft.com/en-us/windows/desktop/dxtecharts/debugging-with-symbols)
	- [Getting Started With Windbg](https://docs.microsoft.com/en-us/windows-hardware/drivers/debugger/getting-started-with-windbg)
	- [Windbg Commands](https://docs.microsoft.com/en-us/windows-hardware/drivers/debugger/commands)
	- [Debugging Using WinDbg](https://docs.microsoft.com/en-us/windows-hardware/drivers/debugger/debugging-using-windbg)
	- [Standard Debugging Techniques](https://docs.microsoft.com/en-us/windows-hardware/drivers/debugger/standard-debugging-techniques)
	- [CrashMe Project](https://mikedoszhang.blogspot.com/search?q=CrashMe+analysis+tutorial)
	- Tools: [Procdump](https://docs.microsoft.com/en-us/sysinternals/downloads/procdump)
	- Tools: [DebugDiag](https://www.microsoft.com/en-us/download/details.aspx?id=49924)
	- Demo
		- [Crash](https://msdn.microsoft.com/library/windows/desktop/ee416349)
		- [Hang](https://blogs.msdn.microsoft.com/benjaminperkins/2013/01/08/debugging-a-hung-application-with-windbg/)
		- [Native Memory Leak](https://docs.microsoft.com/en-us/windows-hardware/drivers/debugger/using-umdh-to-find-a-user-mode-memory-leak)
		- [Managed Memory Leak](https://blogs.msdn.microsoft.com/paullou/2011/06/28/debugging-managed-code-memory-leak-with-memory-dump-using-windbg/)
		- [CPU High](https://blogs.msdn.microsoft.com/ntdebugging/2008/05/15/how-to-track-down-high-cpu-in-user-mode-applications-a-live-debug/)
	- Scripts: [pykd](https://github.com/wu-wenxiang/Tool-Windbg-Pykd-Scripts)

- What's More?
	- [Getting Started With Windbg Kernel Mode](https://docs.microsoft.com/en-us/windows-hardware/drivers/debugger/getting-started-with-windbg--kernel-mode-)
	- [Mex Extension](https://www.microsoft.com/en-us/download/details.aspx?id=53304): [Usage Blog](https://blogs.msdn.microsoft.com/luisdem/2016/07/19/mex-debugging-extension-for-windbg-2/)
	- [Debugging Using Windbg Preview](https://docs.microsoft.com/en-us/windows-hardware/drivers/debugger/debugging-using-windbg-preview)
	- [Time Travel Debugging Overview](https://docs.microsoft.com/en-us/windows-hardware/drivers/debugger/time-travel-debugging-overview)
	- Scripts: [javascript](https://docs.microsoft.com/en-us/windows-hardware/drivers/debugger/time-travel-debugging-javascript-automation)