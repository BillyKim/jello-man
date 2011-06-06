#pragma once
class SpawnPoint;

class ICharacter
{
public:
    virtual ~ICharacter(void) {}

    virtual bool IsDead() const = 0; //if dead character will be respawned after a time
    virtual void Respawn(const SpawnPoint* pSpawnPoint) = 0;
};

