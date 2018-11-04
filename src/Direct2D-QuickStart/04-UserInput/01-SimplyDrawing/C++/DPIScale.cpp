#include "DPIScale.h"

void DPIScale::Initialize(ID2D1Factory *pFactory)
{
    FLOAT dpiX, dpiY;
    pFactory->GetDesktopDpi(&dpiX, &dpiY);
    scaleX = dpiX/96.0f;
    scaleY = dpiY/96.0f;
}

float DPIScale::scaleX = 1.0f;
float DPIScale::scaleY = 1.0f;