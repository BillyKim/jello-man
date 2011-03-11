#include "RenderContext.h"


RenderContext::RenderContext(Camera* pCamera, DirLight dirLight)	:	m_pCamera(pCamera),
																		m_Light(dirLight)
{
}


RenderContext::~RenderContext()
{
}
