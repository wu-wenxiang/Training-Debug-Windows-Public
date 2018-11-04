#pragma once

#include <d2d1.h>

class DPIScale
{
    static float scaleX;
    static float scaleY;

public:
    static void Initialize(ID2D1Factory *pFactory);

    template <typename T>
    static D2D1_POINT_2F PixelsToDips(T x, T y)
    {
        return D2D1::Point2F(static_cast<float>(x) / scaleX, static_cast<float>(y) / scaleY);
    }

	template <typename T>
	static float PixelsToDipsX(T x)
    {
        return static_cast<float>(x) / scaleX;
    }

	template <typename T>
	static float PixelsToDipsY(T y)
    {
        return static_cast<float>(y) / scaleY;
    }
};