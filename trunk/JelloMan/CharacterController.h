#pragma once

class SpawnPoint;
class Level;
class FluidsCharacterActor;
class RenderContext;
struct ID3D10Device;
class PhysXEngine;
#include "FollowCamera.h"
#include "Blox2D.h"

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

    void Init(ID3D10Device* pDXDevice, PhysX* pPhysXEngine, 
              Graphics::Camera::FollowCamera* pCamera);

    void SetSpawnPoint(const SpawnPoint* pSpawnPoint);

    void Tick(float dTime);

    void DrawForward(const RenderContext* pRenderContext);
    void DrawFluids(const RenderContext* pRenderContext);
    void DrawDeferred(const RenderContext* pRenderContext);

    void Respawn();

private:
    const SpawnPoint* m_pSpawnPoint;
    Level* m_pLevel;
    FluidsCharacterActor* m_pCharacter;

    State m_State;
    float m_Timer;
    float m_SpawnWaitTime;

	TextFormat* m_pRespawnFormatCounter;
	TextFormat* m_pRespawnFormatHappyFace;

	/* Disabling default copy constructor and assignment operator */
    CharacterController(const CharacterController&);
    CharacterController& operator=(const CharacterController&);
};

