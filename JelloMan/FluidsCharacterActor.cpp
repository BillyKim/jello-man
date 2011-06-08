#include "FluidsCharacterActor.h"
#include "Level.h"
#include "SpawnPoint.h"
#include "PhysX.h"
#include "Vector3.h"
#include "PhysXSphere.h"
#include "Fluid.h"
#include "UserData.h"

/* CONSTRUCTOR - DESTRUCTOR */
FluidsCharacterActor::FluidsCharacterActor(Level* pLevel)	:	m_pPhysXEngine(0),
										            m_pFluid(0),
										            m_pEmitter(0),
										            m_pCamera(0),
                                                    m_IsTouchingGround(true),
                                                    m_pLevel(pLevel),
                                                    m_CanSwitchGravity(true),
                                                    m_MoveDir(-Vector3::Right),
                                                    m_MoveSpeed(20.0f),
                                                    m_RightDir(0, 0 ,0),
                                                    m_IsDead(false),
													m_Radius(0.5f),
                                                    m_pUserData(0),
                                                    m_GravityRotation(0.0f),
                                                    m_MoveRotation(0.0f)
{
}

FluidsCharacterActor::~FluidsCharacterActor()
{
	SafeDelete(m_pFluid);

    delete m_pUserData;

	m_pEmitter = 0;
	m_pPhysXEngine = 0;
	m_pCamera = 0;
}

/* GENERAL */
void FluidsCharacterActor::Init(ID3D10Device* pDXDevice, PhysX* pPhysXEngine, Graphics::Camera::FollowCamera* pCamera, Vector3 startPos)
{
	ASSERT(pDXDevice != 0, "DXDevice error when creating FluidsCharacterActor!");
	ASSERT(pPhysXEngine->GetScene() != 0, "PhysXScene error when creating FluidsCharacterActor!");
	ASSERT(pCamera != 0, "FollowCamera error when creating FluidsCharacterActor!");

	m_pCamera = pCamera;
    m_pCamera->SetSmoothFlags(Graphics::Camera::FollowCamera::SmoothFlag_Direction | Graphics::Camera::FollowCamera::SmoothFlag_Up);
    m_pCamera->SetFollowDistance(8);

	PhysXSphere sphere(0.5f, 10);
    m_pUserData = new UserData(UserDataFlag_IsTriggerable, dynamic_cast<ITriggerable*>(this));
    sphere.GetShape()->userData = m_pUserData;
	InitActor(pPhysXEngine, sphere, true);

	m_pPhysXEngine = pPhysXEngine;

	SetPosition(startPos);
    RotateMoveDirection(0.0f); // calc right
    
	#pragma region Fluids
	// FLUID
	//int MAX_PARTICLES = (int)maxParticles;
    int MAX_PARTICLES = 1500;

	// setup fluid descriptor
	NxFluidDesc fluidDesc;
    fluidDesc.maxParticles                  = MAX_PARTICLES;
    fluidDesc.kernelRadiusMultiplier		= 2.3f;
    fluidDesc.restParticlesPerMeter			= 5.0f;
	fluidDesc.motionLimitMultiplier			= 1.0f;
	fluidDesc.packetSizeMultiplier			= 8;
    fluidDesc.collisionDistanceMultiplier   = 1.0f;
    fluidDesc.stiffness						= 25.0f;
    fluidDesc.viscosity						= 50.0f;
	fluidDesc.restDensity					= 1000.0f;
	fluidDesc.surfaceTension				= 10.0f;
    fluidDesc.damping						= 0.2f;
    fluidDesc.restitutionForStaticShapes	= 0.1f;
	fluidDesc.dynamicFrictionForStaticShapes= 0.1f;
	fluidDesc.simulationMethod				= NX_F_SPH;
	fluidDesc.flags &= ~NX_FF_HARDWARE;
	fluidDesc.dynamicFrictionForDynamicShapes= 0.8f;
	fluidDesc.collisionGroup = 4;

	// create an attached emitter
	NxFluidEmitterDesc emitterDesc;

	emitterDesc.maxParticles = 0; // using fluid max particles

	emitterDesc.dimensionX = 1.f;
	emitterDesc.dimensionY = 1.f;

	emitterDesc.type = NX_FE_CONSTANT_FLOW_RATE;

	emitterDesc.rate = 300.0f;

	emitterDesc.fluidVelocityMagnitude = .1f;

	emitterDesc.shape = NX_FE_RECTANGULAR;
	emitterDesc.particleLifetime = 2.0f; // in seconds

	////attach to actor
	//emitterDesc.flags |= NX_FEF_ADD_BODY_VELOCITY;
	//emitterDesc.repulsionCoefficient = 300.0f;

	emitterDesc.relPose.M.id();
	emitterDesc.relPose.M.rotX(NxHalfPiF32);
	emitterDesc.relPose.t = NxVec3(0.0f,50.0f,0);
	#pragma endregion

	m_pFluid = new Fluid(pPhysXEngine->GetScene(), fluidDesc, Color(1.0f, 0.0f, 1.0f, 1.0f), 1.0f, pDXDevice);
	ASSERT(m_pFluid != 0, "fluid creation failed");

	m_pEmitter = m_pFluid->GetNxFluid()->createEmitter(emitterDesc);
    #pragma region junk
	//// FORCEFIELD
	//NxForceFieldLinearKernelDesc linearKernelDesc;
 //   linearKernelDesc.constant = NxVec3(-5000,0,0);
 //   
	////The forces do not depend on where the objects are positioned
	//NxMat33 m;
	//m.zero();
	//linearKernelDesc.positionMultiplier = m;
	//linearKernelDesc.noise = NxVec3(1.3,1.3,1.3); //adds a random noise on the forces to make the objects a little more chaotic

	////Set target velocity along the radius to 20
	//linearKernelDesc.velocityTarget = NxVec3(10,0,0);
	//m.diagonal(NxVec3(1,0,0)); //Acts with a force relative to the current velocity to reach the
	//						   //target velocities. 0 means that those components won't be affected
	//linearKernelDesc.velocityMultiplier = m;

 //   NxForceFieldLinearKernel* pLinearKernel;
 //   pLinearKernel = pPhysXEngine->GetScene()->createForceFieldLinearKernel(linearKernelDesc);

 //   NxForceFieldDesc fieldDesc;
	//fieldDesc.coordinates = NX_FFC_SPHERICAL;
 //   fieldDesc.kernel = pLinearKernel;
	//fieldDesc.group = 4;

	//Matrix mat = Matrix::CreateTranslation(Vector3(0,0,0));

	//fieldDesc.pose = mat;

	//fieldDesc.actor = Actor::m_pActor;

 //   m_pForceField = pPhysXEngine->GetScene()->createForceField(fieldDesc);

	//NxForceFieldShape* s = NULL;
	//NxBoxForceFieldShapeDesc b;
	//b.dimensions = NxVec3(20, 20, 20);
	//b.pose.t = NxVec3(0, 0, 0);
	//s = m_pForceField->getIncludeShapeGroup().createShape(b);
    #pragma endregion
}


void FluidsCharacterActor::Respawn(const SpawnPoint* pSpawnPoint)
{
    m_pActor->setLinearVelocity(Vector3::Zero);
    SetPosition(pSpawnPoint->GetPosition());
    m_MoveDir = -Vector3::Right;
    
    m_GravityRotation = 0.0f;
    m_MoveRotation = 0.0f;

    NxVec3 grav;
    m_pPhysXEngine->GetScene()->getGravity(grav);
    m_pPhysXEngine->GetPhysXLock().lock();
    m_pPhysXEngine->GetScene()->setGravity(-Vector3::Up * grav.magnitude());
	m_pPhysXEngine->GetPhysXLock().unlock();

    RotateMoveDirection(0.0f);
	m_IsDead = false;
}

void FluidsCharacterActor::RotateGravityDirection(float rad) //must be normalized!
{
    NxVec3 grav;
    m_pPhysXEngine->GetScene()->getGravity(grav);
    Matrix rotGrav = Matrix::CreateRotation(m_MoveDir, rad);
    grav = static_cast<NxVec3>(Vector3::Transform(grav, rotGrav).XYZ());
    
	m_pPhysXEngine->GetPhysXLock().lock();
    m_pPhysXEngine->GetScene()->setGravity(grav);
	m_pPhysXEngine->GetPhysXLock().unlock();
    
    m_RightDir = Vector3::Normalize(m_MoveDir).Cross(Vector3::Normalize(-grav));

    m_GravityRotation += rad;
}
void FluidsCharacterActor::RotateMoveDirection(float rad) //must be normalized!
{
    Matrix deRot = Matrix::CreateRotation(m_MoveDir, -m_GravityRotation);
    NxVec3 grav;
    m_pPhysXEngine->GetScene()->getGravity(grav);
    grav = static_cast<NxVec3>(Vector3::Transform(grav, deRot).XYZ());
    
    Vector3 up(-grav);
    up.Normalize();

    Matrix mtxRot = Matrix::CreateRotation(up, rad);
    m_MoveDir = Vector3::Transform(m_MoveDir, mtxRot).XYZ();
    
    Matrix mtxRotUp = Matrix::CreateRotation(m_MoveDir, m_GravityRotation);
    up = Vector3::Transform(up, mtxRotUp).XYZ();

    //m_pPhysXEngine->GetPhysXLock().lock();
    m_pPhysXEngine->GetScene()->setGravity(up * -grav.magnitude());
	//m_pPhysXEngine->GetPhysXLock().unlock();

    m_RightDir = Vector3::Normalize(m_MoveDir).Cross(Vector3::Normalize(-grav));

    m_MoveRotation += rad;
}
void FluidsCharacterActor::ChangeMoveSpeed(float speed)
{
    m_MoveSpeed = speed;
}

void FluidsCharacterActor::Tick(float dTime)
{
    Vector3 move = Vector3(0, 0, 0);
    move += -m_MoveDir * 10000 * dTime;
    
    if (CONTROLS->IsKeyDown(VK_UP))
        move += m_MoveDir * dTime * -20000;
    if (CONTROLS->IsKeyDown(VK_DOWN))
        move += m_MoveDir * dTime * 20000;
    if (CONTROLS->IsKeyDown(VK_RIGHT))
        move += m_RightDir * dTime * 20000;
    if (CONTROLS->IsKeyDown(VK_LEFT))
        move += m_RightDir * dTime * -20000;

    if (m_IsTouchingGround == false)
    {
    }
    else
	{
        if (CONTROLS->IsKeyDown(VK_CONTROL) == false)
        {
            m_CanSwitchGravity = true;
        }
        else if  (m_CanSwitchGravity == true)
        {
            if (CONTROLS->IsKeyDown(VK_RIGHT))
            {
                RotateGravityDirection(-PiOver2);
            }
            else if (CONTROLS->IsKeyDown(VK_LEFT))
            {
                RotateGravityDirection(PiOver2);
            }
            if (CONTROLS->IsKeyDown(VK_LEFT) || CONTROLS->IsKeyDown(VK_RIGHT)) 
            {
                m_pLevel->WakeUpAll();
                m_CanSwitchGravity = false;
            }
        }
	}

	if (m_pActor->getLinearVelocity().magnitude() < m_MoveSpeed)
		AddForce(move);

	CheckIfOnGround();

    NxVec3 grav;
    m_pPhysX->GetScene()->getGravity(grav);
    m_pCamera->SetFollowPosition(GetPosition());
    m_pCamera->SetFollowUp(-Vector3::Normalize(grav));
    m_pCamera->SetFollowDirection(Vector3::Normalize(Vector3::Normalize(-m_MoveDir) + Vector3::Normalize(grav) / 2.0f));
	
	m_pPhysXEngine->GetPhysXLock().lock();
    m_pEmitter->setGlobalPosition(GetPosition());
	m_pPhysXEngine->GetPhysXLock().unlock();
}
void FluidsCharacterActor::CheckIfOnGround()
{
	NxVec3 gravDir;
    m_pPhysXEngine->GetScene()->getGravity(gravDir);

	gravDir.normalize();

	NxRay ray(GetPosition() + m_Radius * gravDir, gravDir);
	m_IsTouchingGround = m_pPhysX->GetScene()->raycastAnyShape(ray, NX_ALL_SHAPES, 4294967295U, m_Radius / 10.0f);
}
void FluidsCharacterActor::Draw(const RenderContext* pRenderContext)
{
	m_pFluid->Draw(pRenderContext);
}
void FluidsCharacterActor::OnTriggerEnter(const Trigger* pTrigger)
{
    NxVec3 grav;
    m_pPhysX->GetScene()->getGravity(grav);
    Vector3 up(grav);
    up.Normalize();

    if (pTrigger->GetTriggerName().find(_T("left")) != tstring::npos)
    {
        RotateMoveDirection(-PiOver2);
    }
    else if (pTrigger->GetTriggerName().find(_T("right")) != tstring::npos)
    {
        RotateMoveDirection(PiOver2);
    }
    else if (pTrigger->GetTriggerName().find(_T("up")) != tstring::npos)
    {
    }
    else if (pTrigger->GetTriggerName().find(_T("down")) != tstring::npos)
    {
    }
	else if (pTrigger->GetTriggerName().find(_T("killinside")) != tstring::npos)
    {
		m_IsDead = true;
    }
	else if (pTrigger->GetTriggerName().find(_T("win")) != tstring::npos)
    {
		MessageBeep(0xFFFFFFFF);
		MessageBox(NULL, _T("You won!\nPress enter to play again"), _T("You WON!"), MB_ICONEXCLAMATION | MB_OK);
		m_IsDead = true;
    }
}
void FluidsCharacterActor::OnTriggerLeave(const Trigger* pTrigger)
{
	if (pTrigger->GetTriggerName().find(_T("killoutside")) != tstring::npos)
    {
		m_IsDead = true;
    }
}