#pragma once
#include "Model.h"
#include "Texture2D.h"
#include "DeferredPreEffect.h"
#include "Vector4.h"
#include "Matrix.h"
#include "RenderContext.h"

class TestObject
{
public:
    TestObject(void);
    virtual ~TestObject(void);

    void Init();
    void Draw(const RenderContext* pRenderContext);

private:
    Model<VertexPosNormTex>* m_pModel;
    DeferredPreEffect* m_pEffect;
    Texture2D* m_pDiffuseMap, *m_pSpecMap, *m_pGlossMap;
    Matrix m_mtxWorld;
};

