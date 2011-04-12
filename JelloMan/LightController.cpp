#include "LightController.h"
#include "Light.h"


LightController::LightController(void)
{
}


LightController::~LightController(void)
{
    vector<Light*>::iterator it = m_Lights.begin();
    for (; it != m_Lights.end(); ++it)
        delete *it;
}

void LightController::AddLight(Light* light)
{
    m_Lights.push_back(light);
}

const vector<Light*>& LightController::GetLights() const
{
    return m_Lights;
}