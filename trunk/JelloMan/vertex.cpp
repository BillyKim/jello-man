#include "vertex.h"

void GetInputElementDesc(D3D10_INPUT_ELEMENT_DESC& desc, 
  const LPCSTR&              semanticName,
  UINT                       semanticIndex,
  DXGI_FORMAT                format,
  UINT                       inputSlot,
  UINT                       alignedByteOffset,
  D3D10_INPUT_CLASSIFICATION inputSlotClass,
  UINT                       instanceDataStepRate)
{
    desc.SemanticName = semanticName;
    desc.SemanticIndex = semanticIndex;
    desc.Format = format;
    desc.InputSlot = inputSlot;
    desc.AlignedByteOffset = alignedByteOffset;
    desc.InputSlotClass = inputSlotClass;
    desc.InstanceDataStepRate = instanceDataStepRate;
}


void AddInstancingData(vector<D3D10_INPUT_ELEMENT_DESC>& desc)
{
	desc.push_back(D3D10_INPUT_ELEMENT_DESC());
	desc.push_back(D3D10_INPUT_ELEMENT_DESC());
	desc.push_back(D3D10_INPUT_ELEMENT_DESC());
	desc.push_back(D3D10_INPUT_ELEMENT_DESC());
	GetInputElementDesc(desc[4], "WORLD", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0, D3D10_INPUT_PER_INSTANCE_DATA, 1);
	GetInputElementDesc(desc[5], "WORLD", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16, D3D10_INPUT_PER_INSTANCE_DATA, 1);
	GetInputElementDesc(desc[6], "WORLD", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 32, D3D10_INPUT_PER_INSTANCE_DATA, 1);
	GetInputElementDesc(desc[7], "WORLD", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 48, D3D10_INPUT_PER_INSTANCE_DATA, 1);
}