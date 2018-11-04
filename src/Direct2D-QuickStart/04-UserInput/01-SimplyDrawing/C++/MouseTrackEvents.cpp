#include <Windows.h>

#include "MousetrackEvents.h"

void MouseTrackEvents::OnMouseMove(HWND hwnd)
{
    if (!m_bMouseTracking)
    {
        // Enable mouse tracking.
        TRACKMOUSEEVENT tme;
        tme.cbSize = sizeof(tme);
        tme.hwndTrack = hwnd;
        tme.dwFlags = TME_HOVER | TME_LEAVE;
        tme.dwHoverTime = HOVER_DEFAULT;
        TrackMouseEvent(&tme);
        m_bMouseTracking = true;
    }
}
void MouseTrackEvents::Reset(HWND hwnd)
{
    m_bMouseTracking = false;
}
