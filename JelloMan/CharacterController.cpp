#include "CharacterController.h"
#include "ICharacter.h"
#include "Level.h"
#include "SpawnPoint.h"

CharacterController::CharacterController(Level* pLevel) : 
        m_pLevel(pLevel), 
        m_pSpawnPoint(0), 
        m_pCharacter(0),
        m_State(State_Idle),
        m_Timer(0.0f),
        m_SpawnWaitTime(1.0f)
{
}


CharacterController::~CharacterController(void)
{
}

void CharacterController::SetSpawnPoint(const SpawnPoint* pSpawnPoint)
{
    m_pSpawnPoint = pSpawnPoint;
}

void CharacterController::Respawn()
{
    m_State = State_Idle;
    m_pCharacter->Respawn(m_pSpawnPoint);
}

void CharacterController::Tick(float dTime)
{
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
}