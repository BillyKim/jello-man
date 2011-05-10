#pragma once

class IUpdatable
{
public:
    virtual ~IUpdatable() {}

    virtual void Tick(float dTime) = 0;
};