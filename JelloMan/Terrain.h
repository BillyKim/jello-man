//#pragma once
//#include "D3Dutil.h"
//#include "Vertex.h"
//#include <vector>
//
//class NxActor;
//
//class Terrain
//{
//public:
//	/**
//	* Creates a new Terrain from a file name.
//	* @param fileName the location of the terrain.
//	* @param nrOfColumns the number of columns in the terrain.
//	* @param nrOfRows the number of rows in the terrain.
//	* @param tileSize the scale factor for the tiles.
//	* @param scaleHeight the scale factor for the height.
//	* @param offset a value to add to the height.
//	*/
//	Terrain(Level *pLevel, tstring fileName,tstring textureFileName,unsigned int nrOfColumns, unsigned int nrOfRows, 
//		float tileSize, float scaleHeight, float offset);
//	/**
//	* Destroys the Terrain object.
//	*/
//	virtual ~Terrain(void);
//
//	virtual void Initialize(ContentManager *pContentManager);
//	virtual void Tick(const InputState & refInputState){}
//	virtual void Draw(const RenderContext* pRenderContext);
//	
//private:
//	void CreateEffect(ContentManager *pContentManager);
//	void GetEffectVariables(ID3D10Effect* pEffect);
//	void BuildVertexBuffer();
//	void BuildIndexBuffer();
//
//	void AddTriangle (unsigned int a, unsigned int b, unsigned c);
//	void AddQuad (unsigned int a, unsigned int b, unsigned c, unsigned d);
//	bool LoadHeightMap();
//	void CreateVerticesAndTriangles();
//	void DefineInputlayout();
//	void CreatePhysicsTerrain();
//
//	unsigned int GetNumVertices();	
//	unsigned int GetNrOfTriangles();
//			
//	tstring m_FileName;
//	tstring m_TextureFileName;
//	unsigned int m_NumColumns;
//	unsigned int m_NumRows;
//	float m_TileSize;
//	float m_ScaleHeight;
//	float m_Offset;
//	
//	ID3D10InputLayout*			m_pVertexLayout;
//	ID3D10Buffer*				m_pVertexBuffer;
//	ID3D10Buffer*				m_pIndexBuffer;
//	ID3D10Effect*				m_pDefaultEffect;
//	ID3D10EffectTechnique*		m_pDefaultTechnique;
//	ID3D10ShaderResourceView *	m_pTextureRV;
//
//	ID3D10EffectMatrixVariable* m_pWorldViewProjectionVariable;
//	ID3D10EffectShaderResourceVariable* m_pDiffuseMapVariabele;
//
//	vector<unsigned short> m_VecHeightValues;
//	vector<VertexPosTex> m_VecVertices;
//	vector<DWORD> m_VecIndices;
//
//	// -------------------------
//	// Disabling default copy constructor and default assignment operator.
//	// If you get a linker error from one of these functions, your class is internally trying to use them. This is
//	// an error in your class, these declarations are deliberately made without implementation because they should never be used.
//	// -------------------------
//	Terrain(const Terrain& t);
//	Terrain& operator=(const Terrain& t);
//};
