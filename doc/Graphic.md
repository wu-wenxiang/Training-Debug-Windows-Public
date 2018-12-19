## Native C++ Desktop Rendering Introduction 
- Graphic
	- Rendering Pipeline
	- ![渲染管线.jpg](https://pic4.zhimg.com/v2-1e286dd517c717e3f1c48792275f7e87_r.jpg)
	- Rasterization
	- ![三角形的软件光栅化.png](http://hi.csdn.net/attachment/201103/8/8458191_1299585635RfNA.png)
- Overview
	- 功能+性能+易用性
	- ![graphics01.png](https://docs.microsoft.com/en-us/windows/desktop/learnwin32/images/graphics01.png)
	- [GDI calls these DrvXxx functions to pass data to the drive](https://technet.microsoft.com/en-us/evalcenter/ff570139(v=vs.90))
	- [Overview of the Windows Graphics Architecture](https://docs.microsoft.com/en-us/windows/desktop/learnwin32/overview-of-the-windows-graphics-architecture)
- [GDI](https://msdn.microsoft.com/en-us/library/ms969913.aspx)
	- Demo：[wicviewergdi](https://github.com/Microsoft/Windows-classic-samples/tree/master/Samples/Win7Samples/multimedia/wic/wicviewergdi)
		- [Windows Imaging Component (WIC) provides a Component Object Model (COM) ](https://docs.microsoft.com/en-us/windows/desktop/wic/-wic-api)
		- COINIT_APARTMENTTHREADED
		- 无限循环，分发消息
	- Demo: [comparing-direct2d-and-gdi](https://docs.microsoft.com/en-us/windows/desktop/direct2d/comparing-direct2d-and-gdi)
		- ![direct2d-vs-gdi1.png](https://docs.microsoft.com/en-us/windows/desktop/direct2d/images/direct2d-vs-gdi1.png)
		- GDI和GDI Driver在每个Logon Session中都存在一个，bug会导致蓝屏
		- DirectX Driver在User Mode，更简单，Bug不会导致蓝屏
		- ![direct2d-vs-gdi2.png](https://docs.microsoft.com/en-us/windows/desktop/direct2d/images/direct2d-vs-gdi2.png)
		- GDI的部分功能获得了硬件加速，比如BitBlt，其它则没有
		- GDI对特明度支持有限，即便有支持也是通过bitblt位块转换操作（位块转换操作比如将256色图转换成真彩）
		- GDI+支持特明度，但通过CPU计算
		- 而Direct2D支持全硬件加速的特明度计算。
		- The Desktop Window Manager (DWM), 要求GDI渲染的Surface通过D3D渲染显示到屏幕。这在XP中做不到，因为XP的GDI和D3D的driver stack完全不兼容。Vista开始, GDI DDI display driver实现在Canonical Display Driver (CDD)中，它将GDI内容专程系统内存位图交给DWM。
		- DWM Bug
			- ![graphics05.png](https://docs.microsoft.com/en-us/windows/desktop/learnwin32/images/graphics05.png)
			- Win7 DWM turned on (physical machine with video driver installed and Aero Theme enabled)
			- Since Windows 8, DWM can’t be turned off. Executing the enable DWM function will not do anything, DWM will always be active.
			- ![graphics04.png](https://docs.microsoft.com/en-us/windows/desktop/learnwin32/images/graphics04.png)
			- 在Windows Vista之前，Windows程序将直接绘制到屏幕上
			- 换句话说，程序将直接写入由视频卡显示的内存缓冲器
			- 两个窗口都画到了同一个内存区域
			- 当拖动最上面的窗口时，它下面的窗口必须重新绘制。如果重画太慢，它会导致先前图像中显示的伪影
			- 桌面窗口管理器（DWM）从根本上改变了Windows的绘制方式。当启用DWM时，窗口不再直接绘制到显示缓冲器。
			- 相反，每个窗口绘制到屏幕外的内存缓冲区，也称为屏幕外表面Surface。DWM然后将这些表面复合到屏幕上。
		- [GdiInteropSample](https://github.com/Microsoft/Windows-classic-samples/tree/master/Samples/Win7Samples/multimedia/Direct2D/GdiInteropSample)
		- ![gdicontentind2d.png](https://docs.microsoft.com/en-us/windows/desktop/direct2d/images/gdicontentind2d.png)
- [GDI+](https://docs.microsoft.com/en-us/windows/desktop/gdiplus/-gdiplus-gdi-start)
	- 支持.NET，代码更抽象得更好
	- 增加了功能支持，比如特明度
	- Demo：[DrawLine](https://docs.microsoft.com/en-us/windows/desktop/gdiplus/-gdiplus-drawing-a-line-use)
	- Demo：[bitmaps](https://docs.microsoft.com/en-us/windows/desktop/gdiplus/-gdiplus-loading-and-displaying-bitmaps-use)
- [OpenGL](https://learnopengl.com/Getting-started/OpenGL)
	- [Get Start](https://learnopengl-cn.readthedocs.io/zh/latest/01%20Getting%20started/01%20OpenGL/)
		- 一般认为OpenGL是一个API，包含了一系列可以操作图形、图像的函数。
		- 然而，OpenGL本身并不是一个API，它仅仅是一个由Khronos组织制定并维护的规范(Specification)。
		- OpenGL规范严格规定了每个函数该如何执行，以及它们的输出值。
		- 实际的OpenGL库的开发者通常是显卡的生产商，具体实现各个不同。Apple/Linux
		- 当产生一个bug时通常可以通过升级显卡驱动来解决
		- 早期的OpenGL使用立即渲染模式（Immediate mode，也就是固定渲染管线），简单、低效、不灵活
		- OpenGL3.2开始，规范文档开始废弃立即渲染模式，推出核心模式(Core-profile)
		- 当使用OpenGL的核心模式时，OpenGL迫使我们使用现代的函数
	- [OpenGL on Windows](https://docs.microsoft.com/en-us/windows/desktop/OpenGL/opengl-on-windows-nt--windows-2000--and-windows-95-98)
- [Unity](https://link.zhihu.com/?target=http%3A//unity3d.com/learn/tutorials/modules) 
	- unity是游戏引擎，directx是图形函数库，下面箭头表示调用关系。 
	- 你的程序-->Unity3D-->DirectX-->图形设备（显卡）。 
	- 当然，DirectX和OpenGL是等价的图形函数库，都是游戏引擎的下层调用接口，二者可替换。

## [Direct2D in action](https://docs.microsoft.com/en-us/windows/desktop/learnwin32/your-first-direct2d-program)
 - Hello world
	- Creating a Window
		- ![window01.png](https://docs.microsoft.com/en-us/windows/desktop/learnwin32/images/window01.png)
		- 这里的Class不是C++层面的Class，而是注册到系统的一个数据结构
		- 为什么要加一？[fillrect API](https://docs.microsoft.com/en-us/windows/desktop/api/winuser/nf-winuser-fillrect)，因为这里是从0开始计数的。
	- Window Messages
	- Writing the Window Procedure
		- 避免Hang
		- Create a new thread.
		- Use a thread pool.
		- Use asynchronous I/O calls.
		- Use asynchronous procedure calls.
	- Painting the Window
		- The FillRect function is part of the Graphics Device Interface (GDI)
		- ![painting01.png](https://docs.microsoft.com/en-us/windows/desktop/learnwin32/images/painting01.png)
		- ![painting02.png](https://docs.microsoft.com/en-us/windows/desktop/learnwin32/images/painting02.png)
		- ![painting03.png](https://docs.microsoft.com/en-us/windows/desktop/learnwin32/images/painting03.png)
	- Closing the Window
	- Managing Application State
		- 抽象一下
		- 多根继承
- COM
	- What Is a COM Interface?
		- C++中，近似Interface的概念是纯虚类，只有纯虚函数，没有其它成员
		- ![com02.png](https://docs.microsoft.com/en-us/windows/desktop/learnwin32/images/com02.png)
		- [一个进程可以创建多个STA（单线程公寓），每个STA中可以包含多个COM对象。只有创建STA的线程可以直接访问STA内部的COM对象，因此不用担心多线程问题](https://msdn.microsoft.com/en-us/library/ms809971.aspx)
	- Initializing the COM Library
	- Error Codes in COM
	- Creating an Object in COM
		- ![com03.png](https://docs.microsoft.com/en-us/windows/desktop/learnwin32/images/com03.png)
	- Example: The Open Dialog Box
		- ![fileopen01.png](https://docs.microsoft.com/en-us/windows/desktop/learnwin32/images/fileopen01.png)
	- Managing the Lifetime of an Object
		- ![com04.png](https://docs.microsoft.com/en-us/windows/desktop/learnwin32/images/com04.png)
		- ![com05.png](https://docs.microsoft.com/en-us/windows/desktop/learnwin32/images/com05.png)
	- Asking an Object for an Interface
		- ![com06.png](https://docs.microsoft.com/en-us/windows/desktop/learnwin32/images/com06.png)
		- ![com07.png](https://docs.microsoft.com/en-us/windows/desktop/learnwin32/images/com07.png)
	- Memory Allocation in COM
		- CoTaskMemFree(pszFilePath);
		- **Managed Code 内存泄露问题**
	- COM Coding Practices
		- SafeRelease
		- Smart Pointer
	- Error Handling in COM
- Clock
	- The Desktop Window Manager
	- Retained Mode Versus Immediate Mode
		- 图形API可以分为保留模式API和即时模式API。
		- DATT2D是一种即时模式API。Windows演示基础（WPF）是保留模式API的一个例子。
		- 保留模式的Lib做更多事，因此更易于使用，缺点是不够灵活，更占内存，性能不够优秀
		- ![graphics06.png](https://docs.microsoft.com/en-us/windows/desktop/learnwin32/images/graphics06.png)
		- 即时模式则性能更好，但用起来更繁复
		- ![graphics07.png](https://docs.microsoft.com/en-us/windows/desktop/learnwin32/images/graphics07.png)
	- Your First Direct2D Program
		- ![graphics08.png](https://docs.microsoft.com/en-us/windows/desktop/learnwin32/images/graphics08.png)
	- Render Targets, Devices, and Resources
		- ![graphics09.png](https://docs.microsoft.com/en-us/windows/desktop/learnwin32/images/graphics09.png) 
		- ![graphics10.png](https://docs.microsoft.com/en-us/windows/desktop/learnwin32/images/graphics10.png)
	- Drawing with Direct2D
		- Create device-independent resources.
		- Render the scene.
			- Check if a valid render target exists. If not, create the render target and the device-dependent resources.
			- Call ID2D1RenderTarget::BeginDraw.
			- Issue drawing commands.
			- Call ID2D1RenderTarget::EndDraw.
			- If EndDraw returns D2DERR_RECREATE_TARGET, discard the render target and device-dependent resources.
		- Repeat step 2 whenever you need to update or redraw the scene.
	- DPI and Device-Independent Pixels
		- Dots per inch (DPI)
		- Device-independent pixel (DIPs).
	- Using Color in Direct2D
		- ![graphics15.png](https://docs.microsoft.com/en-us/windows/desktop/learnwin32/images/graphics15.png)
		- ![graphics14.png](https://docs.microsoft.com/en-us/windows/desktop/learnwin32/images/graphics14.png)
	- Applying Transforms in Direct2D
		- ![graphics18.png](https://docs.microsoft.com/en-us/windows/desktop/learnwin32/images/graphics18.png)
	- Appendix: Matrix Transforms
	
			>>> import numpy as np
			>>> a=np.array([[1,2],[3,4]])
			>>> b=np.array([[11,12],[13,14]])
			>>> a
			array([[1, 2],
			       [3, 4]])
			>>> b
			array([[11, 12],
			       [13, 14]])
			>>> np.dot(a,b)
			array([[37, 40],
			       [85, 92]])
- Input
	- Mouse Movement
		- ![input02.png](https://docs.microsoft.com/en-us/windows/desktop/learnwin32/images/input02.png)
		- ![input03.png](https://docs.microsoft.com/en-us/windows/desktop/learnwin32/images/input03.png)

## Howto: [Improving the performance of Direct2D apps](https://docs.microsoft.com/en-us/windows/desktop/direct2d/improving-direct2d-performance)
- Resource usage
	- 硬件上的资源创建和删除是昂贵的操作，因为它们需要大量的开销来与视频卡通信。Bitmaps and brushes 
	- Rendering条件：EndDraw / Flush / Buffer满
	- Reuse resources：游戏中变化的位图
- Restrict the use of flush
	- 让DirectX自己来管理，类似GC
- Bitmaps
	- Create large bitmaps
	- Create an atlas of bitmaps
		- Demo: [atlas](https://docs.microsoft.com/en-us/windows/desktop/direct2d/atlas)
		- ![atlas.png](https://docs.microsoft.com/en-us/windows/desktop/direct2d/images/atlas.png)

				ComPtr<ID2D1Effect> atlasEffect;

				// Create the Atlas Effect.
				DX::ThrowIfFailed(m_d2dContext->CreateEffect(CLSID_D2D1Atlas, &atlasEffect));
				
				// Set the input.
				atlasEffect->SetInputEffect(0, inputImage.Get());
				
				// The images here are 150 x 150 pixels.
				float size = 150.0f;
				
				// Compensate for the padding between images.
				float padding = 10.0f;
				
				// The input rectangle.  150 x 150 pixels with 10 pixel padding
				D2D1_Vector_4F inputRect = D2D1::Vector4F(size + (padding * 2), padding, size, size);
				
				DX::ThrowIfFailed(atlasEffect->SetValue(D2D1_ATLAS_PROP_INPUT_RECT, inputRect));
				
				// Draw the image
				m_d2dContext->DrawImage(atlasEffect.Get());
		- ![atlas2.png](https://docs.microsoft.com/en-us/windows/desktop/direct2d/images/atlas2.png)
	- Create shared bitmaps
		- 上层调用者直接返回已经存在的位图对象
	- Copying bitmaps
- Use tiled bitmap over dashing
- General guidelines for rendering complex static content
	- Full scene caching using a color bitmap
		- compatible render target，渲染目标就是一块显卡上的Buffer
		- scene bitmap
		- device context
	- Per primitive（图元） caching using an A8 bitmap and the FillOpacityMask method
	- Per primitive caching using geometry realizations
		- Win8.1开始引入几何实现，是一种更好的图元绘制方案
- Geometry rendering
	- 用具体的图元绘制函数代替抽象的，因为具体的往往有更好的优化。
	- Rendering static geometry：不透明度掩模和几何实现能提高性能，如上
	- Use a multithreaded device context：多线程，利用多核优势
- Drawing text with Direct2D
	- 渲染分两种，一种是Common的，另一种较少见，适用于已经知道字形渲染位置的
	- DrawTextLayout Vs. DrawText
		- 如果必须多次呈现相同的文本，则使用DrawTextLayout而不是DrawText
		- 因为每次调用DrawText都会创建一个布局。
	- Choosing the right text rendering mode
		- 灰度抗锯齿功能比真彩抗锯齿功能性能好很多，效果却不差多少，参考：[`D2D1_TEXT_ANTIALIAS_MODE`枚举](https://technet.microsoft.com/zh-cn/library/dd368170(VS.85).aspx)
	- Caching
		- 使用全场景或每个图元位图缓存，就像绘制其他图元一样。
- Clipping an arbitrary shape
	- 可以通过使用具有几何掩码的层或具有不透明度画笔的**几何填充**方法获得此结果。
	- 在这个代码示例中，当调用PuthLayer方法时，不会在应用程序创建的层中传递。Direct2D为您创建了一个层。Direct2D能够管理这个资源的分配和销毁，而不需要任何应用程序的参与。
	- 这允许Douth2D在内部重用层并应用资源管理优化。在Windows 8中，对层的使用进行了许多优化，我们建议您尽可能尝试使用层API而不是FillGeometry。
	- PushLayer in Windows 8
	- Axis-aligned clips
		- 如果要裁剪的区域与绘图表面的轴对齐，而不是任意的。这种情况适用于使用剪辑矩形而不是图层。性能增益更多的是别名几何比反锯齿几何
- DXGI interoperability: avoid frequent switches
	- Direct2D可以无缝地与Direct3D曲面互操作。这对于创建呈现2D和3D内容的混合的应用程序非常有用。但是，绘图Direct2D和Direct3D内容之间的每个切换影响性能。
	- 当渲染到DXGI表面时，Direct2D在渲染时保存Direct3D设备的状态，并在完成渲染时恢复它。每次完成一批Direct2D渲染时，都会支付保存和恢复的成本以及刷新所有2D操作的成本，但是Direct3D设备不会刷新。因此，为了提高性能，限制Direct2D和Direct3D之间的渲染开关的数量。
- Know Your Pixel Format
	- 如果渲染目标不使用alpha通道，则应该使用D2D1Y-AlpHaMaMyDead Alpha模式创建。这节省了花费在渲染不需要的alpha通道上的时间。
- Scene Complexity
	- Understand Scene Complexity
	- 填充率：填充率是指图形卡每秒可对视频存储器进行渲染和写入的像素数。
	- 顶点约束：一个场景包含许多复杂的几何图形时是顶点约束的。
	- 你可以通过改变渲染目标的大小来分析场景的复杂性。如果渲染目标的大小按比例缩小，性能增益是可见的，那么应用程序就是填充率界限。否则，场景复杂度是性能瓶颈。
	- 当场景是填充率约束时，减少渲染目标的大小可以提高性能。这是因为要渲染的像素的数量将与渲染目标的大小成比例地减少。
	- 当场景是顶点约束时，减少几何的复杂度。但要记住，这是以牺牲图像质量为代价的。因此，应该在所需质量和所需性能之间进行仔细权衡决定。
- Improving performance for Direct2D printing apps
	- Set the right property values when you create the D2D print control
		- 字体子集模式：DistT2D打印控件在发送要打印的页面之前，在每个页面中设置字体资源。这种模式减少了打印所需的页面资源的大小。
		- 栅格化DPI：当Direct2D打印控件在Direct2D-XPS转换期间需要栅格化Direct2D命令时，它使用这个DPI进行栅格化。换句话说，如果页面没有任何栅格化内容，设置任何DPI不会改变性能和质量。根据页面上的光栅化使用情况，您可以选择不同的光栅化DPI，以实现保真度和性能之间的最佳平衡。
	- Avoid using certain Direct2D drawing patterns
		- 避免使用原始混合模式，而不是D2D1PrimiType BLDENN源代码。
		- 避免绘制GDI元文件。
		- 避免推动复制源背景的层资源（调用PushLayer，将D2D1_LAYER_OPTIONS1_INITIALIZE_FROM_BACKGROUND传递给D2D1_LAYER_PARAMETERS1结构）。
		- 避免使用D2D1ExExpDyMoDead钳位创建位图刷或图像刷。如果不关心图像边界之外的像素，我们建议您使用D2D1_EXTEND_MODE_MIRROR（例如，已知附在笔刷上的图像大于填充的目标区域）。
		- 避免使用透视变换绘制位图。
	- Draw text in a direct and plain way
		- 当呈现文本以显示更好的性能和/或更好的视觉质量时，DICT2D具有多种优化。但是，并非所有的优化都提高了打印性能和质量，因为纸上的打印通常具有更高的DPI，并且打印不需要适应诸如动画之类的场景。因此，我们建议您直接绘制原始文本或字形，并在创建打印命令列表时避免进行以下任何优化。
		- Avoid drawing text with the FillOpacityMask method.
		- Avoid drawing text in Aliased Mode.
	- Draw the original bitmaps when possible

## Performance Tunning
- [Debugging Tips for the Windows Display Driver Model](https://docs.microsoft.com/en-us/windows-hardware/drivers/display/debugging-tips-for-the-windows-vista-display-driver-model)
- Tools: [Sysinternals](https://docs.microsoft.com/en-us/sysinternals) (Procmon / ProcExp / Handle, etc)
- [Profiling native C++ rendering apps](https://docs.microsoft.com/en-us/windows/desktop/direct2d/profiling-directx-applications)
- [Xperf](https://blogs.msdn.microsoft.com/ntdebugging/2008/04/03/windows-performance-toolkit-xperf/) 
	- [Profiling with Xperf](https://developer.mozilla.org/en-US/docs/Mozilla/Performance/Profiling_with_Xperf)
	- 收集步骤
		- This log is quite large with process details. we must keep the log running within 2~3 minutes, otherwise the log would be too large to open
		- Please don’t collect this log while we collecting Procmon log
		- Please download Xperf_x64.zip from the workspace and extract to the affected machine。
		- When CPU usage is high, please Right-click cmd, choose Run as administrator, run the following command: `Xperf.exe -on latency -stackwalk profile` 
		- open notepad.exe and close it for 3 times
		- After 3 minutes, please run the following command to stop it(this may take some time): `xperf.exe -stop -d merged.etl`
		- zip merged.etl to the workspace for analysis.
- [GPUView](https://docs.microsoft.com/en-us/windows-hardware/drivers/display/using-gpuview)
	- [graphics-profiling](https://stackoverflow.com/questions/852254/graphics-profiling)
	- [nvperfhud](http://developer.nvidia.com/object/nvperfhud_home.html)
	- [gpu-perfstudio](http://developer.amd.com/tools-and-sdks/graphics-development/gpu-perfstudio/)
- [UIforETW](https://github.com/google/UIforETW/releases)
	