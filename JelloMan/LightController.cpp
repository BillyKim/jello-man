#include "LightController.h"
#include "Light.h"
#include <algorithm>


LightController::LightController(void)
{
}


LightController::~LightController(void)
{
    Clear();
}
void LightController::Clear()
{
    if (m_Lights.size() != 0)
    {
        for_each(m_Lights.cbegin(), m_Lights.cend(), [](Light* l)
        {
            delete l;
        });
        m_Lights.clear();
    }
}

void LightController::Tick(float dTime)
{
    vector<Light*>::iterator it = m_Lights.begin();
    for (; it != m_Lights.end(); ++it)
    {
        (*it)->Tick(dTime);
    }
}

void LightController::AddLight(Light* light)
{
    m_Lights.push_back(light);
}

const vector<Light*>& LightController::GetLights() const
{
    return m_Lights;
}