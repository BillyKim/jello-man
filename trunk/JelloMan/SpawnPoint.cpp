#include "SpawnPoint.h"
#include "FluidsCharacter.h"
#include "ContentManager.h"
#include "PhysXBox.h"
#include "Vector3.h"
#include "UserData.h"

SpawnPoint::SpawnPoint() :
            m_IsSelected(false),
            m_pModel(0),
            m_pUserData(0)
{
}


SpawnPoint::~SpawnPoint(void)
{
    delete m_pUserData;
}

void SpawnPoint::Init(PhysX* pPhysX)
{
    m_pModel = Content->LoadModel(_T("../Content/Models/engine/spawnpoint.binobj"));
    m_pEffect = Content->LoadEffect<UnlitTexEffect>(_T("../Content/Effects/unlit-tex.fx"));
    m_pTexture = Content->LoadTexture2D(_T("../Content/Textures/engine/tex_spawnpoint.png"));

    PhysXBox box(Vector3(0.591f, 0.6f, 0.683f), 10);
    m_pUserData = new UserData(UserDataFlag_IsPickable, dynamic_cast<ILevelObject*>(this));
    box.GetShape()->userData = m_pUserData;
    InitActor(pPhysX, box, false, false);
    SetPosition(Vector3::Zero);
}

/* ILevelObject */
void SpawnPoint::Selected(bool selected)
{
    m_IsSelected = selected;
}
bool SpawnPoint::IsSelected() const
{
    return m_IsSelected;
}

/* ITransformable */
void SpawnPoint::Translate(const Vector3& add)
{
    Actor::Translate(add);
}
void SpawnPoint::SetPosition(const Vector3& pos)
{
    Actor::SetPosition(pos);
}
Vector3 SpawnPoint::GetPosition() const
{
    return Actor::GetPosition();
}

/* ISerializable */
void SpawnPoint::Serialize(Serializer* pSerializer) const
{
    pSerializer->GetStream()->storeVector3(GetPosition());
}
void SpawnPoint::Deserialize(Serializer* pSerializer)
{
    Init(pSerializer->GetPhysX());
    SetPosition(pSerializer->GetStream()->readVector3());
}
DWORD SpawnPoint::GetUniqueIdentifier() const
{
    return SerializeTypes::SpawnPoint;
}

/* IDrawable */
void SpawnPoint::Draw(RenderContext* pRenderContext)
{
    m_pEffect->SetWorldViewProjection(m_mtxWorldMatrix * pRenderContext->GetCamera()->GetViewProjection());
    m_pEffect->SetDiffuseMap(m_pTexture);

    m_pModel->Draw(m_pEffect);
}