#include "SimpleObject.h"
#include "ContentManager.h"
#include "PhysXMesh.h"
#include "UserData.h"

SimpleObject::SimpleObject(bool useForInstancing): Actor(),
    m_bIsSelected(false), 
    m_strDiffusePath(_T("")),
    m_strSpecPath(_T("")),
    m_strGlossPath(_T("")),
    m_strNormalPath(_T("")),
    m_strModelPath(_T("")),
    m_strPhysXModelPath(_T("")),
    m_pPhysXShape(0),
    m_pTexDiffuse(0), m_pTexSpec(0), m_pTexGloss(0), m_pTexNormal(0),
    m_pModel(0), m_pEffect(0),
    m_bIsRigid(false),
    m_Type(useForInstancing?LevelObjectType_InstancedDraw:LevelObjectType_NormalDraw),
    m_pUserData(0)

{
}
SimpleObject::~SimpleObject(void)
{
    delete m_pPhysXShape;
    delete m_pUserData;
}

#pragma region ILevelObject
#pragma region ILevelObject
void SimpleObject::Init(PhysX* pPhysX)
{
    #pragma region Textures
    if (m_strDiffusePath == _T(""))
        m_pTexDiffuse = Content->LoadTexture2D(DefaultTextureType_Gray);
    else
        m_pTexDiffuse = Content->LoadTexture2D(m_strDiffusePath);

    if (m_strSpecPath == _T(""))
        m_pTexSpec = Content->LoadTexture2D(DefaultTextureType_Gray);
    else
        m_pTexSpec = Content->LoadTexture2D(m_strSpecPath);

    if (m_strGlossPath == _T(""))
        m_pTexGloss = Content->LoadTexture2D(DefaultTextureType_Gray);
    else
        m_pTexGloss = Content->LoadTexture2D(m_strGlossPath);

    if (m_strNormalPath == _T(""))
        m_pTexNormal = Content->LoadTexture2D(DefaultTextureType_Normal);
    else
        m_pTexNormal = Content->LoadTexture2D(m_strNormalPath);
    #pragma endregion
    #pragma region Model
    m_pModel = Content->LoadModel(m_strModelPath);
    #pragma endregion

    if (m_pPhysXShape == 0 && m_strPhysXModelPath != _T(""))
    {
        m_pPhysXShape = new PhysXMesh(pPhysX, string(m_strPhysXModelPath.cbegin(), m_strPhysXModelPath.cend()), 10);
    }
    m_pEffect = Content->LoadEffect<DeferredPreEffectNormals>(_T("../Content/Effects/predeferredNormal.fx"));
    
    m_pUserData = new UserData(UserDataFlag_IsPickable, dynamic_cast<ILevelObject*>(this));
    m_pPhysXShape->GetShape()->userData = m_pUserData;
    InitActor(pPhysX, *m_pPhysXShape, m_bIsRigid);
}
tstring SimpleObject::GetUniqueInstancingID() const
{
	tstring id = m_strDiffusePath + m_strSpecPath + m_strGlossPath + m_strNormalPath + m_strModelPath;
	return id;
}
#pragma endregion


IEditorObject* SimpleObject::Copy() const
{
    SimpleObject* obj = new SimpleObject(true);
    obj->SetDiffusePath(m_strDiffusePath);
    obj->SetSpecPath(m_strSpecPath);
    obj->SetGlossPath(m_strGlossPath);
    obj->SetNormalPath(m_strNormalPath);

    obj->SetModelPath(m_strModelPath);
    obj->SetPhysXModel(m_pPhysXShape->Copy());

    obj->SetRigid(m_bIsRigid);

    obj->Init(m_pPhysX);

    if (m_pActor->isDynamic())
    {
        obj->m_pActor->setAngularVelocity(m_pActor->getAngularVelocity());
        obj->m_pActor->setLinearVelocity(m_pActor->getLinearVelocity());
    }
    
    m_pPhysX->GetPhysXLock().lock();
    obj->m_pActor->setGlobalPose(m_pActor->getGlobalPose());
    m_pPhysX->GetPhysXLock().unlock();

    return obj;
}

#pragma region IUpdateable
void SimpleObject::Tick(float dTime)
{
    Actor::Tick(dTime);
}
#pragma endregion

#pragma region IDrawable
void SimpleObject::Draw(RenderContext* pRenderContext)
{
    m_pEffect->SetDiffuseMap(m_pTexDiffuse);
    m_pEffect->SetSpecMap(m_pTexSpec);
    m_pEffect->SetGlossMap(m_pTexGloss);
    m_pEffect->SetNormalMap(m_pTexNormal);
    m_pEffect->SetWorld(m_mtxWorldMatrix);
    m_pEffect->SetWorldViewProjection(m_mtxWorldMatrix * pRenderContext->GetCamera()->GetViewProjection());
    m_pEffect->Selected(m_bIsSelected);
    m_pModel->Draw(m_pEffect);
}
void SimpleObject::DrawShadow(RenderContext* pRenderContext)
{
    pRenderContext->GetPreShadowEffect()->SetWorldViewProjection(m_mtxWorldMatrix * pRenderContext->GetCamera()->GetViewProjection());
    m_pModel->Draw(pRenderContext->GetPreShadowEffect());
}
#pragma endregion
#pragma endregion

#pragma region ISerializeable
void SimpleObject::Serialize(Serializer* pSerializer) const
{
    pSerializer->GetStream()->storeTString(m_strDiffusePath);
    pSerializer->GetStream()->storeTString(m_strSpecPath);
    pSerializer->GetStream()->storeTString(m_strGlossPath);
    pSerializer->GetStream()->storeTString(m_strNormalPath);
    pSerializer->GetStream()->storeTString(m_strModelPath);
    
    pSerializer->Serialize(m_pPhysXShape);

    pSerializer->GetStream()->storeByte(m_bIsRigid?1:0);

    Actor::Serialize(pSerializer);
}
void SimpleObject::Deserialize(Serializer* pSerializer)
{
    m_strDiffusePath = pSerializer->GetStream()->readTString();
    m_strSpecPath = pSerializer->GetStream()->readTString();
    m_strGlossPath = pSerializer->GetStream()->readTString();
    m_strNormalPath = pSerializer->GetStream()->readTString();
    m_strModelPath = pSerializer->GetStream()->readTString();
     
    m_pPhysXShape = dynamic_cast<PhysXShape*>(pSerializer->Deserialize(PhysXShape::GetShape));

    m_bIsRigid = pSerializer->GetStream()->readByte() == 1;

    Init(pSerializer->GetPhysX());

    Actor::Deserialize(pSerializer);
}
#pragma endregion
