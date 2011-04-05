#include "Bitmap.h"

//-----------------------------------------------------------------
// Bitmap Class
//-----------------------------------------------------------------
// constructors
Bitmap::Bitmap(tstring filePath)
{
	HRESULT hr;
	hr = BLOX_2D->LoadBitmapFromFile(
		filePath.c_str(),
		0,
		0,
		&m_pBitmap
		);

	if (SUCCEEDED(hr))
	{
		m_FilePath = filePath;
		m_bExists = true;
	}
	else 
	{
		tstringstream stream;
		stream << _T("Can't create bitmap: ") << filePath;

		//BLOX_2D->MsgBox(stream.str().c_str(),_T("ERROR"));

		m_FilePath = _T("");
		m_bExists = false;
	}
}

Bitmap::Bitmap(int IDBitmap)
{
	/*HRESULT hr;
	hr = BLOX_2D->LoadResourceBitmap(
		MAKEINTRESOURCE(IDBitmap),
		L"Image",
		0,
		0,
		&m_pBitmap
		);

	if (SUCCEEDED(hr))
	{
		m_FilePath = _T("EMBEDDED");
		m_bExists = true;
	}
	else 
	{
		m_FilePath = _T("");
		m_bExists = false;
	}*/
}

// destructor
Bitmap::~Bitmap()
{
	SafeRelease(m_pBitmap);
}

// getters

D2D1_SIZE_F Bitmap::GetSize()
{
	if (m_bExists)
		return m_pBitmap->GetSize();
	else
	{
		D2D1_SIZE_F temp;
		temp.width = 0;
		temp.height = 0;

		return temp;
	}
}

BITMAP_INFO Bitmap::GetInfo()
{
	BITMAP_INFO info;

	info.filepath = m_FilePath;
	info.bitmap = m_pBitmap;

	return info;
}

bool Bitmap::Exists()
{
	return m_bExists;
}