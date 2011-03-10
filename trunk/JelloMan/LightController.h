#pragma once
#include "D3DUtil.h"
#include <vector>
#include "Light.h"

class LightController
{
public:
    LightController(void);
    virtual ~LightController(void);

    void AddLight(PointLight* light);
    void AddLight(DirectionalLight* light);

    const vector<PointLight*>& GetPointLights() const;
    const vector<DirectionalLight*>& GetDirectionalLights() const;

private:
    vector<PointLight*> m_PointLights;
    vector<DirectionalLight*> m_DirectionalLights;
	vector<SpotLight*> m_SpotLights;
};

