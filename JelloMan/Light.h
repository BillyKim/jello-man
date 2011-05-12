#pragma once
#include "Texture2D.h"
#include "RenderContext.h"
#include "Vector3.h"
#include "Color.h"
#include "LightBehaviour.h"
#include "Camera.h"
#include "Blox2D.h"
#include "IEditorObject.h"
#include "ISerializable.h"

enum LightType
{
	LightType_Point, LightType_Spot, LightType_Directional
};

enum ShadowMapType
{
	ShadowMapType_None = 0, 
	ShadowMapType128x128 = 128, 
	ShadowMapType256x256 = 256, 
	ShadowMapType512x512 = 512, 
	ShadowMapType1024x1024 = 1024,
	ShadowMapType2048x2048 = 2048
};

class Light : public IEditorObject, public ISerializable
{
public:
    virtual void InitGame() = 0;
    virtual void InitEditor() = 0;

	virtual void Tick(float dTime) = 0;
	virtual void Draw(const RenderContext* rc) = 0;

	virtual void Rotate(const Vector3& axis, float angle) = 0;

	virtual void Translate(const Vector3& add) = 0;
	virtual void SetPosition(const Vector3& pos) = 0;
	virtual Vector3 GetPosition() const = 0;

	virtual void Scale(const Vector3& scale) = 0;

	virtual LightType GetType() const = 0;

	virtual void Enable() = 0;
	virtual void Disable() = 0;
	virtual bool IsEnabled() const = 0;

	virtual void Selected(bool selected) = 0;
	virtual bool IsSelected() const = 0;

	virtual bool HasShadowMap() const = 0;
    virtual void SetShadowMap(ID3D10Device* pDXDevice, ShadowMapType type) = 0;
	virtual Texture2D* GetShadowMap() const = 0;
    virtual Camera* GetShadowCamera() const = 0;
    virtual ShadowMapType GetShadowMapType() const = 0;

    virtual D3D10_RECT CalcScissorRect(Camera* camera, UINT backbufferWidth, UINT backbufferHeight) const = 0;

    virtual void SetColor(const Color& color) = 0;
    virtual const Color& GetColor() const = 0;

    virtual void SetMulitplier(float mult) = 0;
    virtual float GetMulitplier() const = 0;

    virtual void SetAttenuationStart(float start) = 0;
    virtual float GetAttenuationStart() const = 0;

    virtual void SetAttenuationEnd(float end) = 0;
    virtual float GetAttenuationEnd() const = 0;

    //this method should delete and init the lightbehaviour
    virtual void SetBehaviour(LightBehaviour* lightBehaviour) = 0;

	// EDITOR
	virtual HitRegion* GetHitRegion() const = 0;

    // ISerializeable
	virtual void Serialize(Serializer* pSerializer) const = 0;
	virtual void Deserialize(Serializer* pSerializer) = 0;
    virtual DWORD GetUniqueIdentifier() const = 0;
};

