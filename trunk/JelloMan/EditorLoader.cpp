#include "EditorLoader.h"
#include "Level.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "RenderContext.h"
#include "SimpleObject.h"
#include "SimpleSoftbody.h"
#include "FixedJoint.h"

EditorLoader::EditorLoader(Level* pLevel, RenderContext* pRenderContext, ID3D10Device* pDXDevice, PhysX* pPhysXEngine)
	:	m_pLevel(pLevel),
		m_pRenderContext(pRenderContext),
		m_pDXDevice(pDXDevice),
		m_pPhysXEngine(pPhysXEngine)
{
}

EditorLoader::~EditorLoader()
{
}

/* LOADERS */
void EditorLoader::AddPointLight()
{
	Vector3 look = m_pRenderContext->GetCamera()->GetLook();
	look.Normalize();

	PointLightDesc pDesc;

	pDesc.position = (m_pRenderContext->GetCamera()->GetPosition() + look * 2);

	BYTE r = 180;
	BYTE g = 180;
	BYTE b = 200;
    BYTE a = 255;

	pDesc.color = Color(r, g, b, a);
	pDesc.multiplier = 2.0f;
	pDesc.attenuationStart = 0;
	pDesc.attenuationEnd = 5;
            
	PointLight* pl = new PointLight(pDesc);
	pl->InitEditor();

	m_pRenderContext->GetLightController()->AddLight(pl);

	cout << "Added pointlight\n";
}

void EditorLoader::AddSpotLight()
{
	Vector3 look = m_pRenderContext->GetCamera()->GetLook();
	look.Normalize();

	SpotLight* sp = new SpotLight();
		sp->SetPosition(m_pRenderContext->GetCamera()->GetPosition() + look * 2);
		sp->SetColor(Color(0.9f, 0.9f, 0.9f, 1.0f));
		sp->SetMulitplier(2.0f);
		sp->SetAttenuationStart(1);
		sp->SetAttenuationEnd(5);
		sp->SetOpeningsAngle(ToRadians(90));
		sp->Rotate(Vector3::Forward, PiOver2);
		sp->SetShadowMap(m_pDXDevice, ShadowMapType1024x1024);
			
	m_pRenderContext->GetLightController()->AddLight(sp);

	cout << "Added spotlight\n";
}

bool EditorLoader::AddLevelObject(	const tstring& modelPath,
									const tstring& physxModelPath,
									const tstring& normalPath,
									const tstring& diffusePath,
									const tstring& specPath,
									const tstring& glossPath	)
{
	if (modelPath != _T("") && physxModelPath != _T(""))
	{        
        if (physxModelPath.rfind(_T(".nxsoftbody")) == tstring::npos)
        {
            bool rigid = false;
            int ret = MessageBoxA(0, "Load as staticmesh?", "Loading: ", MB_ICONQUESTION | MB_YESNO);
		    switch (ret)
		    {
                case IDYES: rigid = false; break;
                default: rigid = true; break;
            }

            SimpleObject* pObj = new SimpleObject();

		    pObj->SetNormalPath(normalPath);

		    pObj->SetModelPath(modelPath);
		    pObj->SetPhysXModel(physxModelPath);
			
		    pObj->SetDiffusePath(diffusePath);
		    pObj->SetSpecPath(specPath);
		    pObj->SetGlossPath(glossPath);

		    pObj->SetRigid(rigid);

		    Vector3 vLook = m_pRenderContext->GetCamera()->GetLook();
		    vLook.Normalize();

		    pObj->Init(m_pPhysXEngine);

		    //pObj->Translate(m_pRenderContext->GetCamera()->GetPosition() + vLook * 10);

		    m_pLevel->AddLevelObject(pObj);
        }
        else
        {
		    Vector3 vLook = m_pRenderContext->GetCamera()->GetLook();
		    vLook.Normalize();
            SimpleSoftbody* pSoftbody = new SimpleSoftbody();

		    pSoftbody->SetModelPath(modelPath);
		    pSoftbody->SetPhysXModel(physxModelPath);
			
		    pSoftbody->SetDiffusePath(diffusePath);
		    pSoftbody->SetSpecPath(specPath);
		    pSoftbody->SetGlossPath(glossPath);
            pSoftbody->SetNormalPath(normalPath);

            pSoftbody->Init(m_pPhysXEngine);
            
            pSoftbody->SetPosition(m_pRenderContext->GetCamera()->GetPosition() + vLook * 10);

            m_pLevel->AddLevelObject(pSoftbody);
        }
		return true;
	}
	
	return false;
}

void EditorLoader::AddTrigger()
{
	Trigger* pTrigger(new Trigger());

	pTrigger->Init(m_pPhysXEngine, Vector3(2,2,2));

	Vector3 look = m_pRenderContext->GetCamera()->GetLook();
	look.Normalize();

	pTrigger->SetPosition(m_pRenderContext->GetCamera()->GetPosition() + look * 2);

	m_pLevel->AddLevelObject(pTrigger);
}

FixedJoint* EditorLoader::AddJoint(SimpleObject* pObj1, SimpleObject* pObj2)
{
	m_pPhysXEngine->GetScene()->resetJointIterator();
	unsigned int joints = m_pPhysXEngine->GetScene()->getNbJoints();

	bool bExists(false);
	NxJoint* j(0);

	for (unsigned int i(0); i < joints; ++i)
	{
		NxJoint* joint(m_pPhysXEngine->GetScene()->getNextJoint());

		NxActor* pActor1, *pActor2;
		joint->getActors(&pActor1,&pActor2);

		if ((pObj1->GetActor() == pActor1 || pObj1->GetActor() == pActor2) && (pObj2->GetActor() == pActor1 || pObj2->GetActor() == pActor2))
		{
			bExists = true;
			j = joint;
		}
	}

	if (bExists && j != 0)
	{
		m_pPhysXEngine->GetPhysXLock().lock();
		m_pPhysXEngine->GetScene()->releaseJoint(*j);
		m_pPhysXEngine->GetPhysXLock().unlock();
	}
	else
		return new FixedJoint(m_pPhysXEngine, pObj1->GetActor(), pObj2->GetActor());

	return 0;
}

void EditorLoader::LoadLevel(const tstring& fileName)
{
	if (fileName != _T(""))
	{
		string s(fileName.begin(), fileName.end());

		m_pLevel->Deserialize(s);
	}
}

void EditorLoader::SaveLevel(const tstring& fileName)
{
	if (fileName != _T(""))
	{
		string s(fileName.begin(), fileName.end());

		m_pLevel->Serialize(s);
	}
}
