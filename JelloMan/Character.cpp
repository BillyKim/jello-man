#include "Character.h"
#include "ContentManager.h"

// CONSTRUCTOR - DESTRUCTOR
Character::Character(Camera* pTrackingCamera)	:	LevelObject(),
													m_Speed(2),
													m_RotationAngle(0),
													m_pTrackingCamera(pTrackingCamera)
{
	m_Pos = Vector3(0.0f,0.0f,0.0f);	
}

Character::~Character()
{
	/*delete m_pModel;
	delete m_pEffect;
	delete m_pDiffuseMap;
	delete m_pSpecMap;
	delete m_pGlossMap;*/
}

// GENERAL
void Character::Tick(const float dTime)
{
	Actor::Tick(dTime);

	// MOVE CHARACTER
	Vector3 dir(0.0f, 0.0f, 0.0f);
	Vector3 Z(0.0f,0.0f,1.0f);

	D3DXVECTOR3 Zdx = Z.ToD3DVector3();

	D3DXMATRIX R;
	D3DXMatrixRotationY(&R, m_RotationAngle);
	D3DXVec3TransformCoord(&Zdx, &Zdx, &R);

	Z = Vector3(Zdx);

	if (CONTROLS->GetKeyboardLayout() == GameConfig::KEYBOARD_LAYOUT_AZERTY)
	{
		if (CONTROLS->IsKeyDown('Z'))
			dir += Z;
		if (CONTROLS->IsKeyDown('Q'))
			m_RotationAngle -= (0.05f * dTime * 60);
	}
	else
	{
		if (CONTROLS->IsKeyDown('W'))
			dir += Z;
		if (CONTROLS->IsKeyDown('A'))
			m_RotationAngle -= (0.05f * dTime * 60);
	}

	if (CONTROLS->IsKeyDown('S'))
		dir -= Z;
	if (CONTROLS->IsKeyDown('D'))
		m_RotationAngle += (0.05f * dTime * 60);

	dir.Normalize();

	int speed = m_Speed;

	if (CONTROLS->IsKeyDown(VK_SHIFT))
		speed *= 5;

	Vector3 oldPos = m_Pos;

	Move(dir * (float)speed * dTime * 60);

	Translate(m_Pos - oldPos);

	m_WorldMatrix = m_WorldMatrix.CreateRotationY(m_RotationAngle) * m_WorldMatrix.CreateTranslation(m_Pos);

	// MOVE CAMERA
	Zdx.y -= 0.5;
	Vector3 posCam = m_Pos - (Vector3(Zdx) * 200);
	Vector3 posChar = Vector3(m_pActor->getGlobalPosition().x,m_pActor->getGlobalPosition().y,m_pActor->getGlobalPosition().z);
	posChar.Y += 50;

	m_pTrackingCamera->LookAt(posCam,posChar,Vector3(0, 1, 0));
}

void Character::Move(Vector3 move)
{
	m_Pos += move;
}