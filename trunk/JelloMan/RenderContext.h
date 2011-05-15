#pragma once

#include "D3DUtil.h"
#include "Camera.h"
#include "LightController.h"
class DeferredRenderer;

class RenderContext
{
public:
	
	// CONSTRUCTOR - DESTRUCTOR
    RenderContext(Camera* pCamera, LightController* pLichtController, DeferredRenderer* pDr);
	virtual ~RenderContext();

	// GETTERS
	Camera* GetCamera() const
	{ return m_pCamera; }

	LightController* GetLightController() const
	{ return m_pLightController; }

    DeferredRenderer* GetDeferredRenderer() const 
    { return m_pDeferredRenderer; }

	// SETTERS
	void SetCamera(Camera* pCamera)
	{ m_pCamera = pCamera; }
	
private:

	// DATAMEMBERS
	Camera* m_pCamera;
    LightController* m_pLightController;
    DeferredRenderer* m_pDeferredRenderer;

	// DISABLE DEFAULT COPY & ASSIGNMENT
	RenderContext(const RenderContext& t);
	RenderContext& operator=(const RenderContext& t);
};

