#ifndef YOUR_RASTERIZER_H
#define YOUR_RASTERIZER_H

#include "../../GfxLib/gfx/Controller_Rasterization.h"

class YourRasterizer : public gfxController_Rasterization {
public:
    YourRasterizer() = default;
    ~YourRasterizer() override = default;

    void DrawPoint(gfxGraphicsPipe* dev, const gfxVertex& v0) override;
    void DrawLine(gfxGraphicsPipe* dev, const gfxVertex& v0, const gfxVertex& v1) override;
    void DrawWireframe(gfxGraphicsPipe* dev, const gfxVertex& v0, const gfxVertex& v1, const gfxVertex& v2) override;
    void DrawFilled(gfxGraphicsPipe* dev, const gfxVertex& v0, const gfxVertex& v1, const gfxVertex& v2) override;
};

#endif // YOUR_RASTERIZER_H
