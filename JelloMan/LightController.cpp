#include "LightController.h"


LightController::LightController(void)
{
}


LightController::~LightController(void)
{
    //for (UINT i = 0; i < m_PointLights.size(); ++i)
    //    delete m_PointLights[i];
    //for (UINT i = 0; i < m_DirectionalLights.size(); ++i)
    //    delete m_DirectionalLights[i];
    //for (UINT i = 0; i < m_SpotLights.size(); ++i)
    //    delete m_DirectionalLights[i];
}

void LightController::AddLight(const PointLight& light)
{
    m_PointLights.push_back(light);
}
void LightController::AddLight(const DirectionalLight& light)
{
    m_DirectionalLights.push_back(light);
}
void LightController::AddLight(const SpotLight& light)
{
    m_SpotLights.push_back(light);
}

vector<PointLight>& LightController::GetPointLights()
{
    return m_PointLights;
}
vector<DirectionalLight>& LightController::GetDirectionalLights()
{
    return m_DirectionalLights;
}
vector<SpotLight>& LightController::GetSpotLights()
{
    return m_SpotLights;
}