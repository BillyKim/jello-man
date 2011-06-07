#pragma once

enum UserDataFlag
{
    UserDataFlag_None = 0,
    UserDataFlag_IsTriggerable = 1 << 0,
    UserDataFlag_IsPickable = 1 << 1
};

struct UserData
{
public:
    UserData(void);
    UserData(UserDataFlag flags, void* object);
    ~UserData(void);

    //default copy constructor and assingment operator are fine

    UserDataFlag flags;
    void* object;
};

