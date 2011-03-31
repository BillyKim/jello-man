#pragma once
#include "Actor.h"
#include "Model.h"
#include "DeferredPreEffect.h"
#include "Texture2D.h"
#include "RenderContext.h"

class TestPhysXBox : public Actor
{
public:
    TestPhysXBox(PhysX* pPhysX, const Vector3& pos);
    virtual ~TestPhysXBox(void);

	void TestPhysXBox::Init();
	void TestPhysXBox::Draw(const RenderContext* pRenderContext);

private:
    Model<VertexPosNormTex>* m_pModel;
    DeferredPreEffect* m_pEffect;
    Texture2D* m_pDiffuseMap, *m_pSpecMap, *m_pGlossMap;
};

