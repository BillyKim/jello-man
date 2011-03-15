#pragma once
#include "Model.h"
#include "Texture2D.h"
#include "PhongEffect.h"
#include "Vector4.h"
#include "Matrix.h"
#include "RenderContext.h"

class TestObject2
{
public:
    TestObject2(void);
    virtual ~TestObject2(void);

    void Init();
    void Draw(const RenderContext* pRenderContext);

private:
    Model<VertexPosNormTex>* m_pModel;
    PhongEffect* m_pEffect;
    //Texture2D* m_pDiffuseMap, *m_pSpecMap, *m_pGlossMap;
    Matrix m_mtxWorld;
};

