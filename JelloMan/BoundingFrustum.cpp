#include "BoundingFrustum.h"
#include "Matrix.h"
#include "BoundingSphere.h"

BoundingFrustum::BoundingFrustum(void)
{
    m_Frustum.resize(6);
}


BoundingFrustum::~BoundingFrustum(void)
{
}


bool BoundingFrustum::CheckCollision(const BoundingSphere& sphere)
{
    D3DXVECTOR3 pos = sphere.position.ToD3DVector3();

    vector<D3DXPLANE>::const_iterator it = m_Frustum.cbegin();
    for(; it != m_Frustum.cend(); ++it)
    {
        if (D3DXPlaneDotCoord(&*it, &pos) + sphere.radius < 0)
        {
            // Outside the frustum, no collision
            return false;
        }
    }
    return true;
}


void BoundingFrustum::BuildFrustum(const Matrix& viewProjection)
{
    // Left plane
    m_Frustum[0].a = viewProjection(0, 3) + viewProjection(0, 0);
    m_Frustum[0].b = viewProjection(1, 3) + viewProjection(1, 0);
    m_Frustum[0].c = viewProjection(2, 3) + viewProjection(2, 0);
    m_Frustum[0].d = viewProjection(3, 3) + viewProjection(3, 0);
 
    // Right plane
    m_Frustum[1].a = viewProjection(0, 3) - viewProjection(0, 0);
    m_Frustum[1].b = viewProjection(1, 3) - viewProjection(1, 0);
    m_Frustum[1].c = viewProjection(2, 3) - viewProjection(2, 0);
    m_Frustum[1].d = viewProjection(3, 3) - viewProjection(3, 0);
 
    // Top plane
    m_Frustum[2].a = viewProjection(0, 3) - viewProjection(0, 1);
    m_Frustum[2].b = viewProjection(1, 3) - viewProjection(1, 1);
    m_Frustum[2].c = viewProjection(2, 3) - viewProjection(2, 1);
    m_Frustum[2].d = viewProjection(3, 3) - viewProjection(3, 1);
 
    // Bottom plane
    m_Frustum[3].a = viewProjection(0, 3) + viewProjection(0, 1);
    m_Frustum[3].b = viewProjection(1, 3) + viewProjection(1, 1);
    m_Frustum[3].c = viewProjection(2, 3) + viewProjection(2, 1);
    m_Frustum[3].d = viewProjection(3, 3) + viewProjection(3, 1);
 
    // Near plane
    m_Frustum[4].a = viewProjection(0, 2);
    m_Frustum[4].b = viewProjection(1, 2);
    m_Frustum[4].c = viewProjection(2, 2);
    m_Frustum[4].d = viewProjection(3, 2);
 
    // Far plane
    m_Frustum[5].a = viewProjection(0, 3) - viewProjection(0, 2);
    m_Frustum[5].b = viewProjection(1, 3) - viewProjection(1, 2);
    m_Frustum[5].c = viewProjection(2, 3) - viewProjection(2, 2);
    m_Frustum[5].d = viewProjection(3, 3) - viewProjection(3, 2);
 
    // Normalize planes
    for_each(m_Frustum.begin(), m_Frustum.end(), [](D3DXPLANE& plane)
    {
        D3DXPlaneNormalize(&plane, &plane);
    });
}