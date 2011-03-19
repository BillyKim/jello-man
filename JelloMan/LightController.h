#pragma once
#include "D3DUtil.h"
#include <vector>
#include "Light.h"
#include "Blox2D.h"
#include "Camera.h"
#include "Controls.h"

class LightController
{
public:
    LightController(void);
    virtual ~LightController(void);

    void AddLight(const PointLight& light);
    void AddLight(const DirectionalLight& light);
    void AddLight(const SpotLight& light);

    vector<PointLight>& GetPointLights();
    const vector<DirectionalLight>& GetDirectionalLights() const;
    const vector<SpotLight>& GetSpotLights() const;

	void VisualLightDebugger(const Camera* pCamera);

	void MoveAble(bool m) { m_bMoveable = m; }

private:
    vector<PointLight> m_PointLights;
    vector<DirectionalLight> m_DirectionalLights;
	vector<SpotLight> m_SpotLights;

	Vector3 m_MousePos;

	vector<D3DXVECTOR3> m_MPos;

	bool m_bLockX;
	bool m_bLockY;
	bool m_bLockZ;
	bool m_bMoveable;
	bool m_bClick;

	vector<HitRegion*> m_pHitRectLights;
	vector<bool> m_LightsSelected;
};

