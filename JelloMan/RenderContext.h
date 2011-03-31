#pragma once

#include "D3DUtil.h"
#include "Camera.h"
#include "LightController.h"

class RenderContext
{
public:
	
	// CONSTRUCTOR - DESTRUCTOR
	RenderContext(Camera* pCamera, LightController* pLichtController);
	virtual ~RenderContext();

	// GETTERS
	Camera* GetCamera() const
	{ return m_pCamera; }

	LightController* GetLightController() const
	{ return m_pLightController; }

	// SETTERS
	void SetCamera(Camera* pCamera)
	{ m_pCamera = pCamera; }
	
private:

	// DATAMEMBERS
	Camera* m_pCamera;
    LightController* m_pLightController;


	// DISABLE DEFAULT COPY & ASSIGNMENT
	RenderContext(const RenderContext& t);
	RenderContext& operator=(const RenderContext& t);
};

