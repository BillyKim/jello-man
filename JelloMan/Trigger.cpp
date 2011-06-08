#include "Trigger.h"
#include "SplineLoader.h"
#include "ContentManager.h"
#include "ITriggerable.h"
#include "UserData.h"

int Trigger::s_Count = 0;

/* CONSTRUCTOR - DESTRUCTOR */
Trigger::Trigger() :	Actor(),
						m_bIsSelected(false),
						m_TriggerName(_T("Trigger")),
						m_Dimensions(Vector3(0,0,0)),
						m_bTriggered(false),
                        m_pContourModel(0),
                        m_pSolidModel(0),
						m_pContourEffect(0),
                        m_pSolidEffect(0),
                        m_pUserData(0)
{
}

Trigger::~Trigger()
{
    delete m_pTriggerShape;
    delete m_pUserData;
}

/* GENERAL */
void Trigger::Init(PhysX* pPhysX, const Vector3& dimensions)
{
	m_Dimensions = dimensions;

	m_pTriggerShape = new PhysXBox(dimensions, 1);

    m_pUserData = new UserData(UserDataFlag_IsPickable, dynamic_cast<ILevelObject*>(this));
	m_pTriggerShape->GetShape()->userData = m_pUserData;

	InitActor(pPhysX, *m_pTriggerShape, false, true);	

	m_pActor->userData = this;

    m_pContourModel = Content->LoadSpline(_T("../Content/Models/box_path.obj"));
    m_pSolidModel = Content->LoadModel(_T("../Content/Models/box1.binobj"));
	m_pContourEffect = Content->LoadEffect<PosColEffect>(_T("../Content/Effects/poscol.fx"));
    m_pSolidEffect = Content->LoadEffect<UnlitNoTexEffect>(_T("../Content/Effects/unlit-notex.fx"));
}

void Trigger::Tick(float dTime)
{
	Actor::Tick(dTime);
}

void Trigger::Draw(const RenderContext* pRenderContext)
{
	Matrix matWorld = Matrix(m_pActor->getGlobalPose());

    #pragma region junk 
	D3D10_VIEWPORT viewPort;
	viewPort.MinDepth = 0.0f;
	viewPort.MaxDepth = 1.0f;
	viewPort.TopLeftX = 0;
	viewPort.TopLeftY = 0;
	viewPort.Height = static_cast<UINT>(BX2D->GetWindowSize().height);
	viewPort.Width = static_cast<UINT>(BX2D->GetWindowSize().width);

	Vector3 vLook = pRenderContext->GetCamera()->GetLook();
	Vector3 length = pRenderContext->GetCamera()->GetPosition() - GetPosition();
	float l = 1 / length.Length() * 100;

    Vector3 pos2D(Vector3::Project(GetPosition(), &viewPort, pRenderContext->GetCamera()->GetProjection(), 
                                            pRenderContext->GetCamera()->GetView(), Matrix::Identity));

	if (vLook.Dot(length) < 0)
	{
        BX2D->SetColor(255, 255, 255);
        BX2D->SetTransform(Matrix3x2F::Scale(l, l, Point2F(pos2D.X, pos2D.Y)));
        BX2D->DrawString(GetTriggerName(), pos2D.X, pos2D.Y);
        BX2D->ResetTransform();
    }
	
    #pragma endregion

    m_pSolidEffect->SetWorldViewProjection(
                                Matrix::CreateScale(Vector3(m_Dimensions.X, m_Dimensions.Y, m_Dimensions.Z)) * 
                                matWorld * 
                                pRenderContext->GetCamera()->GetViewProjection());

	if (!m_bTriggered)
		m_pSolidEffect->SetColor(Color(0.6f,0.6f,1.0f,0.5f));
	else
		m_pSolidEffect->SetColor(Color(1.0f,0.2f,1.0f,0.5f));

    m_pSolidModel->Draw(m_pSolidEffect);
    
	m_pContourEffect->SetWorldViewProjection(
                                Matrix::CreateScale(Vector3(m_Dimensions.X, m_Dimensions.Y, m_Dimensions.Z)*0.01f) * 
                                matWorld * 
                                pRenderContext->GetCamera()->GetViewProjection());
	m_pContourEffect->SetColor(Color(1.0f, 1.0f, 1.0f, 1.0f));
    m_pContourModel->Draw(m_pContourEffect);
}

IEditorObject* Trigger::Copy() const
{
	Trigger* pTrigger = new Trigger();

	pTrigger->Init(m_pPhysX, m_Dimensions);

    tstring newName;
    newName = m_TriggerName.substr(0, m_TriggerName.rfind('_'));

	pTrigger->SetTriggerName(newName);
	pTrigger->SetPosition(m_pActor->getGlobalPosition());

	return pTrigger;
}

void Trigger::SetTriggerName(const tstring& name)
{
    tstringstream stream;
    stream << name << "_" << ++s_Count;
    m_TriggerName = stream.str();
}
void Trigger::Serialize(Serializer* pSerializer) const
{
	pSerializer->GetStream()->storeTString(m_TriggerName);

    pSerializer->GetStream()->storeVector3(m_Dimensions);

	Actor::Serialize(pSerializer);
}
void Trigger::Deserialize(Serializer* pSerializer)
{
	tstring triggerName = pSerializer->GetStream()->readTString();

	tstring newName;
    newName = triggerName.substr(0, triggerName.rfind('_'));

	SetTriggerName(newName);

    Vector3 dim(pSerializer->GetStream()->readVector3());

	Init(pSerializer->GetPhysX(), dim);

	Actor::Deserialize(pSerializer);	
}

/* PHYSX INTERNAL */
void Trigger::OnTriggerEnter(const NxShape& otherShape)
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

	m_bTriggered = true;
}

void Trigger::OnTriggerLeave(const NxShape& otherShape)
{
    if (otherShape.userData != 0)
    {
        void* temp = otherShape.userData;
        UserData* pData = static_cast<UserData*>(temp);
        if (pData->flags & UserDataFlag_IsTriggerable)
        {
            ITriggerable* pObj = static_cast<ITriggerable*>(pData->object);
            pObj->OnTriggerLeave(this);
        }
    }
}

void Trigger::Scale(const Vector3& scale)
{
	m_Dimensions *= scale;

	delete m_pTriggerShape;
	m_pTriggerShape = new PhysXBox(m_Dimensions, 1);

	delete m_pUserData;
    m_pUserData = new UserData(UserDataFlag_IsPickable, dynamic_cast<ILevelObject*>(this));
	m_pTriggerShape->GetShape()->userData = m_pUserData;

	m_pPhysX->GetScene()->releaseActor(*m_pActor);
	m_pActor = 0;
	InitActor(m_pPhysX, *m_pTriggerShape, false, true);	

	m_pActor->userData = this;
}