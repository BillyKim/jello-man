#include "RenderContext.h"


RenderContext::RenderContext(Camera* pCamera, LightController* pLichtController):	
                                                    m_pCamera(pCamera),
                                                    m_pLightController(pLichtController)
{
}


RenderContext::~RenderContext()
{
}
