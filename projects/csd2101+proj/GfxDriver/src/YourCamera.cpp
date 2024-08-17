#include "YourCamera.h"
#include <iostream>

void YourCamera::Move(gfxVector3 const& p) {
    gfxVector3 basis_x, basis_y, basis_z;
    GetBasisVectors(&basis_x, &basis_y, &basis_z);

    gfxVector3 displacement = p.x * basis_x + p.y * basis_y + p.z * basis_z;
    mFrom += displacement;
    mAt += displacement;

    std::cout << "Camera moved to position (" << mFrom.x << ", " << mFrom.y << ", " << mFrom.z << ")\n";
    UpdateSphericalFromPoints();
}

void YourCamera::Move(float x, float y, float z) {
    Move(gfxVector3(x, y, z));
}

void YourCamera::UpdateSphericalFromPoints() {
    gfxVector3 direction = mAt - mFrom;
    mRadius = direction.Length();
    mAzimuth = atan2f(direction.z, direction.x);
    mLatitude = asinf(direction.y / mRadius);
    std::cout << "Updated spherical coordinates: radius " << mRadius << ", azimuth " << mAzimuth << ", latitude " << mLatitude << "\n";
}

void YourCamera::UpdatePointsFromSpherical() {
    mAt.x = mFrom.x + mRadius * cosf(mLatitude) * cosf(mAzimuth);
    mAt.y = mFrom.y + mRadius * sinf(mLatitude);
    mAt.z = mFrom.z + mRadius * cosf(mLatitude) * sinf(mAzimuth);
    std::cout << "Updated points from spherical coordinates: mAt (" << mAt.x << ", " << mAt.y << ", " << mAt.z << ")\n";
}
