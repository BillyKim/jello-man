#pragma once
#include "Light.h"
#include "LightDesc.h"
#include "Model.h"

class SpotLight : public Light
{
public:
	SpotLight();
	SpotLight(const SpotLightDesc& desc);
	~SpotLight();
    
    virtual void InitGame();
    virtual void InitEditor();

	virtual void Tick(float dTime);
	virtual void Draw(const RenderContext* rc);

	virtual void Rotate(const Vector3& axis, float angle);

	virtual void Translate(const Vector3& add);
	virtual void SetPosition(const Vector3& pos);
	virtual const Vector3& GetPosition() const;

	virtual void Scale(const Vector3& scale);

	virtual LightType GetType() const { return LightType_Spot; };

	virtual void Enable();
	virtual void Disable();
	virtual bool IsEnabled() const;

	virtual void Select();
	virtual void Deselect();
	virtual bool IsSelected() const;

	virtual bool HasShadowMap() const;
    virtual void SetShadowMap(ID3D10Device* pDXDevice, ShadowMapType type);
	virtual ShadowMapType GetShadowMapType() const;
	virtual Texture2D* GetShadowMap() const;
    virtual Camera* GetShadowCamera() const;
    
    virtual D3D10_RECT CalcScissorRect(Camera* camera, UINT backbufferWidth, UINT backbufferHeight) const;

    virtual void SetColor(const Color& color);
    virtual const Color& GetColor() const;

    virtual void SetMulitplier(float mult);
    virtual float GetMulitplier() const;

    virtual void SetAttenuationStart(float start);
    virtual float GetAttenuationStart() const;

    virtual void SetAttenuationEnd(float end);
    virtual float GetAttenuationEnd() const;

	virtual void SetOpeningsAngle(float rad);
	virtual void AddOpeningsAngle(float rad);
	virtual float GetOpeningsAngle() const;

    virtual const SpotLightDesc& GetDesc() const;
    
    //this method should delete and init the lightbehaviour
    virtual void SetBehaviour(LightBehaviour* lightBehaviour);

	// EDITOR
	HitRegion* GetHitRegion() const
	{ return m_pHitRegion; }

private:
	bool m_IsEnabled;
	bool m_IsSelected;

	SpotLightDesc m_Desc;
	SpotLightDesc m_StartDesc;

	Texture2D* m_ShadowMap;
    ShadowMapType m_ShadowMapType;

	Matrix m_Rotation;
    Vector3 m_Angles;
	float m_Scale;
    float m_OpeningsAngle;

    LightBehaviour* m_pLightBehaviour;
    Camera* m_pShadowCamera;

    Vector3 m_vUp;

	HitRegion* m_pHitRegion;

	Image* m_pSpotLightImage;

	Model<VertexPosCol>* m_pAttenuationSpline;

    void UpdateShadowCameraView();
    void UpdateShadowCameraProjection();
};

