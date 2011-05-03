#pragma once
#include <string>
#include "ISerializable.h"
#include "CookingStream.h"

class Serializer
{
public:
	Serializer();
	virtual ~Serializer();

	void Init(const std::string& file);
	void Begin(bool open);
	void End();

	void Serialize(ISerializable* obj);

private:
	UserStream* m_Stream;
	string m_Filename;
};

