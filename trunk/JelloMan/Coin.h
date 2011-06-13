#pragma once

#include "D3DUtil.h"
#include "Blox2D.h"
#include "Actor.h"
#include "IDrawable.h"
#include "PhysXShape.h"
#include "ILevelObject.h"
#include "ISerializable.h"
#include "Model.h"
#include "ITrigger.h"
#include "IInstancible.h"

struct UserData;

class Coin:	public Actor,
			public ILevelObject,
			public ISerializable,
            public ITrigger,
            public Instancing::IInstancible
{
public:
	/* CONSTRUCTOR - DESTRUCTOR */
	Coin();
	virtual ~Coin();

	/* GENERAL */
	virtual void Init(PhysX* pPhysX);

	/* IUpdateable */
	virtual void Tick(float dTime);

	virtual void Rotate(const Vector3& axis, float angle) { Actor::Rotate(axis, angle); }
	virtual void Translate(const Vector3& add) { Actor::Translate(add); }
    virtual void Scale(const Vector3& scale) {}

    virtual LevelObjectType GetType() const { return LevelObjectType_InstancedDraw; }

    virtual IEditorObject* Copy() const;

	/* SETTERS */
	virtual void SetPosition(const Vector3& pos) { Actor::SetPosition(pos); }
	virtual void Selected(bool selected) { m_bIsSelected = selected; }

	/* GETTERS */
	const tstring& GetTriggerName() const { return m_TriggerName; }
	virtual Vector3 GetPosition() const { return Actor::GetPosition(); }
	virtual bool IsSelected() const { return m_bIsSelected; }

	/* SERIALIZE */
	virtual void Serialize(Serializer* pSerializer) const;
	virtual void Deserialize(Serializer* pSerializer);
    virtual DWORD GetUniqueIdentifier() const { return SerializeTypes::Coin; }

	/* PHYSX INTERNAL */
	virtual void OnTriggerEnter(const NxShape& otherShape);
	virtual void OnTriggerLeave(const NxShape& otherShape);

    /* IInstancible */
    virtual bool IsUsedForInstancing() const { return true; }
    virtual tstring GetUniqueInstancingID() const;

    virtual Model<VertexPosNormTanTex>* GetModel() const { return m_pModel; }
    virtual Texture2D* GetDiffuse() const { return m_pTexDiffuse; }
    virtual Texture2D* GetSpec() const { return m_pTexSpec; }
    virtual Texture2D* GetGloss() const { return m_pTexGloss; }
    virtual Texture2D* GetNormal() const { return m_pTexNormal; }

    virtual const Matrix* GetMtxWorldPointer() const { return &m_mtxWorldMatrix; }

private:
	/* DATAMEMBERS */
	tstring m_TriggerName;

    UserData* m_pUserData;

	bool m_bIsSelected;
    
    tstring m_strDiffusePath, m_strSpecPath, m_strGlossPath, m_strNormalPath;
    Texture2D *m_pTexDiffuse, *m_pTexSpec, *m_pTexGloss, *m_pTexNormal; 

    tstring m_strModelPath;
    Model<VertexPosNormTanTex>* m_pModel;

    //Disabling default copy constructor and assignment operator
    Coin(const Coin&);
    Coin& operator=(const Coin&);
};

