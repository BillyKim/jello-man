#pragma once
#include "PhysX.h"
#include "Actor.h"

enum JointType
{
    JointType_Fixed = NX_JOINT_FIXED,
    JointType_Revolute = NX_JOINT_REVOLUTE 
};

class IJoint
{    
    virtual JointType GetType() const = 0;
};