#include "UserData.h"


UserData::UserData(void): flags(UserDataFlag_None), object(0)
{
}
UserData::UserData(UserDataFlag flags, void* object): flags(flags), object(object)
{
}


UserData::~UserData(void)
{
}
