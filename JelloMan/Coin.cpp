#include "Coin.h"
#include "UserData.h"
#include "ContentManager.h"
#include "ITriggerable.h"
#include "PhysXSphere.h"

Coin::Coin(void): 
   m_strDiffusePath(_T("")), m_strSpecPath(_T("")), m_strGlossPath(_T("")), m_strNormalPath(_T("")), 
   m_strModelPath(_T("")),
   m_pTexDiffuse(0), m_pTexSpec(0), m_pTexGloss(0), m_pTexNormal(0),
   m_pModel(0), m_bIsSelected(false),
   m_TriggerName(_T("coin"))
{
}


Coin::~Coin(void)
{
    delete m_pUserData;
}

/* GENERAL */
void Coin::Init(PhysX* pPhysX)
{ 
    m_strDiffusePath = _T("../Content/Textures/Coin/tex_coin_diffuse.png");
    m_strSpecPath = _T("../Content/Textures/Coin/tex_coin_spec.png");
    m_strGlossPath = _T("../Content/Textures/Coin/tex_coin_spec.png");
    m_strNormalPath = _T("../Content/Textures/Coin/tex_coin_normal.png");
    m_strModelPath = _T("../Content/Models/Coin/prop_coin.binobj");

    m_pTexDiffuse = Content->LoadTexture2D(m_strDiffusePath);
    m_pTexSpec = Content->LoadTexture2D(m_strSpecPath); 
    m_pTexGloss = Content->LoadTexture2D(m_strGlossPath); 
    m_pTexNormal = Content->LoadTexture2D(m_strNormalPath);

    m_pModel = Content->LoadModel(m_strModelPath);

    m_pUserData = new UserData(UserDataFlag_IsPickable, dynamic_cast<ILevelObject*>(this));

    PhysXSphere sphere(0.3f, 0.1f);
    sphere.GetShape()->userData = m_pUserData;
    InitActor(pPhysX, sphere, false, true);
    m_pActor->userData = dynamic_cast<ITrigger*>(this);
}

/* IUpdateable */
void Coin::Tick(float dTime)
{
    Rotate(Vector3::Up, dTime);
}

IEditorObject* Coin::Copy() const
{
    Coin* pCoin = new Coin();
    pCoin->Init(m_pPhysX);
    pCoin->SetPosition(GetPosition());
    return pCoin;
}

/* SERIALIZE */
void Coin::Serialize(Serializer* pSerializer) const
{
    pSerializer->GetStream()->storeVector3(GetPosition());
}
void Coin::Deserialize(Serializer* pSerializer)
{
    Init(pSerializer->GetPhysX());
    SetPosition(pSerializer->GetStream()->readVector3());
}

/* PHYSX INTERNAL */
void Coin::OnTriggerEnter(const NxShape& otherShape)
{ 
    if (otherShape.userData != 0)
    {
        void* temp = otherShape.userData;
        UserData* pData = static_cast<UserData*>(temp);
        if (pData->flags & UserDataFlag_IsTriggerable)
        {
            ITriggerable* pObj = static_cast<ITriggerable*>(pData->object);
            pObj->OnTriggerEnter(this);
        }
    }
}
void Coin::OnTriggerLeave(const NxShape& otherShape)
{
}


tstring Coin::GetUniqueInstancingID() const
{
	tstring id = m_strDiffusePath + m_strSpecPath + m_strGlossPath + m_strNormalPath + m_strModelPath;
	return id;
}