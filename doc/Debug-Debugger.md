## About Symbol
- Rationales
	- What's the differents between c call & standard call?
		- cdll：参数列表从右向左入栈，调用者负责清栈
		- windll: stdcall, Win32 API原生调用约定，还是从右向左入栈，但是被调用的函数自己清栈
		- oledll: 与windll完全相似，但oledall会假定其载入的函数会统一返回一个Widnows HRESULT错误编码。这类错误编码专门服务于COM（组建对象模型）函数，用于表示错误信息。
		- Demo: x86 assembly & stack
		
				int my_socks(arg1, arg2, arg3)
				{
					char testLocals;
					...
				}
	- What's the basic functions about a debugger?
		- 调试器基本功能：运行/暂停执行/单步执行/设置断点/修改寄存器和内存数据/捕获异常事件
		- 白盒：源码级别的跟踪调试，开发阶段
		- 黑盒：汇编级别的调试
		- 内核态调试：ring0，wireshark与内核态网卡驱动程序进行交互
		- 用户态调试：ring3
		- Windbg/OllyDbg，gdb，PyDbg，Immunity Debugger
		- 智能调试器：自行编写脚本插件，Not AI
		- 调试器原理
			- 无限循环，等待调试事件
			- 事件发生，调用处理历程
			- 处理完毕，继续无限循环，等待调试事件
	- List the typical debug events
		- 断点触发
		- 非法内存操作（Access Violation / Segment Fault）
		- 由被调试程序抛出的异常
		- 其它：进程/线程创建，动态链接库的运行时导入
	- What's the differences between soft, hard and memory breakpoints?
		- 软断点
			- 实现方式是通过替换操作码的首字节为CC，**INT3**事件

					# 设置软断点之前的操作码
					0x44332211:	8BC3	MOV EAX, EBX
					# 设置软断点之后的操作码 
					0x44332211:	CCC3	MOV EAX, EBX
			- INT3事件处理函数会检查断点列表，将其恢复
			- 优点：可以设置无数个
			- 缺点：绕不开CRC校验
		- 硬件断点
			- 8个调试寄存器
				- DR0-DR3：内存地址，因此**最多使用4个硬件断点**
				- DR4/DR5: 保留
				- DR6: 调试状态寄存器，上一次断点出发所产生的调试事件类型信息
				- DR7: 硬件断点的激活开关，存储各个断点的触发条件信息，可以设定触发条件
					- 某个地址上的指令被执行：函数入口，函数入口+偏移
					- 当数据被写入一个特定内存地址：ba
					- 当数据被读出或写入（不包括执行）一个特定非可执行内存地址
			- 硬件断电是 **INT1** 事件
		- 内存断点
			- 改变一个内存区域或一个内存页的访问权限，内存页是操作系统可以一次处理的最小内存块
			- 访问权限
				- 页可执行：读/写 -> AV
				- 页可读：写/执行 -> AV
				- 页可写
				- 保护页：任何访问 -> 一次性AV，然后恢复到之前状态
			- 可以组合权限，比如
				- 读+写
				- 读+执行
			- 保护页
				- 隔离“堆”和“栈”
				- 确保内存块增长不会超过某一边界
				- 用于实现**内存断点机制**：比如对某缓存区进行页保护，等数据写入时就可以被捕捉到事件
			- 优点：也可以绕过CRC，并且可以设置无数断点
			- 缺点：必须以内存页为单位
- Implement
	- How to implement Onlaunch debugging?
		- [CreateProcessA()](https://docs.microsoft.com/en-us/windows/desktop/api/processthreadsapi/nf-processthreadsapi-createprocessa)，Demo: [my_test.py](https://github.com/wu-wenxiang/Training-Python/blob/master/Python-Common/debug/grayHat-src/my_test.py) ???
			- lpApplicationName：文件路径
			- lpCommandLine：命令行参数
			- dwCreationFlags：使生成的进程自动具备可调试特性
			- lpStartupInfo：指定进程的启动方式
			- lpProcessInformation：记录进程启动成功后的相关信息
	- How to implement Attach debugging?
		- [my_test.py](https://github.com/wu-wenxiang/Training-Python/blob/master/Python-Common/debug/grayHat-src/my_test.py) ???
		- [OpenProcess](https://docs.microsoft.com/en-us/windows/desktop/api/processthreadsapi/nf-processthreadsapi-openprocess)：获取指向目标进程的句柄，用于判断是否能顺利访问目标进程
			- dwDesiredAccess：索要哪种类型的访问权限，一般`PROCESS_ALL_ACCESS`
			- bInheritHandle：False
			- dwProcessId：PID
		- [DebugActiveProcess](https://msdn.microsoft.com/en-us/library/windows/desktop/ms679295(v=vs.85).aspx)：Attach住对应的进程
		- [WaitForDebugEvent](https://msdn.microsoft.com/en-us/library/windows/desktop/ms681423(v=vs.85).aspx)：在循环中反复调用这个函数，捕捉目标进程中的调试事件
	- How to implement Breakpoint?
		- [ReadProcessMemory](https://msdn.microsoft.com/en-us/library/windows/desktop/ms680553(v=vs.85).aspx) / [WriteProcessMemory](https://msdn.microsoft.com/en-us/library/windows/desktop/ms681674(v=vs.85).aspx)
		- [GetMoudleHandle](https://docs.microsoft.com/en-us/windows/desktop/api/libloaderapi/nf-libloaderapi-getmodulehandlea)：取得模块的Handle，用于接下来的GetProcAddress
		- [GetProcAddress](https://docs.microsoft.com/en-us/windows/desktop/api/libloaderapi/nf-libloaderapi-getprocaddress)：从DLL中取到Exported的函数或者变量的地址
	- How to implement Hook & Injection?
		- [CreateRemoteThread](https://docs.microsoft.com/en-us/windows/desktop/api/processthreadsapi/nf-processthreadsapi-createremotethread): Creates a thread that runs in the virtual address space of another process
		- 将编译完的二进制模块加载进另一个进程
			- 使用VirtualAllocEx在目标进程的地址空间中创建一块我们DLL所在路径长度的内存空间。
			- 使用WriteProcessMemory将DLL路径写入分配的内存。
			- 一旦DLL路径写入内存中，再使用CreateRemoteThread（或者其他无正式说明的功能），它再调用LoadLibrary函数将DLL注入目标进程中。
		- 代码注入
			- 与dll注入相比，代码注入更隐蔽，不会留下assembly
			- 代码注入与dll注入的区别在于代码注入调用CreateRemoteThread时，会给出代码地址。