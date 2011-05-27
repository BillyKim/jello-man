#pragma once

#include "D3DUtil.h"
#include "CameraBase.h"
#include "LightController.h"
#include "PreShadowEffect.h"
#include "PreShadowEffectInstanced.h"
class DeferredRenderer;

class RenderContext
{
public:
	
	// CONSTRUCTOR - DESTRUCTOR
    RenderContext(Graphics::Camera::CameraBase* pCamera, LightController* pLichtController, DeferredRenderer* pDr,
                PreShadowEffect* pPreShadowEffect, PreShadowEffectInstanced* pPreShadowEffectInstanced);
	virtual ~RenderContext();

	// GETTERS
	Graphics::Camera::CameraBase* GetCamera() const { return m_pCamera; }
	LightController* GetLightController() const { return m_pLightController; }
    DeferredRenderer* GetDeferredRenderer() const { return m_pDeferredRenderer; }
    PreShadowEffect* GetPreShadowEffect() const { return m_pPreShadowEffect; }
    PreShadowEffectInstanced* GetPreShadowEffectInstanced() const { return m_pPreShadowEffectInstanced; }
	// SETTERS
	void SetCamera(Graphics::Camera::CameraBase* pCamera) { m_pCamera = pCamera; }
	
private:

	// DATAMEMBERS
	Graphics::Camera::CameraBase* m_pCamera;
    LightController* m_pLightController;
    DeferredRenderer* m_pDeferredRenderer;
    PreShadowEffect* m_pPreShadowEffect;
    PreShadowEffectInstanced* m_pPreShadowEffectInstanced;

	// DISABLE DEFAULT COPY & ASSIGNMENT
	RenderContext(const RenderContext& t);
	RenderContext& operator=(const RenderContext& t);
};

