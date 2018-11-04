#include <windows.h>
#include <D2d1.h>
#include <assert.h>

#pragma comment(lib, "d2d1")

#include "basewin.h"

HANDLE g_hTimer = NULL;

BOOL InitializeTimer();

template <class T> void SafeRelease(T **ppT)
{
    if (*ppT)
    {
        (*ppT)->Release();
        *ppT = NULL;
    }
}


/// d2d zola ÆíÇÏ³×... 



class Scene
{
    ID2D1Factory                *m_pFactory;
    ID2D1HwndRenderTarget       *m_pRenderTarget;

    ID2D1RadialGradientBrush    *m_pFill;
    ID2D1SolidColorBrush        *m_pStroke;
    ID2D1GradientStopCollection *m_pGradientStops;

    float                       m_fScaleX;
    float                       m_fScaleY;

    D2D1_ELLIPSE                m_ellipse;
    D2D_POINT_2F                m_Ticks[24];

    HRESULT CreateGraphicsResources(HWND hwnd);
    HRESULT CreateDeviceDependentResources();
    void    DiscardDeviceDependentResources();
    void    CalculateLayout();
    void    RenderScene();

    HRESULT CreateRadialBrush();
    void    DrawClockHand(float fHandLength, float fAngle, float fStrokeWidth);

    template <class T>
    T PixelToDipX(T pixels) const
    {
        return static_cast<T>(pixels / m_fScaleX);
    }

    template <class T>
    T PixelToDipY(T pixels) const
    {
        return static_cast<T>(pixels / m_fScaleY);
    }


public:
    Scene() 
        : m_pFill(NULL), 
          m_pStroke(NULL), 
          m_pGradientStops(NULL), 
          m_pFactory(NULL), 
          m_pRenderTarget(NULL), 
          m_fScaleX(1.0f), 
          m_fScaleY(1.0f)
    {
    }

    ~Scene()
    {
        CleanUp();
    }

    HRESULT Initialize();
    void Render(HWND hwnd);
    HRESULT Resize(int x, int y);

    void CleanUp()
    {
        DiscardDeviceDependentResources();

        // Discard device-independent resources.
        SafeRelease(&m_pGradientStops);
        SafeRelease(&m_pFactory);
    }
};


HRESULT Scene::Initialize()
{
    return D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, 
        &m_pFactory);
}

void Scene::Render(HWND hwnd)
{
    HRESULT hr = CreateGraphicsResources(hwnd);
    if (FAILED(hr))
    {
        return;
    }

    assert(m_pRenderTarget != NULL);

    m_pRenderTarget->BeginDraw();

    RenderScene();

    hr = m_pRenderTarget->EndDraw();
    if (hr == D2DERR_RECREATE_TARGET)
    {
        DiscardDeviceDependentResources();
    }
}

HRESULT Scene::Resize(int x, int y)
{
    HRESULT hr = S_OK;
    if (m_pRenderTarget)
    {
        hr = m_pRenderTarget->Resize( D2D1::SizeU(x, y) );
        if (SUCCEEDED(hr))
        {
            CalculateLayout();
        }
    }
    return hr;
}


HRESULT Scene::CreateGraphicsResources(HWND hwnd)
{
    HRESULT hr = S_OK;
    if (m_pRenderTarget == NULL)
    {
        RECT rc;
        GetClientRect(hwnd, &rc);

        D2D1_SIZE_U size = D2D1::SizeU(rc.right, rc.bottom);

        hr = m_pFactory->CreateHwndRenderTarget(
            D2D1::RenderTargetProperties(),
            D2D1::HwndRenderTargetProperties(hwnd, size),
            &m_pRenderTarget
            );

        if (SUCCEEDED(hr))
        {
            hr = CreateDeviceDependentResources();
        }
        if (SUCCEEDED(hr))
        {
            CalculateLayout();
        }
    }
    return hr;
}


HRESULT Scene::CreateDeviceDependentResources()
{
    return m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(0, 0, 0), &m_pStroke);
}


HRESULT Scene::CreateRadialBrush()
{
    SafeRelease(&m_pGradientStops);
    SafeRelease(&m_pFill);

    // Define the gradient stops for the brush.
    D2D1_GRADIENT_STOP gradientStops[2];
    gradientStops[0].color = D2D1::ColorF(D2D1::ColorF::Yellow, 1);
    gradientStops[0].position = 0.0f;
    gradientStops[1].color = D2D1::ColorF(D2D1::ColorF::ForestGreen, 1);
    gradientStops[1].position = 1.0f;

    // Create the gradient stop collection.
    HRESULT hr = m_pRenderTarget->CreateGradientStopCollection(
        gradientStops,          // Array of gradient stops
        2,                      // Number of elements in the array.
        D2D1_GAMMA_2_2,
        D2D1_EXTEND_MODE_CLAMP,
        &m_pGradientStops
        );

    // Create the brush. The radial gradient is centered on the clock face.
    if (SUCCEEDED(hr))
    {
        hr = m_pRenderTarget->CreateRadialGradientBrush(
            D2D1::RadialGradientBrushProperties(
                m_ellipse.point,        // center of gradient
                D2D1::Point2F(0, 0),    // offset
                m_ellipse.radiusX,
                m_ellipse.radiusY),
            m_pGradientStops,           // Pointer to the gradient stops collection.
            &m_pFill);
    }
    return hr;
}



void Scene::DrawClockHand(float fHandLength, float fAngle, float fStrokeWidth)
{
    m_pRenderTarget->SetTransform(
        D2D1::Matrix3x2F::Rotation(fAngle, m_ellipse.point)
            );

    // endPoint defines one end of the hand.
    D2D_POINT_2F endPoint = D2D1::Point2F(
        m_ellipse.point.x,
        m_ellipse.point.y - (m_ellipse.radiusY * fHandLength)
        );

    // Draw a line from the center of the ellipse to endPoint.
    m_pRenderTarget->DrawLine(
        m_ellipse.point, endPoint, m_pStroke, fStrokeWidth);
}

void Scene::RenderScene()
{
    m_pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::SkyBlue));

    m_pRenderTarget->FillEllipse(m_ellipse, m_pFill);
    m_pRenderTarget->DrawEllipse(m_ellipse, m_pStroke);

    // Draw tick marks
    for (DWORD i = 0; i < 12; i++)
    {
        m_pRenderTarget->DrawLine(m_Ticks[i*2], m_Ticks[i*2+1], m_pStroke, 2.0f);
    }

    // Draw hands
    SYSTEMTIME time;
    GetLocalTime(&time);

    // 60 minutes = 30 degrees, 1 minute = 0.5 degree
    const float fHourAngle = (360.0f / 12) * (time.wHour) + (time.wMinute * 0.5f); 
    const float fMinuteAngle =(360.0f / 60) * (time.wMinute);
    const float fSecondAngle = 
        (360.0f / 60) * (time.wSecond) + (360.0f / 60000) * (time.wMilliseconds);

    DrawClockHand(0.6f,  fHourAngle,   6);
    DrawClockHand(0.85f, fMinuteAngle, 4);
    DrawClockHand(0.85f, fSecondAngle, 1);

    // Restore the identity transformation.
    m_pRenderTarget->SetTransform( D2D1::Matrix3x2F::Identity() );
}

void Scene::CalculateLayout()
{
    D2D1_SIZE_F fSize = m_pRenderTarget->GetSize();

    const float x = fSize.width / 2.0f;
    const float y = fSize.height / 2.0f;
    const float radius = min(x, y);

    m_ellipse = D2D1::Ellipse( D2D1::Point2F(x, y), radius, radius);

    // Calculate tick marks.

    D2D_POINT_2F pt1 = D2D1::Point2F(
        m_ellipse.point.x,
        m_ellipse.point.y - (m_ellipse.radiusY * 0.9f)
        );

    D2D_POINT_2F pt2 = D2D1::Point2F(
        m_ellipse.point.x,
        m_ellipse.point.y - m_ellipse.radiusY
        );

    for (DWORD i = 0; i < 12; i++)
    {
        D2D1::Matrix3x2F mat = D2D1::Matrix3x2F::Rotation(
            (360.0f / 12) * i, m_ellipse.point);

        m_Ticks[i*2] = mat.TransformPoint(pt1);
        m_Ticks[i*2 + 1] = mat.TransformPoint(pt2);
    }

    // Create the radial brush. The radial brush is centered on
    // the clock face, so it must be recreated when the size changes.
    CreateRadialBrush();
}


void Scene::DiscardDeviceDependentResources()
{
    SafeRelease(&m_pFill);
    SafeRelease(&m_pStroke);
    SafeRelease(&m_pRenderTarget);
}


class MainWindow : public BaseWindow<MainWindow>
{
    Scene   m_scene;

public:

    PCWSTR  ClassName() const { return L"Clock Window Class"; }
    LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
};

// Constants 
const WCHAR WINDOW_NAME[] = L"Analog Clock";


INT WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR, INT nCmdShow)
{
    if (FAILED(CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE)))
    {
        return 0;
    }
    if (!InitializeTimer())
    {
        CoUninitialize();
        return 0;
    }
    
    MainWindow win;

    if (!win.Create(WINDOW_NAME, WS_OVERLAPPEDWINDOW))
    {
        return 0;
    }

    ShowWindow(win.Window(), nCmdShow);

    // Run the message loop.

    MSG msg = { };
    while (msg.message != WM_QUIT)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
            continue;
        }
        // Wait until the timer expires or any message is posted.
        if (MsgWaitForMultipleObjects(1, &g_hTimer, FALSE, INFINITE, QS_ALLINPUT) 
                == WAIT_OBJECT_0)
        {
            InvalidateRect(win.Window(), NULL, FALSE);
        }
    }


    CloseHandle(g_hTimer);
    CoUninitialize();
    return 0;
}

LRESULT MainWindow::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    HWND hwnd = m_hwnd;

    switch (uMsg)
    {
    case WM_CREATE:
        if (FAILED(m_scene.Initialize()))
        {
            return -1;
        }
        return 0;

    case WM_DESTROY:
        m_scene.CleanUp();
        PostQuitMessage(0);
        return 0;

    case WM_PAINT:
    case WM_DISPLAYCHANGE:
        {
            PAINTSTRUCT ps;
            BeginPaint(m_hwnd, &ps);
            m_scene.Render(m_hwnd);
            EndPaint(m_hwnd, &ps);
        }
        return 0;

    case WM_SIZE:
        {
            int x = (int)(short)LOWORD(lParam);
            int y = (int)(short)HIWORD(lParam);
            m_scene.Resize(x,y);
            InvalidateRect(m_hwnd, NULL, FALSE);
        }
        return 0;

    case WM_ERASEBKGND:
        return 1;

    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
}


BOOL InitializeTimer()
{
    g_hTimer = CreateWaitableTimer(NULL, FALSE, NULL);
    if (g_hTimer == NULL)
    {
        return FALSE;
    }

    LARGE_INTEGER li = {0};

    if (!SetWaitableTimer(g_hTimer, &li, (1000/60), NULL, NULL,FALSE))
    {
        CloseHandle(g_hTimer);
        g_hTimer = NULL;
        return FALSE;
    }

    return TRUE;
}

