#include "FluidsCharacterActor.h"
#include "Level.h"
#include "SpawnPoint.h"
#include "PhysX.h"
#include "Vector3.h"
#include "PhysXSphere.h"
#include "Fluid.h"

/* CONSTRUCTOR - DESTRUCTOR */
FluidsCharacterActor::FluidsCharacterActor(Level* pLevel)	:	m_pPhysXEngine(0),
										            m_pFluid(0),
										            m_pEmitter(0),
										            m_pCamera(0),
                                                    m_IsTouchingGround(true),
                                                    m_pLevel(pLevel),
                                                    m_CanSwitchGravity(true),
                                                    m_MoveDir(-Vector3::Right),
                                                    m_MoveSpeed(0.0f),
                                                    m_RightDir(0, 0 ,0),
                                                    m_IsDead(false),
													m_Radius(0.01f)
{
}

FluidsCharacterActor::~FluidsCharacterActor()
{
	SafeDelete(m_pFluid);

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
	InitActor(pPhysXEngine, sphere, true);

	m_pPhysXEngine = pPhysXEngine;

	SetPosition(startPos);
    ChangeMoveDirection(m_MoveDir); // calc right
    
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
    //ChangeMoveDirection(pSpawnPoint->GetForward());
    //ChangeGravityDirection(pSpawnPoint->GetUp());
    
}
void FluidsCharacterActor::ChangeGravityDirection(const Vector3& dir)
{
    NxVec3 grav;
    m_pPhysXEngine->GetScene()->getGravity(grav);
    grav = dir * grav.magnitude();
    m_pPhysXEngine->GetScene()->setGravity(grav);
    m_RightDir = Vector3::Normalize(m_MoveDir).Cross(Vector3::Normalize(-grav));

    //TODO
}
void FluidsCharacterActor::ChangeMoveDirection(const Vector3& dir) //must be normalized!
{
    m_MoveDir = dir;
    NxVec3 grav;
    m_pPhysXEngine->GetScene()->getGravity(grav);
    m_RightDir = Vector3::Normalize(m_MoveDir).Cross(Vector3::Normalize(-grav));
}
void FluidsCharacterActor::ChangeMoveSpeed(float speed)
{
    m_MoveSpeed = speed;
}

void FluidsCharacterActor::Tick(float dTime)
{
    Vector3 move = Vector3(0, 0, 0);

    NxVec3 grav;
    m_pPhysXEngine->GetScene()->getGravity(grav);
    move += m_MoveDir * m_MoveSpeed;
    
    if (CONTROLS->IsKeyDown(VK_UP))
        move += m_MoveDir * dTime * -10000;
    if (CONTROLS->IsKeyDown(VK_DOWN))
        move += m_MoveDir * dTime * 10000;
    if (CONTROLS->IsKeyDown(VK_RIGHT))
        move += m_RightDir * dTime * 10000;
    if (CONTROLS->IsKeyDown(VK_LEFT))
        move += m_RightDir * dTime * -10000;

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
                grav = m_RightDir * grav.magnitude(); 
            }
            else if (CONTROLS->IsKeyDown(VK_LEFT))
            {
                grav = m_RightDir * -grav.magnitude();
            }
            if (CONTROLS->IsKeyDown(VK_LEFT) || CONTROLS->IsKeyDown(VK_RIGHT)) 
            {
	            m_pPhysXEngine->GetPhysXLock().lock();
                m_pPhysXEngine->GetScene()->setGravity(grav);
	            m_pPhysXEngine->GetPhysXLock().unlock();
                m_pLevel->WakeUpAll();
                m_RightDir = Vector3::Normalize(m_MoveDir).Cross(Vector3::Normalize(-grav));
                m_CanSwitchGravity = false;
            }
        }
	}

	if (m_pActor->getLinearVelocity().magnitude() < 5.5f)
		AddForce(move);

	CheckIfOnGround();

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

	/*BX2D->SetColor(255,0,255);

	D3D10_VIEWPORT viewPort;
	viewPort.TopLeftX = 0;
	viewPort.TopLeftY = 0;
	viewPort.MinDepth = 0.0f;
	viewPort.MaxDepth = 1.0f;
	viewPort.Width = BX2D->GetWindowSize().width;
	viewPort.Height = BX2D->GetWindowSize().height;

	D3DXVECTOR3 pos3D(GetPosition());
	
	D3DXVECTOR3 pos2D;
	D3DXMATRIX projection(pRenderContext->GetCamera()->GetProjection());
	D3DXMATRIX view(pRenderContext->GetCamera()->GetView());
	D3DXMATRIX world;

	D3DXVec3Project(&pos2D, &pos3D, &viewPort, &projection, &view, 0);

	BX2D->SetColor(255,0,255);
	BX2D->FillEllipse(pos2D.x, pos2D.y, 5, 5);*/


	// DEBUG
	/*
	tstringstream stream;
	stream << Actor::m_pActor->getLinearVelocity().magnitude();

	BX2D->DrawString(stream.str(), 500,200);
	*/
}