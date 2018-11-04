#pragma once

#include <Windows.h>

class MouseTrackEvents
{
    bool m_bMouseTracking;

public:
    MouseTrackEvents() : m_bMouseTracking(false)
    {
    }
    
    void OnMouseMove(HWND hwnd);
    void Reset(HWND hwnd);
};