#pragma once
#include "Softbody.h"
#include "RenderContext.h"
#include "DeferredPreEffect.h"
#include "LevelObject.h"

class TestSoftbody : public Softbody, public LevelObject
{
public:
    TestSoftbody(const Vector3& pos);
    virtual ~TestSoftbody(void);

    virtual void Init(PhysX* pPhysX);
	virtual void Tick(const float dTime);
    virtual void Draw(const RenderContext* rc);

private:
    DeferredPreEffect* m_pEffect;

    Vector3 m_vPosition;

    Texture2D* m_pDiffuseMap, *m_pSpecMap, *m_pGlossMap;
};

