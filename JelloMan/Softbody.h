#pragma once
#include "D3DUtil.h"
#include "PhysX.h"
#include "Matrix.h"
#include "Model.h"
#include "RenderContext.h"
#include "SoftbodyMesh.h"

class Softbody
{
public:
    virtual ~Softbody(void);
    
    void AddForce(const Vector3& force);
    void AddSpeed(const Vector3& speed);

protected:
    Softbody();
    void InitSoftbody(PhysX* pPhysX, SoftbodyMesh* pSoftbodyMesh, const tstring& path, const Vector3& pos);

    void SetEffectParams();
    void TransformPositions();

    SoftbodyMesh* m_pSoftbodyMesh;

private:
    static const int MAXTETRA = 10000;

	NxSoftBody* m_pSoftbody;
	PhysX* m_pPhysX;

    Vector3 m_arrPositions[MAXTETRA];
    NxU32 m_numPositions;

    NxU32 m_arrIndices[MAXTETRA*4];
    NxU32 m_numIndices;

    Vector3 m_CenterPoint;
    Vector3 m_Dimension;
    float m_Radius;
};

