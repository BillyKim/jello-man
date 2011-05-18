#include "RenderContext.h"


RenderContext::RenderContext(Graphics::Camera::CameraBase* pCamera, LightController* pLichtController, DeferredRenderer* pDr):	
                                                    m_pCamera(pCamera),
                                                    m_pLightController(pLichtController),
                                                    m_pDeferredRenderer(pDr)
{
}


RenderContext::~RenderContext()
{
}
