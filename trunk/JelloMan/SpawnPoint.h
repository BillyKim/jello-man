#pragma once
#include "Vector3.h"
#include "Matrix.h"
#include "ILevelObject.h"
#include "ISerializable.h"
#include "IDrawable.h"
#include "ICharacter.h"
#include "Model.h"
#include "UnlitTexEffect.h"
#include "Actor.h"

struct UserData;

class SpawnPoint : public Actor, public ILevelObject, public ISerializable, public IDrawable
{
public:
    SpawnPoint();
    virtual ~SpawnPoint(void);
    
    virtual void Init(PhysX* pPhysX);

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
    bool m_IsSelected;

    UserData* m_pUserData;

    Model<VertexPosNormTanTex>* m_pModel;
    UnlitTexEffect* m_pEffect;
    Texture2D* m_pTexture;


  /* Disabled because not used */
    /* ILevelObject */
    virtual IEditorObject* Copy() const { return 0; }; //only one spawnpoint allowed
    virtual LevelObjectType GetType() const { return LevelObjectType_Spawnpoint; }
    
    /* ITransformable */
    virtual void Rotate(const Vector3& /*axis*/, float /*angle*/) {};
    virtual void Scale(const Vector3& /*scale*/) {};
    
    /* IUpdateable */
    virtual void Tick(float dTime) 
    {
        Actor::Tick(dTime);
    };

    /* IDrawable */
    virtual void DrawShadow(RenderContext* /*pRenderContext*/) {};

    /* Disabling default copy and assignment operator */
    SpawnPoint(const SpawnPoint&);
    SpawnPoint& operator=(const SpawnPoint&);
};

