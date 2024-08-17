/*!
@file    YourCullerClipper.h
@author  Prasanna Ghali       (pghali@digipen.edu)

All content (c) 2002 DigiPen Institute of Technology, all rights reserved.
*//*__________________________________________________________________________*/

/*                                                                   includes
----------------------------------------------------------------------------- */

#ifndef YOUR_CULLER_CLIPPER_H_
#define YOUR_CULLER_CLIPPER_H_

#include "../../GfxLib/gfx/GFX.h"


/*                                                                    classes
----------------------------------------------------------------------------- */

/*  _________________________________________________________________________ */
class YourClipper : public gfxController_Clipping {
public:
    // Constructor and Destructor
    YourClipper() {}
    virtual ~YourClipper() {}

    // Operations - compute view-frame frustum
    virtual gfxFrustum ComputeFrustum(gfxMatrix4 const& projection_mtx) override;

    // Operations - culling
    virtual bool Cull(gfxSphere const& bounding_sphere, gfxFrustum const& frustum, gfxOutCode* ptr_outcode) override;

    // Operations - clipping
    virtual gfxVertexBuffer Clip(gfxOutCode outcode, gfxVertexBuffer const& vertex_buffer) override;

private:
    gfxFrustum frustum; // Frustum member variable
};

#endif // YOUR_CULLER_CLIPPER_H_
