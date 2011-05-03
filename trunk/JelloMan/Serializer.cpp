#include "Serializer.h"
#include "FileNotFoundException.h"

Serializer::Serializer(): m_Stream(0), m_Filename("")
{
}


Serializer::~Serializer(void)
{
	delete m_Stream;
}

void Serializer::Init(const std::string& file)
{
	m_Filename = file;
}
void Serializer::Begin(bool open)
{
	//type_info i = typeid(Serializer).hash_code();
	if (m_Filename == "")
	{
		if (open)
		{	
			FILE* file;
			errno_t err = 0;
			err = fopen_s(&file, m_Filename.c_str(), "r");
			fclose(file);
			if (err != 0)
				throw exceptions::FileNotFoundException();
		}
		m_Stream = new UserStream(m_Filename.c_str(), open);
	}
}
void Serializer::End()
{
	delete m_Stream;
	m_Stream = 0;
}

void Serializer::Serialize(ISerializable* obj)
{
	ASSERT(m_Stream != 0);
	obj->Serialize(m_Stream);
}