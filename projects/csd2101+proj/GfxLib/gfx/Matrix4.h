#ifndef GFX_MATRIX4_H_
#define GFX_MATRIX4_H_

#include "GFXInternal.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Math.h"

class gfxMatrix4
{
    // * operator: matrix multiplication
    friend gfxMatrix4 operator*(const gfxMatrix4& l, const gfxMatrix4& r);
    friend gfxVector4 operator*(const gfxMatrix4& l, const gfxVector4& r);
    friend gfxMatrix4 operator*(float l, const gfxMatrix4& r);

public:
    gfxMatrix4();
    gfxMatrix4(float e00, float e10, float e20, float e30,
        float e01, float e11, float e21, float e31,
        float e02, float e12, float e22, float e32,
        float e03, float e13, float e23, float e33);
    gfxMatrix4(const gfxMatrix4& m);
    ~gfxMatrix4();
    gfxMatrix4& operator=(const gfxMatrix4& m);

    float& operator()(unsigned int column, unsigned int row);
    const float& operator()(unsigned int column, unsigned int row) const;

    float* Begin();
    const float* Begin() const;
    float* End();
    const float* End() const;

    void SetTo(float e00, float e10, float e20, float e30,
        float e01, float e11, float e21, float e31,
        float e02, float e12, float e22, float e32,
        float e03, float e13, float e23, float e33);
    void SetToZero();
    void SetToIdentity();

    gfxVector3 GetRow3(unsigned int) const;
    gfxVector4 GetRow4(unsigned int) const;
    gfxVector3 GetCol3(unsigned int) const;
    gfxVector4 GetCol4(unsigned int) const;

    static gfxMatrix4 BuildTranslation(float x, float y, float z);
    static gfxMatrix4 BuildTranslation(const gfxVector3& xyz);
    static gfxMatrix4 BuildRotation(float a, float x, float y, float z);
    static gfxMatrix4 BuildRotation(float angle, const gfxVector3& axis);
    static gfxMatrix4 BuildScaling(float cx, float cy, float cz, float x, float y, float z);
    static gfxMatrix4 BuildScaling(const gfxVector3& pivot, const gfxVector3& scaleFactors);
    static gfxMatrix4 BuildLookAt(const gfxVector3& eye, const gfxVector3& at, const gfxVector3& up);
    static gfxMatrix4 BuildPerspective(float vfov, float aspect, float near, float far);
    static gfxMatrix4 BuildFrustum(float l, float r, float b, float t, float n, float f);
    static gfxMatrix4 BuildOrtho(float l, float r, float b, float t, float n, float f);
    static gfxMatrix4 BuildViewport(float x, float y, float w, float h);
    static gfxMatrix4 BuildNormalTransform(const gfxMatrix4&);

    friend gfxVector3 operator*(const gfxMatrix4& l, const gfxVector3& r);

protected:
    float mMat[16];
};

inline gfxVector3 operator*(const gfxMatrix4& l, const gfxVector3& r) {
    return gfxVector3(
        l(0, 0) * r.x + l(0, 1) * r.y + l(0, 2) * r.z + l(0, 3),
        l(1, 0) * r.x + l(1, 1) * r.y + l(1, 2) * r.z + l(1, 3),
        l(2, 0) * r.x + l(2, 1) * r.y + l(2, 2) * r.z + l(2, 3)
    );
}

#endif  /* GFX_MATRIX4_H_ */
