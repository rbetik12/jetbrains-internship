#pragma once
#include <d2d1.h>
#include <atlbase.h>
#include <atlcomcli.h>
#include <wrl.h>

class TriangleGenerator {
public:
    static Microsoft::WRL::ComPtr<ID2D1PathGeometry> Generate(D2D1_POINT_2F pt1, D2D1_POINT_2F pt2, D2D1_POINT_2F pt3);
};