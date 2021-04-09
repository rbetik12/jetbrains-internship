#include "TriangleGenerator.h"
#include "TriangleApp.h"

Microsoft::WRL::ComPtr<ID2D1PathGeometry> TriangleGenerator::Generate(D2D1_POINT_2F pt1, D2D1_POINT_2F pt2, D2D1_POINT_2F pt3) {
    ID2D1GeometrySink* sink = NULL;
    HRESULT hr = S_OK;
    Microsoft::WRL::ComPtr<ID2D1PathGeometry> pathGeometry;
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
