#pragma once

#include "BaseWindow.h"
#include "MainWindow.h"
#include "GraphicsWindow.h"
#include "OpenGLWindow.h"

class MenuWindow : public BaseWindow<MenuWindow>
{
	HWND button;
	HWND clockButton;
	HWND openGLButton;

	MainWindow mainWindow;
	GraphicsWindow graphicsWindow;
	OpenGLWindow openGLWindow;

public:
    PCWSTR  ClassName() const;
    LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

	MenuWindow() : button(NULL), clockButton(NULL), openGLButton(NULL) {}
};