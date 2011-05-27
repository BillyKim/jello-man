#include "RenderContext.h"


RenderContext::RenderContext(Graphics::Camera::CameraBase* pCamera, 
                             LightController* pLichtController, 
                             DeferredRenderer* pDr,
                             PreShadowEffect* pPreShadowEffect, 
                             PreShadowEffectInstanced* pPreShadowEffectInstanced):	
                                                    m_pCamera(pCamera),
                                                    m_pLightController(pLichtController),
                                                    m_pDeferredRenderer(pDr),
                                                    m_pPreShadowEffect(pPreShadowEffect),
                                                    m_pPreShadowEffectInstanced(pPreShadowEffectInstanced)
{
}


RenderContext::~RenderContext()
{
}
