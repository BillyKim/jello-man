#pragma once
#include "Actor.h"
#include "ILevelObject.h"
#include "ISerializable.h"

#include "Texture2D.h"
#include "Model.h"
#include "DeferredPreEffectNormals.h"
#include "PhysXShape.h"
#include "IInstancible.h"

struct UserData;

class SimpleObject : public Actor, public ILevelObject, public ISerializable, public Instancing::IInstancible, public IDrawable
{
public:
    SimpleObject(bool useForInstancing = false);
    virtual ~SimpleObject(void);

    #pragma region ILevelObject
    #pragma region ILevelObject
    virtual void Init(PhysX* pPhysX);
    virtual void Selected(bool selected) { m_bIsSelected = selected; }
    virtual bool IsSelected() const { return m_bIsSelected; }
    virtual IEditorObject* Copy() const;
    virtual LevelObjectType GetType() const { return m_Type; }
    #pragma endregion

    #pragma region ITransformable
    virtual void Rotate(const Vector3& axis, float angle) { Actor::Rotate(axis, angle); }

    virtual void Translate(const Vector3& add) { Actor::Translate(add); }
    virtual void SetPosition(const Vector3& pos) { Actor::SetPosition(pos); }
    virtual Vector3 GetPosition() const { return Actor::GetPosition(); }

	virtual void Scale(const Vector3& scale) { Actor::Scale(scale); }
    #pragma endregion

    #pragma region IUpdateable
    virtual void Tick(float dTime);
    #pragma endregion

    #pragma region IDrawable
    virtual void Draw(RenderContext* pRenderContext);
    virtual void DrawShadow(RenderContext* pRenderContext);
    #pragma endregion
    #pragma endregion

    #pragma region ISerializeable
    //ISerializeable
	virtual void Serialize(Serializer* pSerializer) const;
	virtual void Deserialize(Serializer* pSerializer);
    virtual DWORD GetUniqueIdentifier() const { return SerializeTypes::SimpleObject; };
    #pragma endregion

	#pragma region IInstancible
    virtual bool IsUsedForInstancing() const { return m_Type == LevelObjectType_InstancedDraw; }
	virtual tstring GetUniqueInstancingID() const;

	virtual Model<VertexPosNormTanTex>* GetModel() const { return m_pModel; }
	virtual Texture2D* GetDiffuse() const { return m_pTexDiffuse; }
	virtual Texture2D* GetSpec() const { return m_pTexSpec; }
	virtual Texture2D* GetGloss() const { return m_pTexGloss; }
	virtual Texture2D* GetNormal() const { return m_pTexNormal; }

	virtual const Matrix* GetMtxWorldPointer() const { return &m_mtxWorldMatrix; }
	#pragma endregion

    void SetDiffusePath(tstring path) { m_strDiffusePath = path; }
    void SetSpecPath(tstring path) { m_strSpecPath = path; }
    void SetGlossPath(tstring path) { m_strGlossPath = path; }
    void SetNormalPath(tstring path) { m_strNormalPath = path; }

    void SetModelPath(tstring path) { m_strModelPath = path; }
    void SetPhysXModel(tstring path) { m_strPhysXModelPath = path; }
    void SetPhysXModel(PhysXShape* shape) { m_pPhysXShape = shape; } //will delete PhysXShape

    void SetRigid(bool rigid) { m_bIsRigid = rigid; }

private:
    tstring m_strDiffusePath, m_strSpecPath, m_strGlossPath, m_strNormalPath;
    Texture2D *m_pTexDiffuse, *m_pTexSpec, *m_pTexGloss, *m_pTexNormal; 

    tstring m_strModelPath;
    Model<VertexPosNormTanTex>* m_pModel;

    tstring m_strPhysXModelPath;
    PhysXShape* m_pPhysXShape;

    DeferredPreEffectNormals* m_pEffect;

    bool m_bIsSelected;
    bool m_bIsRigid;

    LevelObjectType m_Type;
    UserData* m_pUserData;

    //Disable default copy constructor and assignment operator
    SimpleObject(const SimpleObject& copy);
    SimpleObject& operator=(const SimpleObject& other);
};


