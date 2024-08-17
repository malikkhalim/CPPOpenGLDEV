#include "../../GfxLib/gfx/GFX.h"
#include <limits> // For numeric limits
#include <iostream>

gfxSphere gfxModel::ComputeModelBVSphere(std::vector<gfxVector3> const &verts)
{
    if (verts.empty())
    {
        std::cerr << "Vertices list is empty, returning default sphere.\n";
        return gfxSphere(0.f, 0.f, 0.f, 1.f);
    }

    // Start with an arbitrary point as the sphere center
    gfxVector3 center = verts[0];
    float radius = 0.0f;

    // Initial rough guess for the sphere using the first point
    for (const auto &v : verts)
    {
        float distance = (v - center).Length();
        if (distance > radius)
        {
            radius = distance;
        }
    }

    // Refine the sphere using Ritter's method
    for (const auto &v : verts)
    {
        gfxVector3 direction = v - center;
        float distance = direction.Length();

        if (distance > radius)
        {
            float new_radius = (radius + distance) / 2;
            float delta_radius = new_radius - radius;
            center += (delta_radius / distance) * direction;
            radius = new_radius;
        }
    }

    std::cout << "Bounding sphere computed with center (" << center.x << ", " << center.y << ", " << center.z << ") and radius " << radius << "\n";
    return gfxSphere(center.x, center.y, center.z, radius);
}

gfxSphere gfxSphere::Transform(gfxMatrix4 const &xform) const
{
    gfxVector3 new_center = xform * center;
    float max_scale = std::max({xform.GetCol3(0).Length(), xform.GetCol3(1).Length(), xform.GetCol3(2).Length()});
    // std::cout << "Transforming sphere to new center (" << new_center.x << ", " << new_center.y << ", " << new_center.z << ") with scaled radius " << radius * max_scale << "\n";
    return gfxSphere(new_center.x, new_center.y, new_center.z, radius * max_scale);
}
