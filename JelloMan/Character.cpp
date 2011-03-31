#include "Character.h"
#include "ContentManager.h"

// CONSTRUCTOR - DESTRUCTOR
Character::Character()	:	m_pModel(0),
							m_pEffect(0),
							m_mtxWorld(Matrix::Identity),
							m_pDiffuseMap(0),
							m_pSpecMap(0),
							m_pGlossMap(0),
							m_Speed(2),
							m_RotationAngle(0)
{
	m_Pos = Vector3(0.0f,0.0f,0.0f);	
}

Character::~Character()
{
	delete m_pModel;
	delete m_pEffect;
	delete m_pDiffuseMap;
	delete m_pSpecMap;
	delete m_pGlossMap;
}

// GENERAL
void Character::Init()
{
	m_pDiffuseMap = Content->LoadTexture2D(_T("Content/Textures/weapon_diffuse.png"));
    m_pSpecMap = Content->LoadTexture2D(_T("Content/Textures/weapon_spec.png"));
    m_pGlossMap = Content->LoadTexture2D(_T("Content/Textures/weapon_gloss.png"));

    m_pEffect = Content->LoadEffect<DeferredPreEffect>(_T("predeferred.fx"));

    m_pModel = Content->LoadModel(_T("Content/Models/jman.binobj"));
    
    for (vector<ModelMesh<VertexPosNormTex>*>::const_iterator it = m_pModel->GetModelMeshes().cbegin(); it != m_pModel->GetModelMeshes().cend(); ++it)
    {
        (*it)->SetEffect(m_pEffect);
    }
}

void Character::Tick(float dTime)
{
	Vector3 dir(0.0f, 0.0f, 0.0f);
	Vector3 Z(0.0f,0.0f,1.0f);

	D3DXVECTOR3 Zdx = Z.ToD3DVector3();

	D3DXMATRIX R;
	D3DXMatrixRotationY(&R, m_RotationAngle);
	D3DXVec3TransformCoord(&Zdx, &Zdx, &R);

	Z = Vector3(Zdx);

	if (!CONTROLS->RightMBDown())
	{
		if (CONTROLS->GetKeyboardLayout() == GameConfig::KEYBOARD_LAYOUT_AZERTY)
		{
			if (CONTROLS->IsKeyDown('Z'))
				dir += Z;
			if (CONTROLS->IsKeyDown('Q'))
				m_RotationAngle -= 0.05f;
		}
		else
		{
			if (CONTROLS->IsKeyDown('W'))
				dir += Z;
			if (CONTROLS->IsKeyDown('A'))
				m_RotationAngle -= 0.05f;
		}

		if (CONTROLS->IsKeyDown('S'))
			dir -= Z;
		if (CONTROLS->IsKeyDown('D'))
			m_RotationAngle += 0.05f;

		dir.Normalize();

		Move(dir * (float)m_Speed);
	}

	m_mtxWorld = m_mtxWorld.CreateRotationY(m_RotationAngle) * m_mtxWorld.CreateTranslation(m_Pos);
}

void Character::Draw(const RenderContext* pRenderContext)
{
	m_pEffect->SetDiffuseMap(m_pDiffuseMap);
    m_pEffect->SetSpecMap(m_pSpecMap);
    m_pEffect->SetGlossMap(m_pGlossMap);

    m_pEffect->SetWorld(m_mtxWorld);
	m_pEffect->SetWorldViewProjection(m_mtxWorld * pRenderContext->GetCamera()->GetViewProjection());

    m_pModel->Draw();
}

void Character::Move(Vector3 move)
{
	m_Pos += move;
}