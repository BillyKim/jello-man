#pragma once
#include "Softbody.h"
#include "RenderContext.h"
#include "DeferredPreEffect.h"
#include "ILevelObject.h"

class TestSoftbody : public Softbody, public ILevelObject
{
public:
    TestSoftbody(const Vector3& pos);
    virtual ~TestSoftbody(void);

    virtual IEditorObject* Copy() const { return 0; } //non copyable

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
    virtual void Draw(const RenderContext* pRenderContext);
    virtual void DrawShadow(const RenderContext* pRenderContext);

private:
    DeferredPreEffect* m_pEffect;

    Vector3 m_vPosition;

    Texture2D* m_pDiffuseMap, *m_pSpecMap, *m_pGlossMap;

    bool m_bIsSelected;
};

