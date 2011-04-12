#pragma once
#include <vector>

using namespace std;

class Light;

class LightController
{
public:
    LightController(void);
    virtual ~LightController(void);

    void AddLight(Light* light);

    const vector<Light*>& GetLights() const;

private:
    vector<Light*> m_Lights;
};

