#include "CharacterController.h"
#include "ICharacter.h"
#include "Level.h"
#include "SpawnPoint.h"
#include "FluidsCharacterActor.h"
#include "Controls.h"
#include "PhysX.h"

CharacterController::CharacterController(Level* pLevel) : 
        m_pLevel(pLevel), 
        m_pSpawnPoint(0), 
        m_pCharacter(new FluidsCharacterActor(pLevel)),
        m_State(State_Idle),
        m_Timer(0.0f),
        m_SpawnWaitTime(1.0f)
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
}
void CharacterController::DrawFluids(const RenderContext* pRenderContext)
{
    m_pCharacter->Draw(pRenderContext);
}
void CharacterController::DrawDeferred(const RenderContext* /*pRenderContext*/)
{
}