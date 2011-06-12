#pragma once
struct Matrix;
struct D3DXPLANE;
struct BoundingSphere;

#include <vector>

class BoundingFrustum
{
public:
    BoundingFrustum(void);
    virtual ~BoundingFrustum(void);

    void BuildFrustum(const Matrix& viewProjection);

    bool CheckCollision(const BoundingSphere& sphere);

private:
    std::vector<D3DXPLANE> m_Frustum;
};

