#pragma once

#include "D3DUtil.h"
#include "Camera.h"

class RenderContext
{
public:
	
	// CONSTRUCTOR - DESTRUCTOR
	RenderContext(Camera* pCamera, DirLight dirLight);
	virtual ~RenderContext();

	// GETTERS
	Camera* GetCamera() const
	{ return m_pCamera; }

	DirLight GetLight() const
	{ return m_Light; }
	
private:

	// DATAMEMBERS
	Camera* m_pCamera;
	DirLight m_Light;


	// DISABLE DEFAULT COPY & ASSIGNMENT
	RenderContext(const RenderContext& t);
	RenderContext& operator=(const RenderContext& t);
};

