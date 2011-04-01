#pragma once
#include "Model.h"
#include "Texture2D.h"
#include "DeferredPreEffect.h"
#include "Vector4.h"
#include "Matrix.h"
#include "RenderContext.h"
#include "PhysX.h"
#include "Actor.h"

class LevelObject : public Actor
{
public:
    
	// CONSTRUCTOR - DESTRUCTOR
	LevelObject();
    virtual ~LevelObject();

	// GENERAL
    void Init(PhysX* pPhysX);
	void Tick(const float dTime);
    void Draw(const RenderContext* pRenderContext);

	// SETTERS
	void SetModelPath(tstring modelPath)
	{ m_ModelPath = modelPath; }

	void SetDiffusePath(tstring diffusePath)
	{ m_DiffusePath = diffusePath; }

	void SetSpecPath(tstring specPath)
	{ m_SpecPath = specPath; }

	void SetGlossPath(tstring glossPath)
	{ m_GlossPath = glossPath; }

	void SetPhysXModelPath(tstring physXModelPath)
	{ m_PhysXModelPath = physXModelPath; }

	void SetPos(Vector3 pos)
	{ m_Pos = pos; }

	// GETTERS
	bool IsInitialized() const
	{ return m_bInitialized; }

private:

	// DATAMEMBERS
	tstring m_ModelPath;
	tstring m_DiffusePath;
	tstring m_SpecPath;
	tstring m_GlossPath;
	tstring m_PhysXModelPath;

	bool m_bInitialized;

    Model<VertexPosNormTex>* m_pModel;
    DeferredPreEffect* m_pEffect;
    Texture2D* m_pDiffuseMap, *m_pSpecMap, *m_pGlossMap;
    Matrix m_mtxWorld;

	Vector3 m_Pos;
};

