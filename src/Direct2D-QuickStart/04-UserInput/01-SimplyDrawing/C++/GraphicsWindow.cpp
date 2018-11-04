#include <windows.h>
#include <WindowsX.h>
#include <d2d1.h> // Contains helper functions/classes such as ColorF, Matrix3x2F for transforms and initializers for Direct2D structures

#include "GraphicsWindow.h"
#include "DPIScale.h"
#include "resource.h"

D2D1::ColorF::Enum colors[] = { D2D1::ColorF::Yellow, D2D1::ColorF::Salmon, D2D1::ColorF::LimeGreen };

template <class T> void SafeRelease(T **ppT)
{
    if (*ppT)
    {
        (*ppT)->Release();
        *ppT = NULL;
    }
}

HRESULT GraphicsWindow::CreateGraphicsResources()
{
    HRESULT hr = S_OK;

	// Only create once (due to expense) at the beginning and then if resources are discarded due to unavailabilty of device
    if (pRenderTarget == NULL)
    {
        RECT rc;
        GetClientRect(m_hwnd, &rc); // Returned in pixels not Device Independent Pixels (DIPs)

        D2D1_SIZE_U size = D2D1::SizeU(rc.right /* Width */, rc.bottom /* Height */);

		// Get the render target for this window
        hr = pFactory->CreateHwndRenderTarget(
            D2D1::RenderTargetProperties(), // Defaultoptions
            D2D1::HwndRenderTargetProperties(m_hwnd, size),
            &pRenderTarget);

        if (SUCCEEDED(hr))
        {
            const D2D1_COLOR_F color = D2D1::ColorF(1.0f, 1.0f, 0);
            hr = pRenderTarget->CreateSolidColorBrush(color, &pBrush); // Setup the member brush for usage

			if (!SUCCEEDED(hr))
				return E_FAIL;
        }
    }
    return hr;
}

void GraphicsWindow::DiscardGraphicsResources()
{
	// Discard the target and the brush (as the brush is target dependent)
    SafeRelease(&pRenderTarget);
    SafeRelease(&pBrush);
}

// Draw the clock face based upon a passed in ellipse rather than a stored item
void GraphicsWindow::DrawClockHand(std::shared_ptr<DrawableEllipse> clockface, float fHandLength, float fAngle, float fStrokeWidth)
{
    pRenderTarget->SetTransform(
        D2D1::Matrix3x2F::Rotation(fAngle, clockface->ellipse.point)
            );

    // endPoint defines one end of the hand.
    D2D_POINT_2F endPoint = D2D1::Point2F(
        clockface->ellipse.point.x,
        clockface->ellipse.point.y - (clockface->ellipse.radiusY * fHandLength)
        );

    // Draw a line from the center of the ellipse to endPoint.
    pRenderTarget->DrawLine(
        clockface->ellipse.point, endPoint, pBrush, fStrokeWidth);
}


void GraphicsWindow::OnPaint()
{
    HRESULT hr = CreateGraphicsResources();
    if (SUCCEEDED(hr))
    {
        PAINTSTRUCT ps;
        BeginPaint(m_hwnd, &ps);

        pRenderTarget->BeginDraw();

        pRenderTarget->Clear( D2D1::ColorF(D2D1::ColorF::SkyBlue) ); // Clear the device to blue

		// Draw hands
		SYSTEMTIME time;
		GetLocalTime(&time);

		// 60 minutes = 30 degrees, 1 minute = 0.5 degree
		const float fHourAngle = (360.0f / 12) * (time.wHour) + (time.wMinute * 0.5f); 
		const float fMinuteAngle =(360.0f / 60) * (time.wMinute);
		const float fSecondAngle = (360.0f / 60) * (time.wSecond);

		// Draw all stored shapes
        for (auto i = ellipses.begin(); i != ellipses.end(); ++i)
        {
            (*i)->Draw(pRenderTarget, pBrush);

			// Outline the selected item inside the loop to ensure it is at the right depth
			if (Selection() == *i)
			{
				pBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Red)); // SetColor is a non-expensive operation
				pRenderTarget->DrawEllipse(Selection()->ellipse, pBrush, 2.0f);
			}

			// Add clock hands to the ellipses, because.. why not
			pBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Black));
			DrawClockHand(*i, 0.6f,  fHourAngle,   6);
			DrawClockHand(*i, 0.85f, fMinuteAngle, 4);
			DrawClockHand(*i, 0.85f, fSecondAngle, 2);
			
			// Restore the identity transformation for the next ellipse after rotating the clock hands
			pRenderTarget->SetTransform( D2D1::Matrix3x2F::Identity() );
        }

        hr = pRenderTarget->EndDraw(); // If an error occurs during any of the draw commands (begin, clear, fillellipse) it is returned here
        if (FAILED(hr) || hr == D2DERR_RECREATE_TARGET)
        {
			// If we failed for some reason, including if the device has become unavailable and we need to recreate then discard current resources
            DiscardGraphicsResources();
        }
        EndPaint(m_hwnd, &ps);
    }
}

void GraphicsWindow::Resize()
{
    if (pRenderTarget != NULL)
    {
        RECT rc;
        GetClientRect(m_hwnd, &rc);

        D2D1_SIZE_U size = D2D1::SizeU(rc.right, rc.bottom);

        pRenderTarget->Resize(size); // Update the render target to match the window
        InvalidateRect(m_hwnd, NULL, FALSE);
    }
}

// When we change mode change the current cursor and save what it is (for mouse off / back onto our window in WM_SETCURSOR)
void GraphicsWindow::SetMode(Mode m)
{
    mode = m;

    // Update the cursor
    LPWSTR cursor;
    switch (mode)
    {
    case DrawMode:
        cursor = IDC_CROSS;
        break;

    case SelectMode:
        cursor = IDC_HAND;
        break;

    case DragMode:
        cursor = IDC_SIZEALL;
        break;
    }
	// Save current choice in member variable
    hCursor = LoadCursor(NULL, cursor);
    SetCursor(hCursor);
}

std::shared_ptr<DrawableEllipse> GraphicsWindow::Selection() 
{ 
    if (selection == ellipses.end()) 
    { 
        return nullptr;
    }
    else
    {
        return (*selection);
    }
}

void GraphicsWindow::ClearSelection() { selection = ellipses.end(); }

LRESULT GraphicsWindow::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_CREATE:
		// Should be created before the first WM_PAINT message
        if (FAILED(D2D1CreateFactory(
                D2D1_FACTORY_TYPE_SINGLE_THREADED, &pFactory)))
        {
            return -1;  // Fail CreateWindowEx.
        }
		DPIScale::Initialize(pFactory);
		SetTimer(m_hwnd, m_nTimerID, 1000, NULL);
        return 0;

    case WM_DESTROY:
		KillTimer(m_hwnd, m_nTimerID);
        DiscardGraphicsResources();
        SafeRelease(&pFactory);
        break;
	case WM_TIMER:
		InvalidateRect(m_hwnd, NULL, FALSE); // allow the clock faces to cycle the second hand
		break;
    case WM_PAINT:
        OnPaint();
        return 0;

	case WM_LBUTTONDOWN: 
        OnLButtonDown(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), (DWORD)wParam);
        return 0;

    case WM_LBUTTONUP: 
        OnLButtonUp();
        return 0;
		
    case WM_MOUSEMOVE: 
        OnMouseMove(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), (DWORD)wParam);
        return 0;

    case WM_SIZE:
        Resize();
        return 0;

	case WM_COMMAND: // We've received an accelerator command
        switch (LOWORD(wParam))
        {
			case ID_DRAW_MODE:
				SetMode(DrawMode);
				break;

			case ID_SELECT_MODE:
				SetMode(SelectMode);
				break;

			case ID_TOGGLE_MODE:
				if (mode == DrawMode)
				{
					SetMode(SelectMode);
				}
				else
				{
					SetMode(DrawMode);
				}
				break;
			case ID_MOVE_UP:
				MoveSelectionUp();
				break;
			case ID_MOVE_DOWN:
				MoveSelectionDown();
				break;
			case ID_DELETE:
				DeleteSelection();
				break;
        }
        return 0;
	case WM_SETCURSOR:
		// The mouse is back on our window, maintain the cursor display we had before
        if (LOWORD(lParam) == HTCLIENT)
        {
            SetCursor(hCursor);
            return TRUE;
        }
        break;
    }
    return DefWindowProc(m_hwnd, uMsg, wParam, lParam);
}

void GraphicsWindow::OnLButtonDown(int pixelX, int pixelY, DWORD flags)
{
	const float dipX = DPIScale::PixelsToDipsX(pixelX);
    const float dipY = DPIScale::PixelsToDipsY(pixelY);
    
	if (mode == DrawMode)
    {
        POINT pt = { pixelX, pixelY };

        if (DragDetect(m_hwnd, pt))
        {
            SetCapture(m_hwnd);
        
            // Start a new ellipse - OnMove will continually readjust it
            InsertEllipse(dipX, dipY);
        }
    }
    else
    {
        ClearSelection();

        if (HitTest(dipX, dipY))
        {
			// If we selected and item start capturing drag
            SetCapture(m_hwnd);

			// Set a local mouse point to the delta of ellipse and actual click location
            ptMouse = Selection()->ellipse.point;
            ptMouse.x -= dipX;
            ptMouse.y -= dipY;

            SetMode(DragMode);
        }
    }

	// Force a repaint
    InvalidateRect(m_hwnd, NULL, FALSE);
}

void GraphicsWindow::OnMouseMove(int pixelX, int pixelY, DWORD flags)
{
	const float dipX = DPIScale::PixelsToDipsX(pixelX);
    const float dipY = DPIScale::PixelsToDipsY(pixelY);

	// If we're holding the left button and something is selected then manipulate it
    if ((flags & MK_LBUTTON) && Selection())
    { 
        if (mode == DrawMode)
        {
            // Resize the currently being drawn ellipse.
            const float width = (dipX - ptMouse.x) / 2;
            const float height = (dipY - ptMouse.y) / 2;
            const float x1 = ptMouse.x + width;
            const float y1 = ptMouse.y + height;

            Selection()->ellipse = D2D1::Ellipse(D2D1::Point2F(x1, y1), width, height);
        }
        else if (mode == DragMode)
        {
            // Move the selected ellipse by the position + the delta of where you first clicked on the ellipse
			// This avoids the ellipse center snapping to the mouse location
            Selection()->ellipse.point.x = dipX + ptMouse.x;
            Selection()->ellipse.point.y = dipY + ptMouse.y;
        }
        InvalidateRect(m_hwnd, NULL, FALSE);
    }
}

HRESULT GraphicsWindow::InsertEllipse(float x, float y)
{
    try
    {
		// Add a new ellipses to the end of the list
        selection = ellipses.insert(
            ellipses.end(), 
            std::shared_ptr<DrawableEllipse>(new DrawableEllipse()));

        Selection()->ellipse.point = ptMouse = D2D1::Point2F(x, y);
        Selection()->ellipse.radiusX = Selection()->ellipse.radiusY = 2.0f; 
        Selection()->color = D2D1::ColorF( colors[nextColor] );

        nextColor = (nextColor + 1) % ARRAYSIZE(colors);
    }
    catch (std::bad_alloc)
    {
        return E_OUTOFMEMORY;
    }
    return S_OK;
}

void GraphicsWindow::DeleteSelection()
{
	if(Selection()) {
		ellipses.erase(selection);
		ClearSelection();
		InvalidateRect(m_hwnd, NULL, FALSE); // Redraw the scene without this item
	}
}

void GraphicsWindow::MoveSelectionUp()
{
	// End isn't the last element showing, the item before end is so check for that case
	if(Selection() && selection != prev(ellipses.end()) && selection != ellipses.end())
	{
		// Next copies and returns an iterator for the next position without effecting selection
		// Selection is maintained as the list contains the same elements so the iterators are not invalidated
		ellipses.splice(next(next(selection)), ellipses, selection);
	}
	InvalidateRect(m_hwnd, NULL, FALSE); // Redraw the newly layered scene
}

void GraphicsWindow::MoveSelectionDown()
{
	// If we're not the first ellipsis to be drawn then swap us with the one before us
	if(Selection() && selection != ellipses.begin())
	{
		// Prev copies and returns an iterator for the previous position without effecting selection
		// Selection is maintained as the list contains the same elements so the iterators are not invalidated
		ellipses.splice(prev(selection), ellipses, selection);
	}
	InvalidateRect(m_hwnd, NULL, FALSE); // Redraw the newly layered scene
}

BOOL GraphicsWindow::HitTest(float x, float y)
{
	// Start at the end (reverseBegin) to look from the top items down)
	// i is an iterator not the element itself.
    for (auto i = ellipses.rbegin(); i != ellipses.rend(); ++i /* postfix, incremement the iterator and return the new value e.g. skip the end*/)
    {
		// 'dereference' the iterator to return the DrawableEllipse element
        if ((*i)->HitTest(x, y))
        {
			/* Set the selection to the base iterator (not this reversed one).
			   ++ 1 to get the same element rather than the item offset-1.
			*/
            selection = (++i).base();
            return TRUE;
        }
    }
    return FALSE;
}

void GraphicsWindow::OnLButtonUp()
{
	// Stop capturing the mouse outside of the window
    ReleaseCapture(); 
}