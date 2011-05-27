#pragma once
#include "Softbody.h"
#include "RenderContext.h"
#include "DeferredPreEffect.h"
#include "ILevelObject.h"
#include "FollowCamera.h"

class SoftbodyCharacter : public Softbody, public ILevelObject
{
public:
    SoftbodyCharacter(const Vector3& pos, Graphics::Camera::FollowCamera* pCamera);
    virtual ~SoftbodyCharacter(void);

    //ITransformable
    virtual void Rotate(const Vector3& axis, float angle) { Softbody::Rotate(axis, angle); }

    virtual void Translate(const Vector3& add) { Softbody::Translate(add); }
    virtual void SetPosition(const Vector3& pos) { Softbody::SetPosition(pos); }
    virtual Vector3 GetPosition() const { return Softbody::GetPosition(); }

    virtual void Scale(const Vector3& scale) { Softbody::Scale(scale); }

    //ILevelobject
    virtual void Init(PhysX* pPhysX);
    virtual void Selected(bool selected);
    virtual bool IsSelected() const { return m_bIsSelected; }

    //IUpdateable
	virtual void Tick(float dTime);

    //IDrawable
    virtual void Draw(RenderContext* pRenderContext);
    virtual void DrawShadow(RenderContext* pRenderContext);

private:
    DeferredPreEffect* m_pEffect;

    Vector3 m_vPosition;

    Texture2D* m_pDiffuseMap, *m_pSpecMap, *m_pGlossMap;

    float m_RotSpeed;

    Graphics::Camera::FollowCamera* m_pCamera;

    bool m_bIsSelected;
};
