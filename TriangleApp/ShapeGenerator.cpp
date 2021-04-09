#include "ShapeGenerator.h"
#include "TriangleApp.h"

using namespace Microsoft::WRL;

Microsoft::WRL::ComPtr<ID2D1PathGeometry> ShapeGenerator::GenerateTriangle(D2D1_POINT_2F pt1, D2D1_POINT_2F pt2, D2D1_POINT_2F pt3) {
    ID2D1GeometrySink* sink = NULL;
    HRESULT hr = S_OK;
    ComPtr<ID2D1PathGeometry> pathGeometry;
    TriangleApp* app = TriangleApp::GetInstance();
    // Create a path geometry.
    if (SUCCEEDED(hr)) {
        
        hr = app->GetFactory()->CreatePathGeometry(pathGeometry.ReleaseAndGetAddressOf());

        if (SUCCEEDED(hr)) {
            // Write to the path geometry using the geometry sink.
            hr = pathGeometry->Open(&sink);

            if (SUCCEEDED(hr)) {
                sink->BeginFigure(
                    pt1,
                    D2D1_FIGURE_BEGIN_FILLED
                );

                sink->AddLine(pt2);


                sink->AddLine(pt3);

                sink->EndFigure(D2D1_FIGURE_END_CLOSED);

                hr = sink->Close();
            }
            SafeRelease(&sink);
        }
    }
    return pathGeometry;
}

Microsoft::WRL::ComPtr<ID2D1StrokeStyle> ShapeGenerator::CreateStrokeStyle() {
    TriangleApp* app = TriangleApp::GetInstance();
    ComPtr<ID2D1StrokeStyle> strokeStyle;

    app->GetFactory()->CreateStrokeStyle(
        D2D1::StrokeStyleProperties(
        D2D1_CAP_STYLE_ROUND,
        D2D1_CAP_STYLE_ROUND,
        D2D1_CAP_STYLE_ROUND,
        D2D1_LINE_JOIN_ROUND,
        0.0f,
        D2D1_DASH_STYLE_SOLID,
        0.0f),
        nullptr,
        0,
        strokeStyle.GetAddressOf()
    );

    return strokeStyle;
}
