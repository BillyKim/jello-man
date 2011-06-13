#pragma once
#include "Softbody.h"
#include "RenderContext.h"
#include "DeferredPreEffectNormals.h"
#include "ILevelObject.h"
#include "IDrawable.h"
#include "ISerializable.h"

struct UserData;

class SimpleSoftbody : public Softbody, public ILevelObject, public IDrawable, public ISerializable
{
public:
    SimpleSoftbody();
    virtual ~SimpleSoftbody(void);

    virtual IEditorObject* Copy() const { return 0; } //non copyable
    virtual LevelObjectType GetType() const { return LevelObjectType_NormalDraw; };

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

    // Setters
    void SetDiffusePath(tstring path) { m_strDiffusePath = path; }
    void SetSpecPath(tstring path) { m_strSpecPath = path; }
    void SetGlossPath(tstring path) { m_strGlossPath = path; }
    void SetNormalPath(tstring path) { m_strNormalPath = path; }

    void SetModelPath(tstring path) { m_strModelPath = path; }
    void SetPhysXModel(tstring path) { m_strPhysXPath = path; }
    
	//Serialize
	virtual void Serialize(Serializer* pSerializer) const;
	virtual void Deserialize(Serializer* pSerializer);
    virtual DWORD GetUniqueIdentifier() const { return SerializeTypes::SimpleSoftbody; }

private:
    tstring m_strDiffusePath, m_strSpecPath, m_strGlossPath, m_strNormalPath;
    Texture2D *m_pTexDiffuse, *m_pTexSpec, *m_pTexGloss, *m_pTexNormal;

    tstring m_strModelPath, m_strPhysXPath;
    SoftbodyMesh* m_pSBMesh;

    DeferredPreEffectNormals* m_pEffect;

    float m_Timer;

    bool m_bIsSelected;

    UserData* m_pUserData;
};