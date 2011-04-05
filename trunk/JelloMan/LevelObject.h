#pragma once
#include "Model.h"
#include "Texture2D.h"
#include "DeferredPreEffectNormals.h"
#include "Vector4.h"
#include "Matrix.h"
#include "RenderContext.h"
#include "PhysX.h"
#include "Actor.h"
#include "DeferredPreEffect.h"

class LevelObject : public Actor
{
public:
    
	// CONSTRUCTOR - DESTRUCTOR
	LevelObject();
    virtual ~LevelObject();

	// GENERAL
    virtual void Init(PhysX* pPhysX);
	virtual void Tick(const float dTime);
    virtual void Draw(const RenderContext* pRenderContext);

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

	void SetNormalPath(tstring normalPath)
	{ m_NormalPath = normalPath; }

	void UseSimplifiedPhysXMesh(bool use)
	{ m_bUseSimplifiedPhysXMesh = use; }

	void SetSimplifiedPhysXMesh(PhysXShape* pPhysXShape)
	{ m_pPhysXShape = pPhysXShape; }

	void SetMass(int mass)
	{ m_Mass = mass; }

	void SetRigid(bool rigid)
	{ m_bRigid = rigid; }

	void UseNormalMap(bool use)
	{ m_bUseNormalMap = use; }

	void Selected(bool selected)
	{ m_bIsSelected = selected; }

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
	tstring m_NormalPath;

	bool m_bInitialized;
	bool m_bUseSimplifiedPhysXMesh;
	bool m_bRigid;
	bool m_bUseNormalMap;
	bool m_bIsSelected;

	int m_Mass;

    Model<VertexPosNormTanTex>* m_pModel;

    DeferredPreEffectNormals* m_pEffectNormals;
	DeferredPreEffect* m_pEffect;

    Texture2D* m_pDiffuseMap, *m_pSpecMap, *m_pGlossMap, *m_pNormalMap;
	PhysXShape* m_pPhysXShape;
};

