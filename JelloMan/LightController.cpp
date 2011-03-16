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

const vector<PointLight>& LightController::GetPointLights() const
{
    return m_PointLights;
}
const vector<DirectionalLight>& LightController::GetDirectionalLights() const
{
    return m_DirectionalLights;
}
const vector<SpotLight>& LightController::GetSpotLights() const
{
    return m_SpotLights;
}