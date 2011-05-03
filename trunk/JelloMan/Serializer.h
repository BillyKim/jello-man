#pragma once
#include <string>
#include "CookingStream.h"

class ISerializable;

class Serializer
{
public:
	Serializer(const std::string& file);
	virtual ~Serializer();

	void Init(const std::string& file);
	void Begin(bool load);
	void End();

	void Serialize(ISerializable* obj);
    ISerializable* Deserialize(ISerializable* (*GetObject)(DWORD id));

    bool eof();

    UserStream* GetStream() { return m_Stream; }

private:
    void WriteHeader(ISerializable* obj);

	UserStream* m_Stream;
	string m_Filename;

    DWORD m_NumSerialized;
    bool m_bLoad;
};

