#include "YourCullerClipper.h"
#include "../../GfxLib/gfx/Frustum.h"
#include "../../GfxLib/gfx/Sphere.h"
#include "../../GfxLib/gfx/Matrix4.h"
#include "../../GfxLib/gfx/Vector3.h"
#include "../../GfxLib/gfx/Vector4.h"
#include <array>
#include <iostream>

gfxFrustum YourClipper::ComputeFrustum(gfxMatrix4 const& perspective_mtx) {
    gfxFrustum frustum;

    // Define the planes
    frustum.l = gfxPlane(perspective_mtx(3, 0) + perspective_mtx(0, 0),
        perspective_mtx(3, 1) + perspective_mtx(0, 1),
        perspective_mtx(3, 2) + perspective_mtx(0, 2),
        perspective_mtx(3, 3) + perspective_mtx(0, 3));

    frustum.r = gfxPlane(perspective_mtx(3, 0) - perspective_mtx(0, 0),
        perspective_mtx(3, 1) - perspective_mtx(0, 1),
        perspective_mtx(3, 2) - perspective_mtx(0, 2),
        perspective_mtx(3, 3) - perspective_mtx(0, 3));

    frustum.b = gfxPlane(perspective_mtx(3, 0) + perspective_mtx(1, 0),
        perspective_mtx(3, 1) + perspective_mtx(1, 1),
        perspective_mtx(3, 2) + perspective_mtx(1, 2),
        perspective_mtx(3, 3) + perspective_mtx(1, 3));

    frustum.t = gfxPlane(perspective_mtx(3, 0) - perspective_mtx(1, 0),
        perspective_mtx(3, 1) - perspective_mtx(1, 1),
        perspective_mtx(3, 2) - perspective_mtx(1, 2),
        perspective_mtx(3, 3) - perspective_mtx(1, 3));

    frustum.n = gfxPlane(perspective_mtx(3, 0) + perspective_mtx(2, 0),
        perspective_mtx(3, 1) + perspective_mtx(2, 1),
        perspective_mtx(3, 2) + perspective_mtx(2, 2),
        perspective_mtx(3, 3) + perspective_mtx(2, 3));

    frustum.f = gfxPlane(perspective_mtx(3, 0) - perspective_mtx(2, 0),
        perspective_mtx(3, 1) - perspective_mtx(2, 1),
        perspective_mtx(3, 2) - perspective_mtx(2, 2),
        perspective_mtx(3, 3) - perspective_mtx(2, 3));

   
    return frustum;
}

bool YourClipper::Cull(gfxSphere const& bounding_sphere, gfxFrustum const& frustum, gfxOutCode* ptr_outcode) {
    *ptr_outcode = 0;
    bool culled = false;

    // Check each plane of the frustum
    std::array<gfxPlane, 6> planes = { frustum.l, frustum.r, frustum.b, frustum.t, frustum.n, frustum.f };

    gfxVector3 center = bounding_sphere.center;
    float radius = bounding_sphere.radius;

    for (int i = 0; i < 6; ++i) {
        float distance = planes[i].a * center.x + planes[i].b * center.y + planes[i].c * center.z + planes[i].d;
        
        if (distance < -radius) {
            // Sphere is completely outside this plane
            
            culled = true;
            break;
        }
        else if (distance < radius) {
            // Sphere intersects this plane
            *ptr_outcode |= (1 << i);
        }
    }

    return culled;
}

/*  _________________________________________________________________________ */
gfxVertexBuffer YourClipper::Clip(gfxOutCode outcode, gfxVertexBuffer const& vertex_buffer) {
    // Initialize output vertex buffer
    gfxVertexBuffer output_buffer;

    // Store input vertices
    std::vector<gfxVertex> input_vertices(vertex_buffer.begin(), vertex_buffer.end());
    std::vector<gfxVertex> output_vertices;

    // Lambda to perform linear interpolation between vertices
    auto interpolate = [](const gfxVertex& v0, const gfxVertex& v1, float t) {
        gfxVertex result;
        result.x_c = v0.x_c + t * (v1.x_c - v0.x_c);
        result.y_c = v0.y_c + t * (v1.y_c - v0.y_c);
        result.z_c = v0.z_c + t * (v1.z_c - v0.z_c);
        result.w_c = v0.w_c + t * (v1.w_c - v0.w_c);
        result.r = v0.r + t * (v1.r - v0.r);
        result.g = v0.g + t * (v1.g - v0.g);
        result.b = v0.b + t * (v1.b - v0.b);
        result.a = v0.a + t * (v1.a - v0.a);
        for (int i = 0; i < 6; ++i) {
            result.bc[i] = v0.bc[i] + t * (v1.bc[i] - v0.bc[i]);
        }
        result.oc = 0; // Reset outcode
        return result;
        };

    // Helper function to clip against a single plane
    auto clipAgainstPlane = [&](const gfxPlane& plane, const std::vector<gfxVertex>& in_vertices) -> std::vector<gfxVertex> {
        std::vector<gfxVertex> out_vertices;
        for (size_t i = 0; i < in_vertices.size(); ++i) {
            const gfxVertex& current = in_vertices[i];
            const gfxVertex& previous = in_vertices[(i + in_vertices.size() - 1) % in_vertices.size()];

            float current_distance = plane.a * current.x_c + plane.b * current.y_c + plane.c * current.z_c + plane.d * current.w_c;
            float previous_distance = plane.a * previous.x_c + plane.b * previous.y_c + plane.c * previous.z_c + plane.d * previous.w_c;

            bool current_inside = current_distance >= 0;
            bool previous_inside = previous_distance >= 0;

            if (current_inside != previous_inside) {
                float t = previous_distance / (previous_distance - current_distance);
                out_vertices.push_back(interpolate(previous, current, t));
            }

            if (current_inside) {
                out_vertices.push_back(current);
            }
        }
        return out_vertices;
        };

    // Check which planes to clip against based on outcode
    std::vector<gfxPlane> clip_planes;
    if (outcode & GFX_CPLEFT) clip_planes.push_back(frustum.l);
    if (outcode & GFX_CPRIGHT) clip_planes.push_back(frustum.r);
    if (outcode & GFX_CPBOTTOM) clip_planes.push_back(frustum.b);
    if (outcode & GFX_CPTOP) clip_planes.push_back(frustum.t);
    if (outcode & GFX_CPNEAR) clip_planes.push_back(frustum.n);
    if (outcode & GFX_CPFAR) clip_planes.push_back(frustum.f);

    // Perform clipping against each plane
    for (const gfxPlane& plane : clip_planes) {
        output_vertices = clipAgainstPlane(plane, input_vertices);
        input_vertices = output_vertices; // Set input vertices for next plane
    }

    // Populate output buffer with clipped vertices
    for (const gfxVertex& v : output_vertices) {
        output_buffer.push_back(v);
    }

    return output_buffer;
}