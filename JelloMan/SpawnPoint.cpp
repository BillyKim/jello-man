#include "SpawnPoint.h"
#include "FluidsCharacter.h"

SpawnPoint::SpawnPoint() :
            m_Position(Vector3::Zero), 
            m_mtxWorld(Matrix::CreateTranslation(Vector3::Zero)),
            m_IsSelected(false)
{
}
SpawnPoint::SpawnPoint(const Vector3& position): 
            m_Position(position), 
            m_mtxWorld(Matrix::CreateTranslation(position)),
            m_IsSelected(false)
{
}


SpawnPoint::~SpawnPoint(void)
{
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
    m_Position += add;
    m_mtxWorld = Matrix::CreateTranslation(m_Position);
}
void SpawnPoint::SetPosition(const Vector3& pos)
{
    m_Position = pos;
    m_mtxWorld = Matrix::CreateTranslation(m_Position);
}
Vector3 SpawnPoint::GetPosition() const
{
    return m_Position;
}

/* ISerializable */
void SpawnPoint::Serialize(Serializer* pSerializer) const
{
    pSerializer->GetStream()->storeVector3(m_Position);
}
void SpawnPoint::Deserialize(Serializer* pSerializer)
{
    SetPosition(pSerializer->GetStream()->readVector3());
}
DWORD SpawnPoint::GetUniqueIdentifier() const
{
    return SerializeTypes::SpawnPoint;
}

/* IDrawable */
void SpawnPoint::Draw(RenderContext* /*pRenderContext*/)
{
}