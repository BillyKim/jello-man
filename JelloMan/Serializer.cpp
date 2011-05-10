#include "Serializer.h"
#include "FileNotFoundException.h"
#include "ISerializable.h"

Serializer::Serializer(const std::string& file, PhysX* pPhysX): 
        m_Stream(0), m_Filename(file), m_bLoad(false), m_NumSerialized(0),
            m_pPhysX(pPhysX)
{
}


Serializer::~Serializer(void)
{
	End();
}

void Serializer::Begin(bool load)
{
	if (m_Filename != "")
	{
		if (load)
		{	
			FILE* file;
			errno_t err = 0;
			err = fopen_s(&file, m_Filename.c_str(), "r");
			fclose(file);
			if (err != 0)
				throw exceptions::FileNotFoundException();
		}
		m_Stream = new UserStream(m_Filename.c_str(), load);

        if (load == false)
        {
            m_NumSerialized = 0;
            m_Stream->storeDword(0);
        }
        else
        {
            //read header
            m_NumSerialized = m_Stream->readDword();
        }

        m_bLoad = load;
	}
}
void Serializer::End()
{
    if (m_bLoad == false && m_Stream != 0)
    {
        rewind(m_Stream->fp);      
        m_Stream->storeDword(m_NumSerialized);
        m_NumSerialized = 0;
    }

	delete m_Stream;
	m_Stream = 0;
}

void Serializer::Serialize(ISerializable* obj)
{
	ASSERT(m_Stream != 0, "");
    if (obj != 0)
    {
        WriteHeader(obj);
        obj->Serialize(this);
        ++m_NumSerialized;
    }
    else
        cout << "trying to serialize 0ptr - skipped";
}

void Serializer::WriteHeader(ISerializable* obj)
{
    m_Stream->storeDword(obj->GetUniqueIdentifier());
}

ISerializable* Serializer::Deserialize(ISerializable* (*GetObject)(DWORD id))
{
    //read object header
    DWORD header = m_Stream->readDword();

    //get object
    ISerializable* obj = GetObject(header);

    //deserialize
    obj->Deserialize(this);

    --m_NumSerialized;

    return obj;
}

bool Serializer::eof()
{
    return m_NumSerialized <= 0;
}