#pragma once

class IUpdatable
{
public:
    virtual void Tick(float dTime) = 0;
};