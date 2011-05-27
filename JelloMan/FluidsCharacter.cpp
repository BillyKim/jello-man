#include "FluidsCharacter.h"

/* CONSTRUCTOR - DESTRUCTOR */
FluidsCharacter::FluidsCharacter()	:	m_pPhysXEngine(0),
										m_pFluid(0),
										m_pEmitter(0),
										m_pCamera(0),
                                        m_IsTouchingGround(true)
{
}

FluidsCharacter::~FluidsCharacter()
{
	SafeDelete(m_pFluid);

	m_pEmitter = 0;
	m_pPhysXEngine = 0;
	m_pCamera = 0;
}

/* GENERAL */
void FluidsCharacter::Init(ID3D10Device* pDXDevice, PhysX* pPhysXEngine, Graphics::Camera::FollowCamera* pCamera, unsigned int maxParticles, Vector3 startPos)
{
	ASSERT(pDXDevice != 0, "DXDevice error when creating fluidscharacter!");
	ASSERT(pPhysXEngine->GetScene() != 0, "PhysXScene error when creating fluidscharacter!");
	ASSERT(pCamera != 0, "FollowCamera error when creating fluidscharacter!");

	m_pCamera = pCamera;

    InitCharacterAsBox(pPhysXEngine, Vector3(0.5f, 0.5f, 0.5f));

	SetPosition(startPos);
    
	m_pPhysXEngine = pPhysXEngine;

	// FLUID
	int MAX_PARTICLES = (int)maxParticles;

	// setup fluid descriptor
	NxFluidDesc fluidDesc;
    fluidDesc.maxParticles                  = MAX_PARTICLES;
    fluidDesc.kernelRadiusMultiplier		= 1.0f;
    fluidDesc.restParticlesPerMeter			= 10.0f;
	fluidDesc.motionLimitMultiplier			= 1.0f;
	fluidDesc.packetSizeMultiplier			= 8;
    fluidDesc.collisionDistanceMultiplier   = 0.1f;
    fluidDesc.stiffness						= 50.0f;
    fluidDesc.viscosity						= 40.0f;
	fluidDesc.restDensity					= 10.0f;
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
	emitterDesc.particleLifetime = 1.0f; // in seconds

	////attach to actor
	//emitterDesc.flags |= NX_FEF_ADD_BODY_VELOCITY;
	//emitterDesc.repulsionCoefficient = 300.0f;

	emitterDesc.relPose.M.id();
	emitterDesc.relPose.M.rotX(NxHalfPiF32);
	emitterDesc.relPose.t = NxVec3(0.0f,50.0f,0);
	
	m_pFluid = new Fluid(pPhysXEngine->GetScene(), fluidDesc, Color(1.0f, 0.0f, 1.0f, 1.0f), 0.0003f, pDXDevice);
	ASSERT(m_pFluid, "fluid creation failed");

	m_pEmitter = m_pFluid->GetNxFluid()->createEmitter(emitterDesc);

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
}

void FluidsCharacter::Tick(float dTime)
{
    Vector3 move = Vector3(0, 0, 0);
    if (CONTROLS->IsKeyDown(VK_UP))
        move += Vector3::Forward * dTime * 10;
    if (CONTROLS->IsKeyDown(VK_DOWN))
        move += Vector3::Forward * dTime * -10;
    if (CONTROLS->IsKeyDown(VK_RIGHT))
        move += Vector3::Right * dTime * 10;
    if (CONTROLS->IsKeyDown(VK_LEFT))
        move += Vector3::Right * dTime * -10;

    NxVec3 grav;
    m_pPhysXEngine->GetScene()->getGravity(grav);
    move += grav * dTime;

	m_pPhysXEngine->GetPhysXLock().lock();
    PhysXCharacterCollisionType coll = Move(move);
	m_pPhysXEngine->GetPhysXLock().unlock();

    m_IsTouchingGround = coll & PhysXCharacterCollisionType_Down || coll & PhysXCharacterCollisionType_Up;

    if (m_IsTouchingGround && CONTROLS->IsKeyPressed(VK_SPACE))
	{
        m_pPhysXEngine->GetScene()->setGravity(-grav);
	}



	//bool wait = true;
	//while (wait)
	//{
	//	if (m_pPhysXEngine->GetPhysXLock().try_lock())
	//		break;
	//}
	
	
	m_pPhysXEngine->GetPhysXLock().lock();
    m_pEmitter->setGlobalPosition(GetPosition());
	m_pPhysXEngine->GetPhysXLock().unlock();

}

void FluidsCharacter::Draw(RenderContext* pRenderContext)
{
	m_pFluid->Draw(pRenderContext);

	BX2D->SetColor(255,0,255);

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
	BX2D->FillEllipse(pos2D.x, pos2D.y, 5, 5);


	// DEBUG
	/*
	tstringstream stream;
	stream << Actor::m_pActor->getLinearVelocity().magnitude();

	BX2D->DrawString(stream.str(), 500,200);
	*/
}