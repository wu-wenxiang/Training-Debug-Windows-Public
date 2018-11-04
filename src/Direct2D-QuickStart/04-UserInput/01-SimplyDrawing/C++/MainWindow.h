#pragma once

#include "BaseWindow.h"

class MainWindow : public BaseWindow<MainWindow>
{
public:
	MainWindow();

	// We will override the base methods for handling messages and setting our window text
    PCWSTR  ClassName() const;
    LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

	// Main window specific functionality
	void OnSize(HWND hwnd, UINT flag, int width, int height);

	int red;
	int green;
	int blue;
};