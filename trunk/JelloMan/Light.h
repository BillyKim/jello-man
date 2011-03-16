#pragma once
#include "Vector4.h"

////-------SpotLight--------------------->
struct SpotLight
{
public:
    SpotLight(void);
    virtual ~SpotLight(void) {}

    float GetRadius() { return m_Radius; }
    void SetRadius(float radius) { m_Radius = radius; }

    const Vector3& GetPosition() const { return m_Position; }
    const Vector4& GetColor() const { return m_Color; }
    float GetMultiplier() const { return m_Multiplier; }

    float GetAttenuationStart() const { return m_AttenuationStart; }
    float GetAttenuationEnd() const { return m_AttenuationEnd; }

    bool GetEnabled() const { return m_Enabled; }

    void SetPosition(const Vector3& position) { m_Position = position; }
    void SetColor(const Vector4& color) { m_Color = color; }
    void SetMultiplier(float mult) { m_Multiplier = mult; }

    void SetAttenuationStart(float startAtt) { m_AttenuationStart = startAtt; }
    void SetAttenuationEnd(float endAtt) { m_AttenuationEnd = endAtt; }

    void Enable() { m_Enabled = true; }
    void Disable()  { m_Enabled = false; }

private:
    Vector3 m_Position;
    float m_Multiplier;

    Vector4 m_Color;

    float m_AttenuationStart;
    float m_AttenuationEnd;
	float m_Radius;

    bool m_Enabled;
};
//<-------------------------------------

//-------DirectionalLight--------------------->
struct DirectionalLight
{
public:
    DirectionalLight(void);
    virtual ~DirectionalLight(void) {}

    const Vector3& GetDirection() const { return m_Direction; }

    void SetDirection(const Vector3& dir) { m_Direction = dir; }

    const Vector3& GetPosition() const { return m_Position; }
    const Vector4& GetColor() const { return m_Color; }
    float GetMultiplier() const { return m_Multiplier; }

    float GetAttenuationStart() const { return m_AttenuationStart; }
    float GetAttenuationEnd() const { return m_AttenuationEnd; }

    bool GetEnabled() const { return m_Enabled; }

    void SetPosition(const Vector3& position) { m_Position = position; }
    void SetColor(const Vector4& color) { m_Color = color; }
    void SetMultiplier(float mult) { m_Multiplier = mult; }

    void SetAttenuationStart(float startAtt) { m_AttenuationStart = startAtt; }
    void SetAttenuationEnd(float endAtt) { m_AttenuationEnd = endAtt; }

    void Enable() { m_Enabled = true; }
    void Disable()  { m_Enabled = false; }

private:
    Vector3 m_Position;
    float m_Multiplier;

    Vector4 m_Color;
    Vector3 m_Direction;

    float m_AttenuationStart;
    float m_AttenuationEnd;

    bool m_Enabled;
};
//<--------------------------------------------

//-------PointLight--------------------->
struct PointLight
{
    PointLight();

    Vector4 color;
    Vector3 position;
    float multiplier;


    //float m_AttenuationStart;
    //float m_AttenuationEnd;

    //bool m_Enabled;
};
//<--------------------------------------------

