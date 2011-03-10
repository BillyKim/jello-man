#pragma once
#include "Vector3.h"
#include "Vector4.h"

//-------Light------------------->
struct Light
{
public:
    Light(): m_Position(Vector3()), m_Color(Vector4(1.f, 1.f, 1.f, 1.f)), 
             m_Multiplier(1), 
             m_AttenuationStart(0), m_AttenuationEnd(0), //0, 0 means no attenuation
             m_Enabled(true) {}
    virtual ~Light() {}

    const Vector3& GetPosition() const { return m_Position; }
    const Vector4& GetColor() const { return m_Color; }
    float GetMultiplier() const { return m_Multiplier; }

    float GetAttenuationStart() const { return m_AttenuationStart; }
    float GetAttenuationEnd() const { return m_AttenuationEnd; }

    bool GetEnabled() const { return m_Enabled; }

    void SetPosition(const Vector3& position);
    void SetColor(const Vector4& color);
    void SetMultiplier(float mult);

    void SetAttenuationStart(float startAtt);
    void SetAttenuationEnd(float endAtt);

    void Enable();
    void Disable();

protected:
    Vector3 m_Position;
    float m_Multiplier;

    Vector4 m_Color;

    float m_AttenuationStart;
    float m_AttenuationEnd;

    bool m_Enabled;
};

////-------SpotLight--------------------->
struct SpotLight : public Light
{
public:
    SpotLight(void);
    virtual ~SpotLight(void);

	float GetRadius();
	void SetRadius(float radius);

private:
	float m_Radius;
};
//<-------------------------------------

//-------DirectionalLight--------------------->
class DirectionalLight : public Light
{
public:
    DirectionalLight(void);
    virtual ~DirectionalLight(void);

    const Vector3& GetDirection() const { return m_Direction; }

    void SetDirection(const Vector3& dir);

private:
    Vector3 m_Direction;
};
//<--------------------------------------------

//-------PointLight--------------------->
class PointLight : public Light
{
public:
    PointLight(void);
    virtual ~PointLight(void);
};
//<--------------------------------------------

