#pragma once
#include "Light.h"
#include "LightDesc.h"
#include "Model.h"
#include "PosColEffect.h"

class PointLight : public Light
{
public:
	PointLight();
	PointLight(const PointLightDesc& desc);
	~PointLight();

    virtual void InitGame();
    virtual void InitEditor();

	virtual void Tick(float dTime);
	virtual void Draw(const RenderContext* rc);

	virtual void Rotate(const Vector3&, float) {} //pointlight has no rotation

	virtual void Translate(const Vector3& add);
	virtual void SetPosition(const Vector3& pos);
	virtual Vector3 GetPosition() const;

	virtual void Scale(const Vector3& scale);

	virtual LightType GetType() const { return LightType_Point; };

	virtual void Enable();
	virtual void Disable();
	virtual bool IsEnabled() const;

	virtual void Selected(bool selected);
	virtual bool IsSelected() const;

	virtual bool HasShadowMap() const { return false; } //no shadowmaps for a pointlight
    virtual void SetShadowMap(ID3D10Device*, ShadowMapType) { cout << "Warning: trying to set shadowmap on a spotlight"; };
	virtual Texture2D* GetShadowMap() const { cout << "Warning: trying to get the shadowmap from a spotlight"; return 0; }
    virtual Camera* GetShadowCamera() const { cout << "Warning: trying to get the shadowcamera from a spotlight"; return 0;}
    virtual ShadowMapType GetShadowMapType() const { return ShadowMapType_None; };
    
    virtual D3D10_RECT CalcScissorRect(Camera* camera, UINT backbufferWidth, UINT backbufferHeight) const;

    virtual void SetColor(const Color& color);
    virtual const Color& GetColor() const;

    virtual void SetMulitplier(float mult);
    virtual float GetMulitplier() const;

    virtual void SetAttenuationStart(float start);
    virtual float GetAttenuationStart() const;

    virtual void SetAttenuationEnd(float end);
    virtual float GetAttenuationEnd() const;
        
    virtual const PointLightDesc& GetDesc() const;

    //this method should delete and init the lightbehaviour
    virtual void SetBehaviour(LightBehaviour* lightBehaviour);

	// EDITOR
	HitRegion* GetHitRegion() const
	{ return m_pHitRegion; }

    // ISerializeable
	virtual void Serialize(Serializer* pSerializer);
	virtual void Deserialize(Serializer* pSerializer);
    virtual DWORD GetUniqueIdentifier() { return SerializeTypes::PointLight; }

private:
	bool m_IsEnabled;
	bool m_IsSelected;

	PointLightDesc m_Desc;
	PointLightDesc m_StartDesc;

    LightBehaviour* m_pLightBehaviour;

	HitRegion* m_pHitRegion;

	Image* m_pPointLightImage;
	
	Model<VertexPosCol>* m_pAttenuationSpline;
	PosColEffect* m_pEffect;
};

