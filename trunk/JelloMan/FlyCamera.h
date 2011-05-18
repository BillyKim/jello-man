#pragma once
#include "CameraBase.h"

namespace Graphics{
namespace Camera{

class FlyCamera : public CameraBase
{
public:
    FlyCamera(int windowWidth, int windowHeight);
    virtual ~FlyCamera(void);

	virtual void Tick(const float dTime);

private:
	bool m_bControllable;

	// DISABLE DEFAULT COPY & ASSIGNMENT
	FlyCamera(const FlyCamera& t);
	FlyCamera& operator=(const FlyCamera& t);
};

}} //end namespace


