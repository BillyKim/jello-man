#include "Trigger.h"
#include "SplineLoader.h"
#include "ContentManager.h"

/* CONSTRUCTOR - DESTRUCTOR */
Trigger::Trigger() :	Actor(),
						m_bIsSelected(false),
						m_TriggerName(_T("Trigger")),
						m_Dimensions(Vector3(0,0,0)),
						m_bTriggered(false),
						m_pModel(0),
						m_pEffect(0)
{
}

Trigger::~Trigger()
{
    delete m_pTriggerShape;
}

/* GENERAL */
void Trigger::Init(PhysX* pPhysX, const Vector3& dimensions)
{
	m_Dimensions = dimensions;

	m_pTriggerShape = new PhysXBox(dimensions, 1);

	m_pTriggerShape->GetShape()->userData = dynamic_cast<ILevelObject*>(this);

	InitActor(pPhysX, *m_pTriggerShape, false, true);	

	m_pActor->userData = this;

	m_pModel = Content->LoadSpline(_T("../Content/Models/box_path.obj"));
	m_pEffect = Content->LoadEffect<PosColEffect>(_T("../Content/Effects/poscol.fx"));
}

void Trigger::Tick(float dTime)
{
	Actor::Tick(dTime);
}

void Trigger::Draw(RenderContext* pRenderContext)
{
	Matrix matProj = pRenderContext->GetCamera()->GetProjection();
	Matrix matView = pRenderContext->GetCamera()->GetView();
	Matrix matWorld = Matrix(m_pActor->getShapes()[0]->getGlobalPose());

	D3D10_VIEWPORT viewPort;
	viewPort.MinDepth = 0.0f;
	viewPort.MaxDepth = 1.0f;
	viewPort.TopLeftX = 0;
	viewPort.TopLeftY = 0;
	viewPort.Height = static_cast<UINT>(BX2D->GetWindowSize().height);
	viewPort.Width = static_cast<UINT>(BX2D->GetWindowSize().width);

	Vector3 vLook = pRenderContext->GetCamera()->GetLook();
	Vector3 length = pRenderContext->GetCamera()->GetPosition() - GetPosition();
	//float l = length.Length() / 100;

	//Vector3 pos2D(Vector3::Project(pSObj->GetPosition(), &viewPort, matProj, matView, matWorld));

	if (vLook.Dot(length) < 0)
	{
		/*BX2D->SetColor(255, 255, 0, 0.5f);
		BX2D->FillEllipse(pos2D.X, pos2D.Y, 4 / l, 4 / l);*/

		NxBounds3 bounds;
		m_pActor->getShapes()[0]->getWorldBounds(bounds);
		

		NxVec3 mid;
		//bounds.getCenter(mid);
		mid.zero();

		NxVec3 dim(m_Dimensions.X,m_Dimensions.Y,m_Dimensions.Z);

		/*bounds.getDimensions(dim);*/

		Vector3 cube[8];

		cube[0] = Vector3(mid.x + (dim.x/2), mid.y + (dim.y/2), mid.z + (dim.z/2));
		cube[1] = Vector3(mid.x - (dim.x/2), mid.y + (dim.y/2), mid.z + (dim.z/2));
		cube[2] = Vector3(mid.x + (dim.x/2), mid.y - (dim.y/2), mid.z + (dim.z/2));
		cube[3] = Vector3(mid.x - (dim.x/2), mid.y - (dim.y/2), mid.z + (dim.z/2));
		cube[4] = Vector3(mid.x + (dim.x/2), mid.y + (dim.y/2), mid.z - (dim.z/2));
		cube[5] = Vector3(mid.x - (dim.x/2), mid.y + (dim.y/2), mid.z - (dim.z/2));
		cube[6] = Vector3(mid.x + (dim.x/2), mid.y - (dim.y/2), mid.z - (dim.z/2));
		cube[7] = Vector3(mid.x - (dim.x/2), mid.y - (dim.y/2), mid.z - (dim.z/2));

		Vector3 cube2D[8];

		for (int i = 0; i < 8; ++i)
		{
			cube2D[i] = Vector3::Project(cube[i], &viewPort, matProj, matView, matWorld);
		}

		Point2D pol1[4];
		pol1[0] = Point2F(cube2D[0].X, cube2D[0].Y);
		pol1[1] = Point2F(cube2D[1].X, cube2D[1].Y);
		pol1[2] = Point2F(cube2D[3].X, cube2D[3].Y);
		pol1[3] = Point2F(cube2D[2].X, cube2D[2].Y);

		Point2D pol2[4];
		pol2[0] = Point2F(cube2D[4].X, cube2D[4].Y);
		pol2[1] = Point2F(cube2D[5].X, cube2D[5].Y);
		pol2[2] = Point2F(cube2D[7].X, cube2D[7].Y);
		pol2[3] = Point2F(cube2D[6].X, cube2D[6].Y);

		Point2D pol3[4];
		pol3[0] = Point2F(cube2D[0].X, cube2D[0].Y);
		pol3[1] = Point2F(cube2D[2].X, cube2D[2].Y);
		pol3[2] = Point2F(cube2D[6].X, cube2D[6].Y);
		pol3[3] = Point2F(cube2D[4].X, cube2D[4].Y);

		Point2D pol4[4];
		pol4[0] = Point2F(cube2D[1].X, cube2D[1].Y);
		pol4[1] = Point2F(cube2D[3].X, cube2D[3].Y);
		pol4[2] = Point2F(cube2D[7].X, cube2D[7].Y);
		pol4[3] = Point2F(cube2D[5].X, cube2D[5].Y);

		Point2D pol5[4];
		pol5[0] = Point2F(cube2D[0].X, cube2D[0].Y);
		pol5[1] = Point2F(cube2D[1].X, cube2D[1].Y);
		pol5[2] = Point2F(cube2D[5].X, cube2D[5].Y);
		pol5[3] = Point2F(cube2D[4].X, cube2D[4].Y);

		Point2D pol6[4];
		pol6[0] = Point2F(cube2D[2].X, cube2D[2].Y);
		pol6[1] = Point2F(cube2D[3].X, cube2D[3].Y);
		pol6[2] = Point2F(cube2D[7].X, cube2D[7].Y);
		pol6[3] = Point2F(cube2D[6].X, cube2D[6].Y);

		float opacity(0.2f);

		if (m_bIsSelected)
			opacity = 1.0f;

		if (!m_bTriggered)
			BX2D->SetColor(100, 100, 255, opacity);
		else
			BX2D->SetColor(255, 0, 255, opacity);

		BX2D->FillPolygon(pol1, 4);
		BX2D->FillPolygon(pol2, 4);
		BX2D->FillPolygon(pol3, 4);
		BX2D->FillPolygon(pol4, 4);
		BX2D->FillPolygon(pol5, 4);
		BX2D->FillPolygon(pol6, 4);
	}

	Matrix scale = Matrix::CreateScale(Vector3(m_Dimensions.X / 100, m_Dimensions.Y / 100, m_Dimensions.Z / 100));

	m_pEffect->SetWorld(scale * matWorld);
	m_pEffect->SetWorldViewProjection(scale * matWorld * matView * matProj);
	m_pEffect->SetColor(Color(1.0f,1.0f,1.0f,.2f));
	m_pModel->Draw(m_pEffect);
}

IEditorObject* Trigger::Copy() const
{
	Trigger* pTrigger = new Trigger();

	pTrigger->Init(m_pPhysX, m_Dimensions);

	tstring newName = m_TriggerName + _T("_COPY");

	pTrigger->SetTriggerName(newName);
	pTrigger->SetPosition(m_pActor->getGlobalPosition());

	return pTrigger;
}

void Trigger::Serialize(Serializer* pSerializer) const
{
	pSerializer->GetStream()->storeTString(m_TriggerName);

    pSerializer->GetStream()->storeVector3(m_Dimensions);

	Actor::Serialize(pSerializer);
}
void Trigger::Deserialize(Serializer* pSerializer)
{
	m_TriggerName = pSerializer->GetStream()->readTString();

    Vector3 dim(pSerializer->GetStream()->readVector3());

	Init(pSerializer->GetPhysX(), dim);

	Actor::Deserialize(pSerializer);
}

/* PHYSX INTERNAL */
void Trigger::OnTrigger()
{
	m_bTriggered = true;
}