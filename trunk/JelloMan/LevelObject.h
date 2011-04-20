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
#include "PreShadowEffect.h"

#include "ILevelObject.h"

class LevelObject : public ILevelObject, public Actor
{
public:
    
	// CONSTRUCTOR - DESTRUCTOR
	LevelObject();
    virtual ~LevelObject();

	// GENERAL
    virtual void Init(PhysX* pPhysX);
    virtual void Selected(bool selected);

    //IUpdatable
	virtual void Tick(float dTime);

    //IDrawable
    virtual void Draw(RenderContext* pRenderContext);
    virtual void DrawShadow(RenderContext* pRenderContext, PreShadowEffect* e);

    //ITransformable
    virtual void Rotate(const Vector3& axis, float angle) { Actor::Rotate(axis, angle); }

    virtual void Translate(const Vector3& add) { Actor::Translate(add); }
    virtual void SetPosition(const Vector3& pos) { Actor::SetPosition(pos); }
    virtual const Vector3& GetPosition() const { return Actor::GetPosition(); }

    virtual void Scale(const Vector3& scale) { Actor::Scale(scale); }

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

	// GETTERS
	bool IsInitialized() const
	{ return m_bInitialized; }

    Model<VertexPosNormTanTex>* GetModel()
    { return m_pModel; }

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

