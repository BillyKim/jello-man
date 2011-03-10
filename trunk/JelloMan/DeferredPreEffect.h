#pragma once
#include "Effect.h"
#include "Matrix.h"
#include "Texture2D.h"

class DeferredPreEffect : public Effect
{
public:
    DeferredPreEffect(ID3D10Device* pDXDevice, ID3D10Effect* pEffect);
    virtual ~DeferredPreEffect(void);

    void SetWorld(const Matrix& world);
    void SetWorldViewProjection(const Matrix& wvp);

    void SetDiffuseMap(Texture2D* diffuseMap);
    void SetSpecMap(Texture2D* specMap);
    void SetGlossMap(Texture2D* glossMap);

private:
    ID3D10EffectMatrixVariable* m_pWorld;
    ID3D10EffectMatrixVariable* m_pWVP;

    ID3D10EffectShaderResourceVariable* m_pDiffuseMap;
    ID3D10EffectShaderResourceVariable* m_pSpecMap;
    ID3D10EffectShaderResourceVariable* m_pGlossMap;
    //ID3D10ShaderResourceView* m_pNormalMap;

};


//cbuffer cbPerObject
//{
//	matrix mtxWorld : World;
//	matrix mtxWorldVP : WorldViewProjection;
//};
//
//Texture2D diffuseMap : DiffuseMap;
////Texture2D normalMap : NormalMap;
//Texture2D specMap : SpecMap;
//Texture2D glossMap : GlossMap;
