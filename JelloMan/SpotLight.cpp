#include "SpotLight.h"
#include "LightBehaviourNormal.h"
#include "ContentManager.h"

SpotLight::SpotLight():
            m_Rotation(Matrix::Identity),
			m_IsEnabled(true),
			m_IsSelected(false),
			m_ShadowMap(0),
            m_pLightBehaviour(0),
            m_pShadowCamera(0),
            m_OpeningsAngle(0.0f),
            m_vUp(0, 1, 0),
			m_pHitRegion(0),
			m_pSpotLightImage(0),
			m_ShadowMapType(ShadowMapType_None),
			m_pAttenuationSpline(0),
			m_pEffect(0),
			m_pAttenuationSpline2(0),
			m_pAttenuationSpline3(0)
{
    SetBehaviour(new LightBehaviourNormal());

    m_Desc.attenuationStart = 0;
    m_Desc.attenuationEnd = 500;
    m_Desc.color = Color(1.0f, 1.0f, 1.0f);
    m_Desc.direction = Vector3(1, 0, 0);
    m_Desc.multiplier = 1.0f;
    m_Desc.position = Vector3(0, 0, 0);
    m_Desc.power = 1.0f;
    m_StartDesc = m_Desc;

	m_pSpotLightImage = Content->LoadImage(_T("../Content/Images/Editor/slight.png"));

	m_pAttenuationSpline = Content->LoadSpline(_T("../Content/Models/cone_path.obj"), Color(255.0f,255.0f,255.0f,1.0f));
	m_pAttenuationSpline2 = Content->LoadSpline(_T("../Content/Models/arrow_path.obj"), Color(255.0f,255.0f,255.0f,1.0f));
	m_pAttenuationSpline3 = Content->LoadSpline(_T("../Content/Models/orb_path.obj"), Color(255.0f,255.0f,255.0f,1.0f));
	m_pEffect = Content->LoadEffect<PosColEffect>(_T("../Content/Effects/poscol.fx"));
}
SpotLight::SpotLight(const SpotLightDesc& desc):
            m_Rotation(Matrix::Identity),
			m_IsEnabled(true),
			m_IsSelected(false),
			m_ShadowMap(0),
            m_pLightBehaviour(0),
            m_pShadowCamera(0),
            m_OpeningsAngle(0.0f),
            m_vUp(0, 1, 0),
			m_pHitRegion(0),
			m_Desc(desc),
			m_pSpotLightImage(0),
			m_ShadowMapType(ShadowMapType_None),
			m_pAttenuationSpline(0),
			m_pEffect(0),
			m_pAttenuationSpline2(0),
			m_pAttenuationSpline3(0)
{
	SetBehaviour(new LightBehaviourNormal());
	m_StartDesc = m_Desc;

	m_pSpotLightImage = Content->LoadImage(_T("../Content/Images/Editor/slight.png"));

	m_pAttenuationSpline = Content->LoadSpline(_T("../Content/Models/cone_path.obj"), Color(255.0f,255.0f,255.0f,1.0f));
	m_pAttenuationSpline2 = Content->LoadSpline(_T("../Content/Models/arrow_path.obj"), Color(255.0f,255.0f,255.0f,1.0f));
	m_pAttenuationSpline3 = Content->LoadSpline(_T("../Content/Models/orb_path.obj"), Color(255.0f,255.0f,255.0f,1.0f));
	m_pEffect = Content->LoadEffect<PosColEffect>(_T("../Content/Effects/poscol.fx"));
}
SpotLight::~SpotLight()
{
	delete m_ShadowMap;
    delete m_pLightBehaviour;
    delete m_pShadowCamera;
	delete m_pHitRegion;
}

void SpotLight::InitGame()
{
    m_StartDesc = m_Desc;
}
void SpotLight::InitEditor()
{
    m_Desc = m_StartDesc;
}

void SpotLight::Tick(float dTime)
{
    m_pLightBehaviour->Tick(dTime);
}
void SpotLight::Draw(const RenderContext* rc)
{
	D3DXMATRIX matProj = rc->GetCamera()->GetProjection();
	D3DXMATRIX matView = rc->GetCamera()->GetView();

	D3DXMATRIX matIdent;
	D3DXMatrixIdentity(&matIdent);

	Vector3 vLook = rc->GetCamera()->GetLook();
	Vector3 vRight = rc->GetCamera()->GetRight();

	D3D10_VIEWPORT viewP;
	viewP.TopLeftX = 0;
	viewP.TopLeftY = 0;
	viewP.Width = (UINT)BX2D->GetWindowSize().width;
	viewP.Height = (UINT)BX2D->GetWindowSize().height;
	viewP.MinDepth = 0.0f;
	viewP.MaxDepth = 1.0f;

	int size = 10;
	
	// VIEWPORT PROJECTION
	D3DXVECTOR3 pos = D3DXVECTOR3(m_Desc.position.X, m_Desc.position.Y, m_Desc.position.Z);
	
	Vector3 length = rc->GetCamera()->GetPosition() - Vector3(pos);
	float l = length.Length();
	l *= 0.001f;

	D3DXVECTOR3 pos2D;
	D3DXVec3Project(&pos2D, &pos, &viewP, &matProj, &matView, &matIdent);

	// HITRECT
	SafeDelete(m_pHitRegion);

	m_pHitRegion = new HitRegion(	
		HitRegion::TYPE_ELLIPSE,
		pos2D.x,
		pos2D.y, 
		size / l,
		size / l);

	if (vLook.Dot(length) < 0)
	{
		// DRAW
		if (m_pHitRegion->HitTest(CONTROLS->GetMousePos()) || m_IsSelected)
		{
			BX2D->SetColor(255,255,255);
			BX2D->FillEllipse(pos2D.x, pos2D.y, size / l, size / l);
			BX2D->SetColor(30,30,30);
			BX2D->DrawEllipse(pos2D.x, pos2D.y, size / l, size / l, 2.0f / l);
			BX2D->SetColor(255,0,255);
			BX2D->DrawEllipse(pos2D.x, pos2D.y, (size * 4) / l, (size * 4) / l, 2.0f);
		}
		else
		{
			BX2D->SetColor(static_cast<int>(m_Desc.color.R * 255), static_cast<int>(m_Desc.color.G * 255), static_cast<int>(m_Desc.color.B * 255));
			BX2D->FillEllipse(pos2D.x, pos2D.y, size / l, size / l);
			BX2D->SetColor(230,230,230);
			BX2D->DrawEllipse(pos2D.x, pos2D.y, size / l, size / l, 2.0f / l);
		}

		BX2D->DrawImage(
			m_pSpotLightImage,
			pos2D.x - (m_pSpotLightImage->GetDimensions().width / (16 * l)),
			pos2D.y - (m_pSpotLightImage->GetDimensions().height / (16 * l)),
			false,
			1.0f,
			m_pSpotLightImage->GetDimensions().width / (8 * l),
			m_pSpotLightImage->GetDimensions().height / (8 * l));
	}

	if (m_IsSelected)
	{
		// ATTENUATION
		Matrix matWorld =
			Matrix::CreateScale(
			Vector3(m_Desc.attenuationEnd / 100.0f,m_Desc.attenuationEnd / 100.0f,m_Desc.attenuationEnd / 100.0f)) * 
            Matrix::CreateRotationZ(PiOver2) *
			m_Rotation *
			Matrix::CreateTranslation(
			Vector3(m_Desc.position.X, m_Desc.position.Y, m_Desc.position.Z));

		m_pEffect->SetWorld(matWorld);
		m_pEffect->SetWorldViewProjection(matWorld * rc->GetCamera()->GetViewProjection());
		m_pAttenuationSpline->Draw(m_pEffect);

		matWorld =
			Matrix::CreateScale(
			Vector3(m_Desc.attenuationEnd / 100.0f,m_Desc.attenuationEnd / 100.0f,m_Desc.attenuationEnd / 100.0f)) * 
			Matrix::CreateTranslation(
			Vector3(m_Desc.position.X, m_Desc.position.Y, m_Desc.position.Z));

		m_pEffect->SetWorld(matWorld);
		m_pEffect->SetWorldViewProjection(matWorld * rc->GetCamera()->GetViewProjection());
		m_pAttenuationSpline3->Draw(m_pEffect);
	}
	else
	{
		Matrix matWorld =
			Matrix::CreateScale(
			Vector3(0.5f,0.5f,0.5f)) *
            Matrix::CreateRotationZ(PiOver2) * 
			m_Rotation * 
			Matrix::CreateTranslation(
			Vector3(m_Desc.position.X, m_Desc.position.Y, m_Desc.position.Z));

		m_pEffect->SetWorld(matWorld);
		m_pEffect->SetWorldViewProjection(matWorld * rc->GetCamera()->GetViewProjection());
		m_pAttenuationSpline2->Draw(m_pEffect);
	}
}

void SpotLight::Translate(const Vector3& add)
{
	m_Desc.position += add;
    UpdateShadowCameraView();
}
void SpotLight::SetPosition(const Vector3& pos)
{
	m_Desc.position = pos;
    UpdateShadowCameraView();
}
Vector3 SpotLight::GetPosition() const
{
    return m_Desc.position;
}

void SpotLight::Rotate(const Vector3& axis, float angle)
{
    Matrix rot(Matrix::CreateRotation(axis, angle));
    m_Desc.direction = Vector3::Transform(m_Desc.direction, rot).XYZ();
    m_vUp = Vector3::Transform(m_vUp, rot).XYZ();
    UpdateShadowCameraView();

	m_Rotation *= rot;
}

void SpotLight::Scale(const Vector3& scale)
{
	m_Desc.attenuationEnd *= (scale.X + scale.Y + scale.Z) / 3;
    UpdateShadowCameraProjection();
}

void SpotLight::Enable()
{
	m_IsEnabled = true;
}
void SpotLight::Disable()
{
	m_IsEnabled = false;
}
bool SpotLight::IsEnabled() const
{
	return m_IsEnabled;
}

void SpotLight::Selected(bool selected)
{
	m_IsSelected = selected;
}
bool SpotLight::IsSelected() const
{
	return m_IsSelected;
}
bool SpotLight::HasShadowMap() const
{
	return m_ShadowMap != 0;
}
Texture2D* SpotLight::GetShadowMap() const
{
	return m_ShadowMap;
}
Camera* SpotLight::GetShadowCamera() const
{
    return m_pShadowCamera;
}
ShadowMapType SpotLight::GetShadowMapType() const
{
    return m_ShadowMapType;
}
void SpotLight::SetShadowMap(ID3D10Device* pDXDevice, ShadowMapType type)
{
    delete m_ShadowMap;
    m_ShadowMap = 0;
    delete m_pShadowCamera;
    m_pShadowCamera = 0;

    m_ShadowMapType = type;

	if (type != ShadowMapType_None)
	{
		m_ShadowMap = new Texture2D(pDXDevice, type, type, false);
        m_pShadowCamera = new Camera(type, type, false);
        m_pShadowCamera->SetPosition(m_Desc.position);    

        UpdateShadowCameraProjection();
        UpdateShadowCameraView();
	}

	m_ShadowMapType = type;
}

void SpotLight::UpdateShadowCameraView()
{
    if (m_pShadowCamera != 0)
        m_pShadowCamera->LookAt(m_Desc.position, m_Desc.position + m_Desc.direction, m_vUp); 
}
void SpotLight::UpdateShadowCameraProjection()
{
    if (m_pShadowCamera != 0)
        m_pShadowCamera->SetLens(1.0f, m_OpeningsAngle, 1.0f, m_Desc.attenuationEnd);
}

D3D10_RECT SpotLight::CalcScissorRect(Camera* camera, UINT backbufferWidth, UINT backbufferHeight) const
{
    Vector4 pos;
    pos = Vector3::Transform(m_Desc.position, camera->GetViewProjection());

    float len = abs(pos.W);
	len *= 0.001f;
	float size = m_Desc.attenuationEnd / len * 1.1f;//len.Length();

	pos.X /= pos.W;
	pos.Y /= pos.W;

	pos.X += 1.f; pos.X /= 2;
	pos.Y += 1.f; pos.Y /= 2; pos.Y = 1 - pos.Y;

	D3D10_RECT r;
	r.left = static_cast<LONG>(pos.X * backbufferWidth - size);
	r.right =  static_cast<LONG>(r.left + size * 2);
	r.top = static_cast<LONG>(pos.Y * backbufferHeight - size);
	r.bottom = static_cast<LONG>(r.top + size * 2);

    return r;
}

void SpotLight::SetColor(const Color& color)
{
    m_Desc.color = color;
}
const Color& SpotLight::GetColor() const
{
    return m_Desc.color;
}

void SpotLight::SetMulitplier(float mult)
{
    m_Desc.multiplier = mult;
}
float SpotLight::GetMulitplier() const
{
    return m_Desc.multiplier;
}

void SpotLight::SetAttenuationStart(float start)
{
    m_Desc.attenuationStart = start;
    UpdateShadowCameraProjection();
}    
float SpotLight::GetAttenuationStart() const
{
    return m_Desc.attenuationStart;
}

void SpotLight::SetAttenuationEnd(float end)
{
    m_Desc.attenuationEnd = end;
    UpdateShadowCameraProjection();
}
float SpotLight::GetAttenuationEnd() const
{
    return m_Desc.attenuationEnd;
}

void SpotLight::SetOpeningsAngle(float rad)
{
    m_OpeningsAngle = rad;

    m_Desc.power = -(log(100.0f) / (log(cos(m_OpeningsAngle / 2.0f)))); //use only a half of the angle

    UpdateShadowCameraProjection();
}
void SpotLight::AddOpeningsAngle(float rad)
{
	SetOpeningsAngle(m_OpeningsAngle + rad);
}

float SpotLight::GetOpeningsAngle() const
{
	return m_OpeningsAngle;
}

const SpotLightDesc& SpotLight::GetDesc() const
{
    return m_Desc;
}

void SpotLight::SetBehaviour(LightBehaviour* lightBehaviour)
{
    delete m_pLightBehaviour;
    m_pLightBehaviour = lightBehaviour;
    m_pLightBehaviour->Init(this);
}

void SpotLight::Serialize(Serializer* pSerializer)
{
    pSerializer->GetStream()->storeFloat(m_Desc.attenuationEnd);
    pSerializer->GetStream()->storeFloat(m_Desc.attenuationStart);
    pSerializer->GetStream()->storeColor(m_Desc.color);
    pSerializer->GetStream()->storeFloat(m_Desc.multiplier);
    pSerializer->GetStream()->storeVector3(m_Desc.position);
    pSerializer->GetStream()->storeVector3(m_Desc.direction);
    pSerializer->GetStream()->storeVector3(m_vUp);
    pSerializer->GetStream()->storeFloat(m_Desc.power);
    pSerializer->GetStream()->storeWord(static_cast<WORD>(m_ShadowMapType));

    pSerializer->Serialize(m_pLightBehaviour);
}
void SpotLight::Deserialize(Serializer* pSerializer)
{
    m_Desc.attenuationEnd = pSerializer->GetStream()->readFloat();
    m_Desc.attenuationStart = pSerializer->GetStream()->readFloat();
    m_Desc.color = pSerializer->GetStream()->readColor();
    m_Desc.multiplier = pSerializer->GetStream()->readFloat();
    m_Desc.position = pSerializer->GetStream()->readVector3();
    m_Desc.direction = pSerializer->GetStream()->readVector3();
    m_vUp = pSerializer->GetStream()->readVector3();
    m_Rotation = Matrix::CreateLookAt(Vector3::Zero, m_Desc.direction, m_vUp);
    m_Rotation *= Matrix::CreateRotation(m_vUp, -PiOver2);

    m_Desc.power = pSerializer->GetStream()->readFloat();
    //-(log(100.0f) / (log(cos(m_OpeningsAngle / 2.0f))));
    m_OpeningsAngle = 2.0f*acos(pow(E, -log(100.0f) / m_Desc.power));

    SetShadowMap(Content->GetDxDevice(), static_cast<ShadowMapType>(pSerializer->GetStream()->readWord()));

    UpdateShadowCameraProjection();
    UpdateShadowCameraView();
    
    SetBehaviour(dynamic_cast<LightBehaviour*>(pSerializer->Deserialize(LightBehaviour::Create)));
}