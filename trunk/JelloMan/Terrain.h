#pragma once
#include "Actor.h"
#include "ILevelObject.h"
#include "IDrawable.h"
#include "Model.h"

class RenderContext;
class PhysX;
class DeferredPreEffectNormals;
class Texture2D;
struct UserData;

class Terrain : public Actor, public ILevelObject, public IDrawable
{
public:
    Terrain(void);
    virtual ~Terrain(void);

    virtual void Init(PhysX* pPhysX);
    virtual void Selected(bool selected) { m_bIsSelected = selected; }
    virtual bool IsSelected() const { return m_bIsSelected; }
    virtual IEditorObject* Copy() const;
    virtual LevelObjectType GetType() const { return LevelObjectType_NormalDraw; } 

    // ITransformable
    virtual void Rotate(const Vector3& axis, float angle) { Actor::Rotate(axis, angle); }

    virtual void Translate(const Vector3& add) { return Actor::Translate(add); }
    virtual void SetPosition(const Vector3& pos) { return Actor::SetPosition(pos); }
    virtual Vector3 GetPosition() const { return Actor::GetPosition(); }

    virtual void Scale(const Vector3& /*scale*/) {}

    // IUpdateable
    virtual void Tick(float dTime) {}
   
    // IDrawable
    virtual void Draw(const RenderContext* pRenderContext);
    virtual void DrawShadow(const RenderContext* pRenderContext);

    // Setters
    void SetDiffusePath(tstring path) { m_strDiffusePath = path; }
    void SetSpecPath(tstring path) { m_strSpecPath = path; }
    void SetGlossPath(tstring path) { m_strGlossPath = path; }
    void SetNormalPath(tstring path) { m_strNormalPath = path; }
    void SetTerrainPath(tstring path) { m_strTerrainPath = path; }

private:
    tstring m_strDiffusePath, m_strSpecPath, m_strGlossPath, m_strNormalPath;
    tstring m_strTerrainPath;

    bool m_bIsSelected;

    UserData* m_pUserData;

    Texture2D* m_pTexDiffuse;
    Texture2D* m_pTexSpec;
    Texture2D* m_pTexGloss;
    Texture2D* m_pTexNorm;

    DeferredPreEffectNormals* m_pEffect;

    Model<VertexPosNormTanTex>* m_pModel;
};

