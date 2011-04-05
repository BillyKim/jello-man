#pragma once

#include "D3DUtil.h"
#include <vector>
#include "Controls.h"
#include "GameTimer.h"
#include "Blox2D.h"

//-----------------------------------------------------------------
// Bitmap Class
//-----------------------------------------------------------------

struct BITMAP_INFO
{
	tstring filepath;
	ID2D1Bitmap* bitmap;
};

class Bitmap
{
public:
	// constructors
	Bitmap(tstring filepath);
	Bitmap(int IDBitmap);

	// destructor
	virtual  ~Bitmap();

	// getters
	D2D1_SIZE_F GetSize();
	BITMAP_INFO GetInfo();
	bool Exists();

private:
	tstring m_FilePath;
	bool m_bExists;

	ID2D1Bitmap* m_pBitmap;
};