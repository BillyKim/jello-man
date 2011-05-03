#include "Level.h"
#include "ContentManager.h"
#include "PhysXSphere.h"
#include "PhysXBox.h"
#include "SpotLight.h"
#include "PointLight.h"

// CONSTRUCTOR - DESTRUCTOR
Level::Level(ID3D10Device* pDXDevice)	:	
                m_pDXDevice(pDXDevice),
				m_pRenderContext(0),
				m_pBaseGrid(new BaseGrid(pDXDevice)),
				m_bShowGrid(false),
				m_bTickCharacter(false),
				m_pCharacter(0),
				m_bEditor(true)
{

}


Level::~Level()
{
	delete m_pBaseGrid;
	
	Clear();
}

// GENERAL
void Level::Initialize(PhysX* pPhysXEngine, Camera* pTrackingCamera)
{
	m_pPhysXEngine = pPhysXEngine;
	
	m_pBaseGrid->Init();

	// CHARACTER
	Character* pCharacter = new Character(pTrackingCamera);

	pCharacter->UseNormalMap(false);
	pCharacter->UseSimplifiedPhysXMesh(false);

	pCharacter->SetModelPath(_T("Content/Models/jman.binobj"));
	pCharacter->SetPhysXModelPath(_T("Content/Models/jman.nxconcave"));

	pCharacter->SetDiffusePath(_T("Content/Textures/weapon_diffuse.png"));
	pCharacter->SetSpecPath(_T("Content/Textures/weapon_spec.png"));
	pCharacter->SetGlossPath(_T("Content/Textures/weapon_gloss.png"));

	pCharacter->SetRigid(false);

	pCharacter->Init(pPhysXEngine);

	pCharacter->Translate(Vector3(0,0,0));

	m_pCharacter = pCharacter;

	m_pLevelObjects.push_back(pCharacter);

	// LOAD NEW LEVELOBJECT
	LevelObject* pLevelObject = new LevelObject();

	pLevelObject->UseNormalMap(true);
	pLevelObject->UseSimplifiedPhysXMesh(false);

	pLevelObject->SetModelPath(_T("Content/Models/as_val.obj"));
	pLevelObject->SetPhysXModelPath(_T("Content/Models/as_val.nxconcave"));

	pLevelObject->SetDiffusePath(_T("Content/Textures/weapon_color.png"));
	pLevelObject->SetSpecPath(_T("Content/Textures/weapon_spec.png"));
	pLevelObject->SetGlossPath(_T("Content/Textures/weapon_gloss.png"));
	pLevelObject->SetNormalPath(_T("Content/Textures/weapon_normal.png"));

	pLevelObject->SetRigid(true);

	pLevelObject->Init(pPhysXEngine);

	pLevelObject->Translate(Vector3(0,200,0));

	m_pLevelObjects.push_back(pLevelObject);
}

void Level::Tick(const float dTime)
{
	for (vector<ILevelObject*>::iterator it = m_pLevelObjects.begin(); it != m_pLevelObjects.end(); ++it)
	{
		if (!m_bTickCharacter)
		{
			if ((*it) != m_pCharacter)
				(*it)->Tick(dTime);
		}
		else
			(*it)->Tick(dTime);
		
	}
}

void Level::AddLevelObject(ILevelObject* pLevelObject)
{
	m_pLevelObjects.push_back(pLevelObject);
}

// SERIALISATION
ISerializable* GetObject(DWORD id)
{
    switch (id)
    {
        case SerializeTypes::SpotLight: return new SpotLight();
        case SerializeTypes::PointLight: return new PointLight();
        default: ASSERT(false, _T("File corrupt")); return 0;
    }
}
void Level::Serialize(const string& path)
{
    Serializer s(path);

    s.Begin(false);
    for_each(m_pLevelObjects.cbegin(), m_pLevelObjects.cend(), [&](ILevelObject* obj)
    {
        ISerializable* is = dynamic_cast<ISerializable*>(obj);
        if (is != 0)
            s.Serialize(is);
    });
    for_each(m_pRenderContext->GetLightController()->GetLights().cbegin(), m_pRenderContext->GetLightController()->GetLights().cend(), [&](Light* light)
    {
        ISerializable* is = dynamic_cast<ISerializable*>(light);
        if (is != 0)
            s.Serialize(is);
    });
    s.End();
}
void Level::Deserialize(const string& path)
{
    Serializer s(path);

    s.Begin(true);
    
    Clear();
    while (s.eof() == false)
    {
        ISerializable* obj = s.Deserialize(GetObject);
        
        //Test is lvlObj
        ILevelObject* lvlObj = dynamic_cast<ILevelObject*>(obj);
        if (lvlObj != 0)
        {
            m_pLevelObjects.push_back(lvlObj);
        }
        else
        {
            Light* l = dynamic_cast<Light*>(obj);
            if (l != 0)
            {
                m_pRenderContext->GetLightController()->AddLight(l);              
            }
        }
    }
    s.End();
}
void Level::Clear()
{   
    m_pRenderContext->GetLightController()->Clear();
    for_each(m_pLevelObjects.cbegin(), m_pLevelObjects.cend(), [](ILevelObject* obj)
    {
		delete obj;
    });
    m_pLevelObjects.clear();
}

// DRAW
void Level::DrawDeferred(RenderContext* pRenderContext)
{
	m_pRenderContext = pRenderContext;

	for (vector<ILevelObject*>::iterator it = m_pLevelObjects.begin(); it != m_pLevelObjects.end(); ++it)
	{
		(*it)->Draw(pRenderContext);
	}
}

void Level::DrawShadowMap(RenderContext* pRenderContext, PreShadowEffect* pPreShadowEffect)
{
    for (vector<ILevelObject*>::const_iterator lIt = m_pLevelObjects.cbegin(); lIt != m_pLevelObjects.cend(); ++lIt)
	{
		ILevelObject* lobj = *lIt;
        lobj->DrawShadow(pRenderContext, pPreShadowEffect);
	}
}

void Level::DrawForward(const RenderContext* pRenderContext)
{
	if (m_bShowGrid)
		m_pBaseGrid->Draw(pRenderContext);

	// DRAW LIGHTS
	if (m_bEditor)
	{
		BX2D->SetAntiAliasing(true);

		for (unsigned int i = 0; i < pRenderContext->GetLightController()->GetLights().size(); ++i)
		{
			m_pRenderContext->GetLightController()->GetLights()[i]->Draw(m_pRenderContext);
		}

		BX2D->SetAntiAliasing(false);
	}
}