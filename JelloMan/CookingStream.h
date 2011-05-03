#ifndef STREAM_H
#define STREAM_H
#include "Nxf.h"
#include "NxSimpleTypes.h"
#include "NxStream.h"
#include "Vector4.h"
#include "Color.h"

class UserStream : public NxStream
{
public:
	UserStream(const char* filename, bool load);
	virtual						~UserStream();

	virtual		NxU8			readByte()								const;
	virtual		NxU16			readWord()								const;
	virtual		NxU32			readDword()								const;
	virtual		float			readFloat()								const;
	virtual		double			readDouble()							const;
    virtual		Vector2		    readVector2()                           const;
    virtual		Vector3		    readVector3()                           const;
    virtual		Vector4		    readVector4()                           const;
    virtual		Color		    readColor()                             const;
	virtual		void			readBuffer(void* buffer, NxU32 size)	const;

	virtual		NxStream&		storeByte(NxU8 b);
	virtual		NxStream&		storeWord(NxU16 w);
	virtual		NxStream&		storeDword(NxU32 d);
	virtual		NxStream&		storeFloat(NxReal f);
	virtual		NxStream&		storeDouble(NxF64 f);
    virtual		NxStream&		storeVector2(const Vector2& f);
    virtual		NxStream&		storeVector3(const Vector3& f);
    virtual		NxStream&		storeVector4(const Vector4& f);
    virtual		NxStream&		storeColor(const Color& f);
	virtual		NxStream&		storeBuffer(const void* buffer, NxU32 size);

	FILE*			fp;
};

class MemoryWriteBuffer : public NxStream
{
public:
	MemoryWriteBuffer();
	virtual						~MemoryWriteBuffer();
	void			clear();

	virtual		NxU8			readByte()								const	{ NX_ASSERT(0);	return 0;	}
	virtual		NxU16			readWord()								const	{ NX_ASSERT(0);	return 0;	}
	virtual		NxU32			readDword()								const	{ NX_ASSERT(0);	return 0;	}
	virtual		float			readFloat()								const	{ NX_ASSERT(0);	return 0.0f;}
	virtual		double			readDouble()							const	{ NX_ASSERT(0);	return 0.0;	}
	virtual		void			readBuffer(void*, NxU32)	const	{ NX_ASSERT(0);				}

	virtual		NxStream&		storeByte(NxU8 b);
	virtual		NxStream&		storeWord(NxU16 w);
	virtual		NxStream&		storeDword(NxU32 d);
	virtual		NxStream&		storeFloat(NxReal f);
	virtual		NxStream&		storeDouble(NxF64 f);
	virtual		NxStream&		storeBuffer(const void* buffer, NxU32 size);

	NxU32			currentSize;
	NxU32			maxSize;
	NxU8*			data;
};

class MemoryReadBuffer : public NxStream
{
public:
	MemoryReadBuffer(const NxU8* data);
	virtual						~MemoryReadBuffer();

	virtual		NxU8			readByte()								const;
	virtual		NxU16			readWord()								const;
	virtual		NxU32			readDword()								const;
	virtual		float			readFloat()								const;
	virtual		double			readDouble()							const;
	virtual		void			readBuffer(void* buffer, NxU32 size)	const;

	virtual		NxStream&		storeByte(NxU8)							{ NX_ASSERT(0);	return *this;	}
	virtual		NxStream&		storeWord(NxU16)						{ NX_ASSERT(0);	return *this;	}
	virtual		NxStream&		storeDword(NxU32)					    { NX_ASSERT(0);	return *this;	}
	virtual		NxStream&		storeFloat(NxReal)						{ NX_ASSERT(0);	return *this;	}
	virtual		NxStream&		storeDouble(NxF64)						{ NX_ASSERT(0);	return *this;	}
	virtual		NxStream&		storeBuffer(const void*, NxU32)	        { NX_ASSERT(0);	return *this;	}

	mutable		const NxU8*		buffer;
};

#endif
