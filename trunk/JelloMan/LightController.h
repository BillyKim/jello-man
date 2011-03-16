#pragma once
#include "D3DUtil.h"
#include <vector>
#include "Light.h"

class LightController
{
public:
    LightController(void);
    virtual ~LightController(void);

    void AddLight(const PointLight& light);
    void AddLight(const DirectionalLight& light);
    void AddLight(const SpotLight& light);

    const vector<PointLight>& GetPointLights() const;
    const vector<DirectionalLight>& GetDirectionalLights() const;
    const vector<SpotLight>& GetSpotLights() const;

private:
    vector<PointLight> m_PointLights;
    vector<DirectionalLight> m_DirectionalLights;
	vector<SpotLight> m_SpotLights;
};

