//#include "StdAfx.h"
//#include "Terrain.h"
//#include "Level.h"
//#include "GraphCamera.h"
//
//
//
//#include <iostream>
//#include <fstream>
//#include <vector>
//using namespace std;
//
//Terrain::Terrain(Level *pLevel, tstring fileName,tstring textureFileName,unsigned int nrOfColumns,unsigned int nrOfRows, 
//				 float tileSize, float scaleHeight, float offset)
//	:LevelElement(pLevel)
//	,m_FileName(fileName)
//	,m_TextureFileName(textureFileName)
//	,m_NumColumns(nrOfColumns)
//	,m_NumRows(nrOfRows)
//	,m_TileSize(tileSize)
//	,m_ScaleHeight(scaleHeight)
//	,m_Offset(offset)
//	,m_pVertexLayout(0)
//	,m_pVertexBuffer(0)
//	,m_pIndexBuffer(0)
//	,m_pDefaultEffect(0)	
//	,m_pDefaultTechnique(0)	
//	,m_pTextureRV(0)
//	,m_pWorldViewProjectionVariable(NULL)
//	,m_pDiffuseMapVariabele(0)
//	
//{
//}
//
//Terrain::~Terrain(void)
//{
//	SafeRelease(m_pVertexBuffer );
//	SafeRelease(m_pVertexLayout);
//	SafeRelease(m_pIndexBuffer);
//}
//void Terrain::Initialize(ContentManager *pContentManager)
//{
//	CreateEffect(pContentManager);
//	m_pTextureRV = pContentManager->GetTexture(m_pLevel->GetDevice(), m_TextureFileName);
//
//	if(LoadHeightMap())
//	{
//		CreateVerticesAndTriangles();
//		DefineInputlayout();
//		BuildVertexBuffer();
//		BuildIndexBuffer();
//		CreatePhysicsTerrain();
//
//	}
//}
//void Terrain::Draw(const RenderContext* pRenderContext)
//{
//	if(!m_pDefaultTechnique )
//	{
//		MessageBox(0,_T("No Technique"),_T("ERROR"),0);
//		return;
//	}
//	if(!m_pVertexBuffer)
//	{
//		MessageBox(0,_T("No Vertices"),_T("ERROR"),0);
//		return;
//	}	
//	// Update Shader matrix variables
//	D3DXMATRIX matView = pRenderContext->GetCamera()->GetView();
//	D3DXMATRIX matProj = pRenderContext->GetCamera()->GetProj();
//	m_pWorldViewProjectionVariable->SetMatrix((float*)&(m_World*matView*matProj));
//
//	// Set the input layout
//    m_pLevel->GetDevice()->IASetInputLayout( m_pVertexLayout );
//
//	//set the texture variabele
//	m_pDiffuseMapVariabele->SetResource(m_pTextureRV);	
//
//
//	// Set vertex buffer(s)
//    UINT offset = 0;
//	UINT vertexBufferStride = sizeof(VertexPosTex);
//    m_pLevel->GetDevice()->IASetVertexBuffers( 0, 1, &m_pVertexBuffer, &vertexBufferStride, &offset );
//   	
//	// Set index buffer
//	m_pLevel->GetDevice()->IASetIndexBuffer(m_pIndexBuffer,DXGI_FORMAT_R32_UINT,0);
//
//    // Set primitive topology
//    m_pLevel->GetDevice()->IASetPrimitiveTopology( D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
//
//    D3D10_TECHNIQUE_DESC techDesc;
//    m_pDefaultTechnique->GetDesc( &techDesc );
//    for( UINT p = 0; p < techDesc.Passes; ++p )
//    {
//        m_pDefaultTechnique->GetPassByIndex(p)->Apply(0);
//		m_pLevel->GetDevice()->DrawIndexed( m_VecIndices.size(), 0, 0 ); 
//    }
//}
//
//void Terrain::CreateEffect(ContentManager *pContentManager)
//{
//	m_pDefaultEffect = pContentManager->GetEffect(m_pLevel->GetDevice(),  _T("./Effect/Terrain.fx"));
//	
//	//get first technique found
//	m_pDefaultTechnique = m_pDefaultEffect->GetTechniqueByIndex(0);
//	if(!m_pDefaultTechnique->IsValid())MessageBox(0,_T("Technique not valid"),_T("ERROR"),0);
//
//	GetEffectVariables(m_pDefaultEffect);
//}
//
//void Terrain::GetEffectVariables(ID3D10Effect* pEffect)
//{
//	//get effect variables
//	m_pWorldViewProjectionVariable = pEffect->GetVariableBySemantic("WorldViewProjection")->AsMatrix();
//	if(!m_pWorldViewProjectionVariable->IsValid())MessageBox(0,_T("Getting EffectSemantic WorldViewProjection Failed"),_T("ERROR"),0);
//
//	m_pDiffuseMapVariabele = pEffect->GetVariableBySemantic( "DiffuseMap" )->AsShaderResource();
//	if(!m_pDiffuseMapVariabele->IsValid())MessageBox(0,_T("Getting EffectSemantic DiffuseMap Failed"),_T("ERROR"),0);
//}
//
//unsigned int Terrain::GetNumVertices()
//{
//	return m_NumColumns*m_NumRows;
//}
//	
//
//unsigned int Terrain::GetNrOfTriangles()
//{
//	return 2 * (m_NumColumns-1) * (m_NumRows-1);
//}
//
//bool Terrain::LoadHeightMap(){
//	
//	//create already sized vector to store height values
//	m_VecHeightValues.resize( GetNumVertices(), 0 );
//	//open binary file 
//	std::ifstream inFile;
//	inFile.open(m_FileName.c_str(),std::ios_base::binary);
//	if ( !inFile )
//	{
//		MessageBox(0,L"Terrain file not found", _T("ERROR"),0);
//		return false;
//	}
//	//read height info
//	inFile.read((char*)&m_VecHeightValues[0], (std::streamsize)(GetNumVertices() * sizeof(unsigned short)) );	
//	inFile.close();
//
//	return true;
//}
//
//void Terrain::CreateVerticesAndTriangles()
//{
//
//	//Create DirectX10 Mesh 5.15
//	int numQuadX = m_NumRows-1;
//	int numQuadZ = m_NumColumns-1;
//	double gridWidth = numQuadX * m_TileSize;
//	double gridDepth = numQuadZ * m_TileSize;
//
//
//	//create a grid, centered around origin
//	//fill vertex vector
//	m_VecVertices.reserve(GetNumVertices());
//
//	for(unsigned int row=0;row<m_NumRows;row++)
//	{
//		for(unsigned int col=0;col<m_NumColumns;col++)
//		{
//			VertexPosTex v;
//			v.pos.x = float( m_TileSize*col);
//			v.pos.y = float(m_VecHeightValues[row*m_NumColumns+col]*m_ScaleHeight+m_Offset);
//			v.pos.z = float( m_TileSize*row);
//			v.tex.x = float(col)/m_NumColumns;
//			v.tex.y = float(row)/m_NumRows;
//			//float xcoord =  -gridWidth/2 + m_TileSize*x;
//			//float zcoord = -gridDepth/2 + m_TileSize*z;
//			//float ycoord = m_VecHeightValues[x+z*m_NumRows];
//			//float u = x/m_NumColumns;
//			//float v = z/m_NumRows;
//			//VertexPosTex v(xcoord, ycoord, zcoord,u,v);
//			m_VecVertices.push_back(v);
//		}
//	}
//
//	//create triangles
//	unsigned int numTriangles = numQuadX * numQuadZ * 6;
//	//resize vector for speed purposes
//	m_VecIndices.reserve(numTriangles);
//
//	for(unsigned int row = 0; row < (m_NumRows-1); ++row)
//	{
//		for(unsigned int column = 0 ; column < (m_NumColumns-1); ++column)
//		{
//			int index1 = row * m_NumColumns + column;
//			int index2 = index1 + 1;
//
//			int index3 = index1 + m_NumColumns;
//			int index4 = index3 + 1;
//
//			AddQuad(index1,index2, index3, index4);
//		} 
//	}
//}
//
//void Terrain::AddTriangle (unsigned int a, unsigned int b, unsigned c)
//{
//	m_VecIndices.push_back(a);
//	m_VecIndices.push_back(b);
//	m_VecIndices.push_back(c);
//}
//
//void Terrain::AddQuad (unsigned int a, unsigned int b, unsigned c, unsigned d)
//{
//	AddTriangle(a,c,d);
//	AddTriangle(a,d,b);
//}
//void Terrain::DefineInputlayout()
//{
//	// Define the input layout DO NOT USE D3D10_APPEND_ALIGNED_ELEMENT 
//	D3D10_INPUT_ELEMENT_DESC layout[] =
//	{
//		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D10_INPUT_PER_VERTEX_DATA, 0 },  
//		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D10_INPUT_PER_VERTEX_DATA, 0 }
//	};
//	UINT numElements = sizeof(layout)/sizeof(layout[0]);
//
//    // Create the input layout
//    D3D10_PASS_DESC PassDesc;
//	// Get the pass decriptor from the effect technique
//    m_pDefaultTechnique->GetPassByIndex( 0 )->GetDesc( &PassDesc );
//    HR(m_pLevel->GetDevice()->CreateInputLayout( layout, numElements, PassDesc.pIAInputSignature, PassDesc.IAInputSignatureSize, &m_pVertexLayout ));
//
//}
//
//void Terrain::BuildVertexBuffer()
//{
//	//fill a buffer description to copy the vertexdata into graphics memory
//	D3D10_BUFFER_DESC bd;
//	bd.Usage = D3D10_USAGE_DEFAULT;
//	bd.ByteWidth = sizeof( VertexPosTex ) * m_VecVertices.size();
//	bd.BindFlags = D3D10_BIND_VERTEX_BUFFER;
//	bd.CPUAccessFlags = 0;
//	bd.MiscFlags = 0;
//
//	D3D10_SUBRESOURCE_DATA initData;
//	initData.pSysMem = m_VecVertices.data();
//	//create a ID3D10Buffer in graphics memory containing the vertex info
//	HR(m_pLevel->GetDevice()->CreateBuffer( &bd, &initData, &m_pVertexBuffer ));
//}
//void Terrain::BuildIndexBuffer()
//{
//	D3D10_BUFFER_DESC ibd;
//    ibd.Usage = D3D10_USAGE_IMMUTABLE;
//    ibd.ByteWidth = sizeof(DWORD) * m_VecIndices.size();
//    ibd.BindFlags = D3D10_BIND_INDEX_BUFFER;
//    ibd.CPUAccessFlags = 0;
//    ibd.MiscFlags = 0;
//    D3D10_SUBRESOURCE_DATA initData;
//    initData.pSysMem = m_VecIndices.data();
//    HR(m_pLevel->GetDevice()->CreateBuffer(&ibd, &initData, &m_pIndexBuffer));
//}
//
//void Terrain::CreatePhysicsTerrain()
//{
//	 NxHeightFieldDesc heightFieldDesc;    
//	 heightFieldDesc.nbColumns           = m_NumColumns;    
//	 heightFieldDesc.nbRows              = m_NumRows;    
//	 heightFieldDesc.verticalExtent      = -10;    
//	 heightFieldDesc.convexEdgeThreshold = 0;    
//	 //Allocate storage for samples.    
//	 heightFieldDesc.samples             = new NxU32[m_NumColumns*m_NumRows];    
//	 heightFieldDesc.sampleStride        = sizeof(NxU32); 
//	 
//	 NxU8* currentByte = (NxU8*)heightFieldDesc.samples;    
//	 for (NxU32 row = 0; row < m_NumRows; row++)
//	 {        
//		 for (NxU32 column = 0; column < m_NumColumns; column++)            
//		 {            
//			 NxHeightFieldSample* currentSample = (NxHeightFieldSample*)currentByte;
//			 currentSample->height         = m_VecHeightValues[column*m_NumRows+row]/2; //from unsigned to signed, prevent clipping
//			 //Desired height value. Signed 16 bit integer.           
//			 currentSample->materialIndex0 = 0;            
//			 currentSample->materialIndex1 = 0;            
//			 currentSample->tessFlag = 0;            
//			 currentByte += heightFieldDesc.sampleStride;
//		 }        
//	 }    
//	 NxHeightField* heightField = m_pLevel->GetPhysicsScene()->getPhysicsSDK().createHeightField(heightFieldDesc);    
//
//	
//	 //Data has been copied therefore free the buffer.    
//	 delete[] heightFieldDesc.samples;
//
//	 NxHeightFieldShapeDesc heightFieldShapeDesc;   
//	 heightFieldShapeDesc.heightField     = heightField;    
//	 heightFieldShapeDesc.heightScale     = m_ScaleHeight*2;//undo division in currentSample->height  
//	 heightFieldShapeDesc.rowScale        = m_TileSize;
//	 heightFieldShapeDesc.columnScale     = m_TileSize;    
//	 heightFieldShapeDesc.materialIndexHighBits = 0;    
//	 heightFieldShapeDesc.holeMaterial = -1;
//	 //heightFieldShapeDesc.
//	 NxActorDesc actorDesc;    
//	 actorDesc.shapes.pushBack(&heightFieldShapeDesc);    
//	 actorDesc.globalPose.t = NxVec3(0,m_Offset,0);    
//	 NxActor* newActor = m_pLevel->GetPhysicsScene()->createActor(actorDesc);
//
//}
