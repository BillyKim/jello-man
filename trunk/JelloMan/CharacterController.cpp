#include "CharacterController.h"
#include "ICharacter.h"
#include "Level.h"
#include "SpawnPoint.h"
#include "FluidsCharacterActor.h"
#include "Controls.h"
#include "PhysX.h"
#include "ContentManager.h"

CharacterController::CharacterController(Level* pLevel) : 
        m_pLevel(pLevel), 
        m_pSpawnPoint(0), 
        m_pCharacter(new FluidsCharacterActor(pLevel)),
        m_State(State_Idle),
        m_Timer(0.0f),
        m_SpawnWaitTime(4.0f),
		m_pRespawnFormatCounter(0),
		m_pRespawnFormatHappyFace(0)
{
}


CharacterController::~CharacterController(void)
{
    delete m_pCharacter;
}

void CharacterController::Init(ID3D10Device* pDXDevice, PhysX* pPhysXEngine, 
              Graphics::Camera::FollowCamera* pCamera)
{
    m_pCharacter->Init(pDXDevice, pPhysXEngine, pCamera, Vector3::Zero);
}

void CharacterController::SetSpawnPoint(const SpawnPoint* pSpawnPoint)
{
    m_pSpawnPoint = pSpawnPoint;
}

void CharacterController::Respawn()
{
    if (m_pSpawnPoint == 0)
    {
        PANIC("No spawnpoint set, unable to spawn!");
    }
    else
    {
        m_pCharacter->Respawn(m_pSpawnPoint);
    }
    m_State = State_Idle;
}

void CharacterController::Tick(float dTime)
{
    m_pCharacter->Tick(dTime);

    if (m_State == State_Idle && m_pCharacter->IsDead())
    {
        m_State = State_WaitingToRespawn;
        m_Timer = m_SpawnWaitTime;
    }
    else if (m_State == State_WaitingToRespawn)
    {
        m_Timer -= dTime;
        if (m_Timer <= 0)
        {
            Respawn();
        }
    }
    if (CONTROLS->IsKeyPressed(VK_RETURN))
    {
        Respawn();
    }
}


void CharacterController::DrawForward(const RenderContext* /*pRenderContext*/)
{
	if (m_State == State_WaitingToRespawn)
	{
		if (!m_pRespawnFormatHappyFace)
			m_pRespawnFormatHappyFace = Content->LoadTextFormat(_T("Arial"),200,true,false);
		if (!m_pRespawnFormatCounter)
			m_pRespawnFormatCounter = Content->LoadTextFormat(_T("Arial"), 12, true, false);

		float opacity(0.0f);

		if (m_Timer >= 3.0f)
			opacity = (4.0f - m_Timer) / 2.0f;
		else opacity = 1.0f;

		BX2D->SetColor(0.0,0.0,0.0,opacity);
		BX2D->FillBackGround();

		float opacity2(0.0f);

		if (m_Timer >= 1.0f)
			opacity2 = (4.0f - m_Timer) / 4.0f;
		else opacity2 = 1.0f;

		BX2D->SetColor(255,255,255,opacity2);
		BX2D->SetFont(m_pRespawnFormatHappyFace);
		m_pRespawnFormatHappyFace->SetHorizontalAlignment(TEXT_ALIGNMENT_CENTER);
		m_pRespawnFormatHappyFace->SetVerticalAlignment(PARAGRAPH_ALIGNMENT_MIDDLE);

		D2D1_MATRIX_3X2_F rot;
		D2D1MakeRotateMatrix(90,Point2F(BX2D->GetWindowSize().width/2,
									BX2D->GetWindowSize().height/2),&rot);

		BX2D->SetTransform(rot);

		if (m_Timer >= 3.0f)
			BX2D->DrawStringCentered(_T(":("),-20);
		else if (m_Timer >= 2.0f)
			BX2D->DrawStringCentered(_T(":|"),-20);
		else if (m_Timer >= 1.0f)
			BX2D->DrawStringCentered(_T(":)"),-20);
		else if (m_Timer >= 0.0f)
			BX2D->DrawStringCentered(_T(":D"),-20);

		BX2D->SetFont(m_pRespawnFormatCounter);

		BX2D->ResetTransform();

		BX2D->SetColor(255,255,255);

		tstringstream stream;
		stream << _T("Respawning in: ") << (int)m_Timer;
		BX2D->DrawStringCentered(stream.str(), 0, 300);
	}
}
void CharacterController::DrawFluids(const RenderContext* pRenderContext)
{
    m_pCharacter->Draw(pRenderContext);
}
void CharacterController::DrawDeferred(const RenderContext* /*pRenderContext*/)
{
}