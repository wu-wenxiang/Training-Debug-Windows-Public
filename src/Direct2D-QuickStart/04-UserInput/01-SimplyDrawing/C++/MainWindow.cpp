#include <shobjidl.h> 
#include <atlbase.h>

#include "MainWindow.h"

template<class T> void SafeRelease(T **ppT)
{
    if (*ppT)
    {
        (*ppT)->Release();
        *ppT = NULL;
    }
}

// Start with black as the colour
MainWindow::MainWindow() : red(0), green(0), blue(0) { }

// Override the virtual class name for use in creation
PCWSTR  MainWindow::ClassName() const { return L"Sample Window Class"; }

// Handle messages from the window (except for create which is handled by base)
LRESULT MainWindow::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
    {
		case WM_SIZE: // Example of using the size event.  lParam contains 2 16bit numbers packed into one 32
			{
				int width = LOWORD(lParam);  // Macro to get the low-order word.
				int height = HIWORD(lParam); // Macro to get the high-order word.

				// Respond to the message:
				OnSize(m_hwnd, (UINT)wParam, width, height);
			}
        break;

		case WM_PAINT:
			{
				PAINTSTRUCT ps;
				HDC hdc = BeginPaint(m_hwnd, &ps);

				// GDI function
				FillRect(hdc, &ps.rcPaint /* Current update region */, CreateSolidBrush(RGB(red, green, blue)));

				EndPaint(m_hwnd, &ps);
			}
			return 0;
		case WM_SHOWWINDOW: // Closest thing to 'on load' I could find :)
			{
				bool showing = wParam != 0;
				if(showing)
				{
					// Initialize the COM library in apartment threaded with OLE disabled
					HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
					if (SUCCEEDED(hr))
					{
						// We want to populate an IFileOpenDialog from COM
						CComPtr<IFileOpenDialog> pFileOpen;

						// Create the FileOpenDialog object and populate our interface pointer
						hr = pFileOpen.CoCreateInstance(__uuidof(FileOpenDialog));

						if (SUCCEEDED(hr))
						{
							// Show the Open dialog box.
							hr = pFileOpen->Show(NULL);

							// Get the file name from the dialog box.
							if (SUCCEEDED(hr))
							{
								// Store the chosen file in a shell item interface pointer
								CComPtr<IShellItem> pItem;
								hr = pFileOpen->GetResult(&pItem);
								if (SUCCEEDED(hr))
								{
									PWSTR pszFilePath;
									hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

									// Display the file name to the user.
									if (SUCCEEDED(hr))
									{
										MessageBox(NULL, pszFilePath, L"File Path", MB_OK);
										CoTaskMemFree(pszFilePath);
									}
									// pItem goes out of scope and releases self
								}
							}
							// pFileOpen goes out of scope and releases self
						}
						CoUninitialize();
					}
				}
				//PostMessage(m_hwnd, WM_CLOSE, 0, 0);
			}
			break;
    }
    return DefWindowProc(m_hwnd, uMsg, wParam, lParam); // Do the default action
}

// Change our colour (just to see a visual difference on screen)
void MainWindow::OnSize(HWND hwnd, UINT flag, int width, int height)
{
	if(red < 255)
		red++;
	else
		red = 0;
			
	if(green < 255)
		green++;
	else
		green = 0;

	if(blue < 255)
		blue++;
	else
		blue = 0;
}