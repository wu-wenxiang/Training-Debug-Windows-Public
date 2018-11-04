#include <shobjidl.h> 
#include <atlbase.h>

#include "MenuWindow.h"

#include "MainWindow.h"
#include "GraphicsWindow.h"
#include "OpenGLWindow.h"

template<class T> void SafeRelease(T **ppT)
{
    if (*ppT)
    {
        (*ppT)->Release();
        *ppT = NULL;
    }
}

// Override the virtual class name for use in creation
PCWSTR  MenuWindow::ClassName() const { return L"Learning C++"; }

// Handle messages from the window (except for create which is handled by base)
LRESULT MenuWindow::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
    {
		case WM_CREATE: 
			{
				button = CreateWindow(L"button", L"File picker and Coloured Window", WS_CHILD, 0, 0, 300, 100, m_hwnd, NULL,::GetModuleHandle(NULL),0);
				ShowWindow(button, SW_SHOW);

				clockButton = CreateWindow(L"button", L"Clock Window", WS_CHILD, 300, 0, 200, 100, m_hwnd, NULL,::GetModuleHandle(NULL),0);
				ShowWindow(clockButton, SW_SHOW);

				openGLButton = CreateWindow(L"button", L"OpenGL Window", WS_CHILD, 500, 0, 200, 100, m_hwnd, NULL,::GetModuleHandle(NULL),0);
				ShowWindow(openGLButton, SW_SHOW);
			}
			break;
		case WM_DESTROY:
			PostQuitMessage(0); // Add message to the queue to quit the application (return a 0 response from GetMessage)
			return 0;
		case WM_COMMAND:
			{
				if(button == (HWND)lParam)
				{
					if (!mainWindow.Create(L"Resize to see the update area change colour", WS_OVERLAPPEDWINDOW, m_hwnd))
					{
						return 0;
					}

					ShowWindow(mainWindow.Window(), SW_SHOW);
				}
				else if(clockButton == (HWND)lParam)
				{
					if (!graphicsWindow.Create(L"F1 to enter drawing mode, F2 for selection mode", WS_OVERLAPPEDWINDOW, m_hwnd))
					{
						return 0;
					}

					ShowWindow(graphicsWindow.Window(), SW_SHOW);
				}
				else if(openGLButton == (HWND)lParam)
				{
					if (!openGLWindow.Create(L"Open GL Demo", WS_OVERLAPPEDWINDOW, m_hwnd))
					{
						return 0;
					}

					ShowWindow(openGLWindow.Window(), SW_SHOW);
				}
			}
			break;
    }
    return DefWindowProc(m_hwnd, uMsg, wParam, lParam); // Do the default action
}