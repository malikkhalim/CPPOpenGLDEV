#include "YourRasterizer.h"
#include "../../GfxLib/gfx/GraphicsPipe.h"
#include <iostream>

void YourRasterizer::DrawPoint(gfxGraphicsPipe *dev, const gfxVertex &v0)
{
    if (!dev)
        return;

    unsigned int *frameBuffer = dev->GetFrameBuffer();
    size_t width = dev->GetWidth();
    size_t height = dev->GetHeight();

    int x = static_cast<int>(v0.x_d);
    int y = static_cast<int>(v0.y_d);

    if (x >= 0 && x < width && y >= 0 && y < height)
    {
        frameBuffer[y * width + x] = 0xFFFFFF; // White color
        std::cout << "Drawing point at (" << x << ", " << y << ")\n";
    }
}

void YourRasterizer::DrawLine(gfxGraphicsPipe *dev, const gfxVertex &v0, const gfxVertex &v1)
{
    if (!dev)
        return;

    unsigned int *frameBuffer = dev->GetFrameBuffer();
    size_t width = dev->GetWidth();
    size_t height = dev->GetHeight();

    int x0 = static_cast<int>(v0.x_d);
    int y0 = static_cast<int>(v0.y_d);
    int x1 = static_cast<int>(v1.x_d);
    int y1 = static_cast<int>(v1.y_d);

    int dx = std::abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
    int dy = -std::abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
    int err = dx + dy, e2;

    while (true)
    {
        if (x0 >= 0 && x0 < width && y0 >= 0 && y0 < height)
        {
            frameBuffer[y0 * width + x0] = 0xFFFFFF; // White color
            std::cout << "Drawing line point at (" << x0 << ", " << y0 << ")\n";
        }
        if (x0 == x1 && y0 == y1)
            break;
        e2 = 2 * err;
        if (e2 >= dy)
        {
            err += dy;
            x0 += sx;
        }
        if (e2 <= dx)
        {
            err += dx;
            y0 += sy;
        }
    }
}

void YourRasterizer::DrawWireframe(gfxGraphicsPipe *dev, const gfxVertex &v0, const gfxVertex &v1, const gfxVertex &v2)
{
    if (!dev)
        return;

    DrawLine(dev, v0, v1);
    DrawLine(dev, v1, v2);
    DrawLine(dev, v2, v0);
}

void YourRasterizer::DrawFilled(gfxGraphicsPipe *dev, const gfxVertex &v0, const gfxVertex &v1, const gfxVertex &v2)
{
    if (!dev)
        return;

    unsigned int *frameBuffer = dev->GetFrameBuffer();
    float *depthBuffer = dev->GetDepthBuffer();
    size_t width = dev->GetWidth();
    size_t height = dev->GetHeight();

    auto edgeFunction = [](const gfxVertex &a, const gfxVertex &b, const gfxVertex &c)
    {
        return (c.x_d - a.x_d) * (b.y_d - a.y_d) - (c.y_d - a.y_d) * (b.x_d - a.x_d);
    };

    float area = edgeFunction(v0, v1, v2);
    if (area == 0)
        return; // Degenerate triangle

    int minX = static_cast<int>(std::max(0.0f, std::floor(std::min({v0.x_d, v1.x_d, v2.x_d}))));
    int maxX = static_cast<int>(std::min(static_cast<float>(width - 1), std::ceil(std::max({v0.x_d, v1.x_d, v2.x_d}))));
    int minY = static_cast<int>(std::max(0.0f, std::floor(std::min({v0.y_d, v1.y_d, v2.y_d}))));
    int maxY = static_cast<int>(std::min(static_cast<float>(height - 1), std::ceil(std::max({v0.y_d, v1.y_d, v2.y_d}))));

    for (int y = minY; y <= maxY; ++y)
    {
        for (int x = minX; x <= maxX; ++x)
        {
            gfxVertex p;
            p.x_d = static_cast<float>(x);
            p.y_d = static_cast<float>(y);

            float w0 = edgeFunction(v1, v2, p);
            float w1 = edgeFunction(v2, v0, p);
            float w2 = edgeFunction(v0, v1, p);

            if (w0 >= 0 && w1 >= 0 && w2 >= 0)
            {
                w0 /= area;
                w1 /= area;
                w2 /= area;

                float z = w0 * v0.z_d + w1 * v1.z_d + w2 * v2.z_d;
                if (z < depthBuffer[y * width + x])
                {
                    depthBuffer[y * width + x] = z;
                    frameBuffer[y * width + x] = 0xFFFFFF; // White color
                }
            }
        }
    }
}
