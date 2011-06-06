#pragma once
#include "Vector3.h"
#include "Matrix.h"
#include "ILevelObject.h"
#include "ISerializable.h"
#include "IDrawable.h"
#include "ICharacter.h"

class SpawnPoint : public ILevelObject, public ISerializable, public IDrawable
{
public:
    SpawnPoint();
    SpawnPoint(const Vector3& position);
    virtual ~SpawnPoint(void);
    
    /* ILevelObject */
    virtual void Selected(bool selected);
    virtual bool IsSelected() const;

    /* ITransformable */
	virtual void Translate(const Vector3& add);
	virtual void SetPosition(const Vector3& pos);
	virtual Vector3 GetPosition() const;

    /* ISerializable */
	virtual void Serialize(Serializer* pSerializer) const;
	virtual void Deserialize(Serializer* pSerializer);
    virtual DWORD GetUniqueIdentifier() const;

    /* IDrawable */
    virtual void Draw(RenderContext* pRenderContext);

private:
    Vector3 m_Position;
    Matrix m_mtxWorld;

    bool m_IsSelected;

  /* Disabled because not used */
    /* ILevelObject */
    virtual void Init(PhysX* /*pPhysX*/) {};
    virtual IEditorObject* Copy() const { return 0; }; //only one spawnpoint allowed
    
    /* ITransformable */
    virtual void Rotate(const Vector3& /*axis*/, float /*angle*/) {};
    virtual void Scale(const Vector3& /*scale*/) {};
    
    /* IUpdateable */
    virtual void Tick(float /*dTime*/) {};

    /* IDrawable */
    virtual void DrawShadow(RenderContext* /*pRenderContext*/) {};

    /* Disabling default copy and assignment operator */
    SpawnPoint(const SpawnPoint&);
    SpawnPoint& operator=(const SpawnPoint&);
};

