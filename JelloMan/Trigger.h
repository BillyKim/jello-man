#pragma once

#include "D3DUtil.h"
#include "Blox2D.h"
#include "Actor.h"
#include "IDrawable.h"
#include "PhysXShape.h"
#include "ILevelObject.h"
#include "ISerializable.h"
#include "PhysXBox.h"
#include "Model.h"
#include "UnlitNoTexEffect.h"
#include "PosColEffect.h"
#include "ITrigger.h"

struct UserData;

class Trigger:	public Actor,
				public IDrawable,
				public ILevelObject,
				public ISerializable,
                public ITrigger
{
public:
	/* CONSTRUCTOR - DESTRUCTOR */
	Trigger();
	virtual ~Trigger();

	/* GENERAL */
	virtual void Init(PhysX* /*pPhysX*/) {}
	virtual void Init(PhysX* pPhysX, const Vector3& dimensions);	

	/* IUpdateable */
	virtual void Tick(float dTime);

	/* IDrawable */
	virtual void Draw(const RenderContext* pRenderContext);
	virtual void DrawShadow(const RenderContext* /*pRenderContext*/) { }

	virtual void Rotate(const Vector3& axis, float angle) { Actor::Rotate(axis, angle); }
	virtual void Translate(const Vector3& add) { Actor::Translate(add); }
	virtual void Scale(const Vector3& scale);

    virtual LevelObjectType GetType() const { return LevelObjectType_Trigger; }

	virtual IEditorObject* Copy() const;

	/* SETTERS */
	void SetTriggerName(const tstring& name);
	void ResetTrigger() { m_bTriggered = false; }

	virtual void SetPosition(const Vector3& pos) { Actor::SetPosition(pos); }
	virtual void Selected(bool selected) { m_bIsSelected = selected; }

	/* GETTERS */
	const tstring& GetTriggerName() const { return m_TriggerName; }
	virtual Vector3 GetPosition() const { return Actor::GetPosition(); }
	virtual bool IsSelected() const { return m_bIsSelected; }
	bool Triggered() const { return m_bTriggered; }

	/* SERIALIZE */
	virtual void Serialize(Serializer* pSerializer) const;
	virtual void Deserialize(Serializer* pSerializer);
    virtual DWORD GetUniqueIdentifier() const { return SerializeTypes::PhysXTrigger; }

	/* PHYSX INTERNAL */
	virtual void OnTriggerEnter(const NxShape& otherShape);
	virtual void OnTriggerLeave(const NxShape& otherShape);

private:
    static int s_Count;

	/* DATAMEMBERS */
	tstring m_TriggerName;

	PhysXShape* m_pTriggerShape;
    UserData* m_pUserData;

	bool m_bIsSelected;
	bool m_bTriggered;

	Vector3 m_Dimensions;

    Model<VertexPosNormTanTex>* m_pSolidModel;
    Model<VertexPos>* m_pContourModel;

	PosColEffect* m_pContourEffect;
    UnlitNoTexEffect* m_pSolidEffect;
};

