#pragma once

class SpawnPoint;
class Level;
class ICharacter;

class CharacterController
{
    enum State
    {
        State_Idle,
        State_WaitingToRespawn
    };
public:
    CharacterController(Level* pLevel);
    virtual ~CharacterController(void);

    void SetSpawnPoint(const SpawnPoint* pSpawnPoint);

    void Tick(float dTime);

    void Respawn();

private:
    const SpawnPoint* m_pSpawnPoint;
    Level* m_pLevel;
    ICharacter* m_pCharacter;

    State m_State;
    float m_Timer;
    float m_SpawnWaitTime;
};

