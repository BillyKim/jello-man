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