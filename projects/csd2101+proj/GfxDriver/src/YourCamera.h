/*!
@file    YourCamera.h
@author  Prasanna Ghali  (pghali, pghali@digipen.edu)

CVS: $Id: Camera.cpp,v 1.13 2005/03/15 23:34:41 pghali Exp $

All content (c) 2005 DigiPen (USA) Corporation, all rights reserved.
*//*__________________________________________________________________________*/

/*                                                                   includes
----------------------------------------------------------------------------- */

#ifndef YOUR_CAMERA_H_
#define YOUR_CAMERA_H_

#include "../../GfxLib/gfx/GFX.h"

/*                                                                    classes
----------------------------------------------------------------------------- */

/*  _________________________________________________________________________ */
class YourCamera : public gfxCamera {
public:
    // Constructor and Destructor
    YourCamera() {}
    virtual ~YourCamera() {}

    // Displacement methods
    virtual void Move(float x, float y, float z) override;
    virtual void Move(gfxVector3 const& p) override;

    // Update methods
    virtual void UpdateSphericalFromPoints() override;
    virtual void UpdatePointsFromSpherical() override;
};

#endif // YOUR_CAMERA_H_
