#pragma once
#include "Light.h"
#include "LightDesc.h"
#include "Model.h"
#include "PosColEffect.h"

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
	virtual Vector3 GetPosition() const;

	virtual void Scale(const Vector3& scale);

	virtual LightType GetType() const { return LightType_Spot; };

	virtual void Enable();
	virtual void Disable();
	virtual bool IsEnabled() const;

	virtual void Selected(bool selected);
	virtual bool IsSelected() const;
    virtual IEditorObject* Copy() const;

	virtual bool HasShadowMap() const;
    virtual void SetShadowMap(ID3D10Device* pDXDevice, ShadowMapType type);
	virtual ShadowMapType GetShadowMapType() const;
	virtual Texture2D* GetShadowMap() const;
    virtual Graphics::Camera::CameraBase* GetShadowCamera() const;
    
    virtual D3D10_RECT CalcScissorRect(Graphics::Camera::CameraBase* camera, UINT backbufferWidth, UINT backbufferHeight) const;

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

    // ISerializeable
	virtual void Serialize(Serializer* pSerializer) const;
	virtual void Deserialize(Serializer* pSerializer);
    virtual DWORD GetUniqueIdentifier() const { return SerializeTypes::SpotLight; }

private:
	bool m_IsEnabled;
	bool m_IsSelected;

	SpotLightDesc m_Desc;
	SpotLightDesc m_StartDesc;

	Texture2D* m_ShadowMap;
    ShadowMapType m_ShadowMapType;

	Matrix m_Rotation;
    float m_OpeningsAngle;

    LightBehaviour* m_pLightBehaviour;
    Graphics::Camera::CameraBase* m_pShadowCamera;

    Vector3 m_vUp;

	HitRegion* m_pHitRegion;

	Image* m_pSpotLightImage;

	Model<VertexPosCol>* m_pAttenuationSpline;
	Model<VertexPosCol>* m_pAttenuationSpline2;
	Model<VertexPosCol>* m_pAttenuationSpline3;
	PosColEffect* m_pEffect;

    void UpdateShadowCameraView();
    void UpdateShadowCameraProjection();
};

