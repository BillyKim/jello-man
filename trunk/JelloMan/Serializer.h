#pragma once
#include <string>
#include "CookingStream.h"

class ISerializable;

class Serializer
{
public:
	Serializer(const std::string& file, PhysX* pPhysX);
	virtual ~Serializer();

	void Begin(bool load);
	void End();

	void Serialize(ISerializable* obj);
    ISerializable* Deserialize(ISerializable* (*GetObject)(DWORD id));

    bool eof();

    UserStream* GetStream() { return m_Stream; }

    //Usefull when deserializing
    PhysX* GetPhysX() { return m_pPhysX; }

private:
    void WriteHeader(ISerializable* obj);

	UserStream* m_Stream;
	string m_Filename;

    DWORD m_NumSerialized;
    bool m_bLoad;

    PhysX* m_pPhysX;
};

