#pragma once

#include "D3DUtil.h"
#include "RenderContext.h"
#include "FluidEffect.h"
#include "vertex.h"
#include "Particle.h"
#include "Color.h"
#include "ModelMesh.h"

class Fluid 
{
public:

	/* CONSTRUCTOR - DESTRUCTOR */

	Fluid(NxScene* pScene, NxFluidDesc &desc, const Color& particleColor,  float particleSize, ID3D10Device* pDevice);
	~Fluid();

	/* GENERAL */

	void Draw(const RenderContext* pRenderContext);

	/* GETTERS */

	NxFluid* GetNxFluid() { return m_pFluid; }
	const vector<Particle>& GetParticles() { return m_ParticleBuffer; }
	unsigned GetNrParticles() { return m_NrParticleBuffer; }
	float GetParticleSize() { return m_ParticleSize; }
	const Color& GetColor() { return m_ParticleColor; }
	
	/* SETTERS */

	void SetParticleSize(float size) { m_ParticleSize = size; }
	void SetParticleColor(const Color& color) { m_ParticleColor = color; }

private:

	/* DATAMEMBERS */
	unsigned int m_NrParticleBuffer;
	vector<Particle> m_ParticleBuffer;

	NxFluid* m_pFluid;
	Color m_ParticleColor;
	float m_ParticleSize;
	unsigned int m_MaxParticles;

	// RENDERSTUFF
	void BuildVertexBuffer();
	void UpdateVertexBuffer();

	ID3D10Device* m_pDevice;

	ID3D10Buffer* m_pVertexBuffer;

	vector<VertexPos> m_VecVertices;

    FluidEffect* m_pEffect;
    Texture2D* m_pTexRainbow;

	Matrix m_mtxWorld;

	
	/* Disableing default copyconstructor and assignment operator */
	Fluid(const Fluid&);
	Fluid& operator=(const Fluid&);
};