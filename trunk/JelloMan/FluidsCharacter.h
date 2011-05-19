#pragma once

#include "D3DUtil.h"
#include "Fluid.h"
#include "SoftbodyCharacter.h"
#include "Vector3.h"
#include "PhysX.h"
#include "RenderContext.h"
#include "Actor.h"
#include "PhysXSphere.h"
#include "Blox2D.h"

class FluidsCharacter : public Actor
{
public:

	/* CONSTRUCTOR - DESTRUCTOR */
	FluidsCharacter();
	virtual ~FluidsCharacter();

	/* GENERAL */
	void Init(ID3D10Device* pDXDevice, PhysX* pPhysXEngine, unsigned int maxParticles);
	void Tick(float dTime);
	void Draw(const RenderContext* pRenderContext);

	void Draw(RenderContext* pRenderContext) {};

	virtual void Serialize(Serializer* pSerializer)  const {};
	virtual void Deserialize(Serializer* pSerializer){};

private:

	/* DATAMEMBERS */

	PhysX* m_pPhysXEngine;

	Fluid* m_pFluid;

	NxFluidEmitter* m_pEmitter;
	NxForceField* m_pForceField;

	Vector3 m_Pos;
};

