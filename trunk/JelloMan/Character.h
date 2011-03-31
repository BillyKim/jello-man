#pragma once

#include "Model.h"
#include "Texture2D.h"
#include "DeferredPreEffect.h"
#include "Vector4.h"
#include "Matrix.h"
#include "RenderContext.h"

class Character
{
public:

	// CONSTRUCTOR - DESTRUCTOR
	Character(Camera* pTrackingCamera);
	virtual ~Character();

	// GENERAL
	void Init();
	void Tick(float dTime);
    void Draw(const RenderContext* pRenderContext);
	void Move(Vector3 move);

private:

	// DATAMEMBERS
	Model<VertexPosNormTex>* m_pModel;

    DeferredPreEffect* m_pEffect;

    Texture2D* m_pDiffuseMap, *m_pSpecMap, *m_pGlossMap;

    Matrix m_mtxWorld;
	
	float m_RotationAngle;
	int m_Speed;

	Vector3 m_Pos;

	Camera* m_pTrackingCamera;
};

