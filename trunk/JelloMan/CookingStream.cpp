#include "stdafx.h"
#include "NxPhysics.h"
#include "CookingStream.h"

UserStream::UserStream(const char* filename, bool load) : fp(NULL)
{
	errno_t err = 0;
	err = fopen_s(&fp, filename, load ? "rb" : "wb");
	if (err != 0)
		cout << "UserStream: ERROR opening file: " + string(filename);
}

UserStream::~UserStream()
{
	if(fp)	fclose(fp);
}

// Loading API
NxU8 UserStream::readByte() const
{
	NxU8 b;
	size_t r = fread(&b, sizeof(NxU8), 1, fp);
	NX_ASSERT(r);
	return b;
}

NxU16 UserStream::readWord() const
{
	NxU16 w;
	size_t r = fread(&w, sizeof(NxU16), 1, fp);
	NX_ASSERT(r);
	return w;
}

NxU32 UserStream::readDword() const
{
	NxU32 d;
	size_t r = fread(&d, sizeof(NxU32), 1, fp);
	NX_ASSERT(r);
	return d;
}

float UserStream::readFloat() const
{
	NxReal f;
	size_t r = fread(&f, sizeof(NxReal), 1, fp);
	NX_ASSERT(r);
	return f;
}

double UserStream::readDouble() const
{
	NxF64 f;
	size_t r = fread(&f, sizeof(NxF64), 1, fp);
	NX_ASSERT(r);
	return f;
}
Vector2 UserStream::readVector2() const
{
    float x = readFloat(), 
          y = readFloat();
    return Vector2(x, y);
}
Vector3	UserStream::readVector3() const
{
    float x = readFloat(), 
          y = readFloat(),
          z = readFloat();
    return Vector3(x, y, z);
}
Vector4	UserStream::readVector4() const
{
    float x = readFloat(), 
          y = readFloat(),
          z = readFloat(),
          w = readFloat();
    return Vector4(x, y, z, w);
}
Color UserStream::readColor() const
{
    float r = readFloat(), 
          g = readFloat(),
          b = readFloat(),
          a = readFloat();
    return Color(r, g, b, a);
}
Matrix UserStream::readMatrix() const
{
    float mat[4][4];
    mat[0][0] = readFloat(); mat[0][1] = readFloat(); mat[0][2] = readFloat(); mat[0][3] = readFloat();
    mat[1][0] = readFloat(); mat[1][1] = readFloat(); mat[1][2] = readFloat(); mat[1][3] = readFloat();
    mat[2][0] = readFloat(); mat[2][1] = readFloat(); mat[2][2] = readFloat(); mat[2][3] = readFloat();
    mat[3][0] = readFloat(); mat[3][1] = readFloat(); mat[3][2] = readFloat(); mat[3][3] = readFloat();
    Matrix m(   mat[0][0], mat[0][1], mat[0][2], mat[0][3],
                mat[1][0], mat[1][1], mat[1][2], mat[1][3],
                mat[2][0], mat[2][1], mat[2][2], mat[2][3],
                mat[3][0], mat[3][1], mat[3][2], mat[3][3]);
    return m;
}
string UserStream::readString()	const
{
    string ret("");
	WORD buffSize;
    char* buffer;
    buffSize = readWord();
    if (buffSize > 0)
    {
        buffer = new char[buffSize];
        readBuffer(buffer, buffSize * sizeof(char));
        ret = string(buffer);
        ret = ret.substr(0, buffSize);
        delete buffer;
    }
    return ret;
}
tstring UserStream::readTString() const
{
    tstring ret(_T(""));
	WORD buffSize;
    tchar* buffer;
    buffSize = readWord();
    if (buffSize > 0)
    {
        buffer = new tchar[buffSize];
        readBuffer(buffer, buffSize * sizeof(tchar));
        ret = tstring(buffer);
        ret = ret.substr(0, buffSize);
        delete buffer;
    }
    return ret;
}
void UserStream::readBuffer(void* buffer, NxU32 size)	const
{
	size_t w = fread(buffer, size, 1, fp);
	NX_ASSERT(w);
}

// Saving API
NxStream& UserStream::storeByte(NxU8 b)
{
	size_t w = fwrite(&b, sizeof(NxU8), 1, fp);
	NX_ASSERT(w);
	return *this;
}

NxStream& UserStream::storeWord(NxU16 w)
{
	size_t ww = fwrite(&w, sizeof(NxU16), 1, fp);
	NX_ASSERT(ww);
	return *this;
}

NxStream& UserStream::storeDword(NxU32 d)
{
	size_t w = fwrite(&d, sizeof(NxU32), 1, fp);
	NX_ASSERT(w);
	return *this;
}

NxStream& UserStream::storeFloat(NxReal f)
{
	size_t w = fwrite(&f, sizeof(NxReal), 1, fp);
	NX_ASSERT(w);
	return *this;
}

NxStream& UserStream::storeDouble(NxF64 f)
{
	size_t w = fwrite(&f, sizeof(NxF64), 1, fp);
	NX_ASSERT(w);
	return *this;
}

NxStream& UserStream::storeVector2(const Vector2& f)
{  
    storeFloat(f.X);
    storeFloat(f.Y);
	return *this;
}

NxStream& UserStream::storeVector3(const Vector3& f)
{
    storeFloat(f.X);
    storeFloat(f.Y);
    storeFloat(f.Z);
	return *this;
}
NxStream& UserStream::storeVector4(const Vector4& f)
{
    storeFloat(f.X);
    storeFloat(f.Y);
    storeFloat(f.Z);
    storeFloat(f.W);
	return *this;
}
NxStream& UserStream::storeMatrix(const Matrix& mat)
{
    storeFloat(mat(0, 0));  storeFloat(mat(0, 1));  storeFloat(mat(0, 2));  storeFloat(mat(0, 3));
    storeFloat(mat(1, 0));  storeFloat(mat(1, 1));  storeFloat(mat(1, 2));  storeFloat(mat(1, 3));
    storeFloat(mat(2, 0));  storeFloat(mat(2, 1));  storeFloat(mat(2, 2));  storeFloat(mat(2, 3));
    storeFloat(mat(3, 0));  storeFloat(mat(3, 1));  storeFloat(mat(3, 2));  storeFloat(mat(3, 3));
    return *this;
}
NxStream& UserStream::storeColor(const Color& c)
{
    storeFloat(c.R);
    storeFloat(c.G);
    storeFloat(c.B);
    storeFloat(c.A);
	return *this;
}
NxStream& UserStream::storeString(const string& str)
{
    storeWord(static_cast<WORD>(str.size()));
    if (str.size() > 0)
        storeBuffer(str.c_str(), str.size() * sizeof(char));
    return *this;
}
NxStream& UserStream::storeTString(const tstring& str)
{
    storeWord(static_cast<WORD>(str.size()));
    if (str.size() > 0)
        storeBuffer(str.c_str(), str.size() * sizeof(tchar));
    return *this;
}
NxStream& UserStream::storeBuffer(const void* buffer, NxU32 size)
{
	size_t w = fwrite(buffer, size, 1, fp);
	NX_ASSERT(w);
	return *this;
}




MemoryWriteBuffer::MemoryWriteBuffer() : currentSize(0), maxSize(0), data(NULL)
{
}

MemoryWriteBuffer::~MemoryWriteBuffer()
{
	NX_DELETE_ARRAY(data);
}

void MemoryWriteBuffer::clear()
{
	currentSize = 0;
}

NxStream& MemoryWriteBuffer::storeByte(NxU8 b)
{
	storeBuffer(&b, sizeof(NxU8));
	return *this;
}
NxStream& MemoryWriteBuffer::storeWord(NxU16 w)
{
	storeBuffer(&w, sizeof(NxU16));
	return *this;
}
NxStream& MemoryWriteBuffer::storeDword(NxU32 d)
{
	storeBuffer(&d, sizeof(NxU32));
	return *this;
}
NxStream& MemoryWriteBuffer::storeFloat(NxReal f)
{
	storeBuffer(&f, sizeof(NxReal));
	return *this;
}
NxStream& MemoryWriteBuffer::storeDouble(NxF64 f)
{
	storeBuffer(&f, sizeof(NxF64));
	return *this;
}
NxStream& MemoryWriteBuffer::storeBuffer(const void* buffer, NxU32 size)
{
	NxU32 expectedSize = currentSize + size;
	if(expectedSize > maxSize)
	{
		maxSize = expectedSize + 4096;

		NxU8* newData = new NxU8[maxSize];
		NX_ASSERT(newData!=NULL);

		if(data)
		{
			memcpy(newData, data, currentSize);
			delete[] data;
		}
		data = newData;
	}
	memcpy(data+currentSize, buffer, size);
	currentSize += size;
	return *this;
}


MemoryReadBuffer::MemoryReadBuffer(const NxU8* data) : buffer(data)
{
}

MemoryReadBuffer::~MemoryReadBuffer()
{
	// We don't own the data => no delete
}

NxU8 MemoryReadBuffer::readByte() const
{
	NxU8 b;
	memcpy(&b, buffer, sizeof(NxU8));
	buffer += sizeof(NxU8);
	return b;
}

NxU16 MemoryReadBuffer::readWord() const
{
	NxU16 w;
	memcpy(&w, buffer, sizeof(NxU16));
	buffer += sizeof(NxU16);
	return w;
}

NxU32 MemoryReadBuffer::readDword() const
{
	NxU32 d;
	memcpy(&d, buffer, sizeof(NxU32));
	buffer += sizeof(NxU32);
	return d;
}

float MemoryReadBuffer::readFloat() const
{
	float f;
	memcpy(&f, buffer, sizeof(float));
	buffer += sizeof(float);
	return f;
}

double MemoryReadBuffer::readDouble() const
{
	double f;
	memcpy(&f, buffer, sizeof(double));
	buffer += sizeof(double);
	return f;
}

void MemoryReadBuffer::readBuffer(void* dest, NxU32 size) const
{
	memcpy(dest, buffer, size);
	buffer += size;
}
