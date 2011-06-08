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

    bool eof() const;

    UserStream* GetStream() const { return m_Stream; }

    //Usefull when deserializing
    PhysX* GetPhysX() const { return m_pPhysX; }

private:
    void WriteHeader(ISerializable* obj);

	UserStream* m_Stream;
	string m_Filename;

    DWORD m_NumSerialized;
    bool m_bLoad;

    PhysX* m_pPhysX;

	
	Serializer(const PhysX& t);
	Serializer& operator=(const PhysX& t);
};

