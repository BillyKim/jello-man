#pragma once
#include "D3DUtil.h"
#include <vector>
#include "Light.h"
#include "Blox2D.h"
#include "Camera.h"
#include "Controls.h"
#include "ContentManager.h"
#include "Model.h"

class LightController
{
public:
    LightController(void);
    virtual ~LightController(void);

    void AddLight(const PointLight& light);
    void AddLight(const DirectionalLight& light);
    void AddLight(const SpotLight& light);

    vector<PointLight>& GetPointLights();
    vector<DirectionalLight>& GetDirectionalLights();
    vector<SpotLight>& GetSpotLights();

private:
    vector<PointLight> m_PointLights;
    vector<DirectionalLight> m_DirectionalLights;
	vector<SpotLight> m_SpotLights;
};

