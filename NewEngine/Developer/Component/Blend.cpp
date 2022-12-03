#include "Blend.h"
#include "RenderBase.h"

void Blend::SetBlendMode(const int& blendmode)
{
	RenderBase* renderBase = RenderBase::GetInstance().get();

	//switch (blendmode)
	//{
	//case Alpha2D:
	//	renderBase->GetCommandList()->SetPipelineState(renderBase->GetPipelineStateAlpha2D());
	//	renderBase->GetCommandList()->SetGraphicsRootSignature(renderBase->GetRootSignature());
	//	renderBase->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//	break;

	//case AlphaLine:
	//	renderBase->GetCommandList()->SetPipelineState(renderBase->GetPipelineStateLine());
	//	renderBase->GetCommandList()->SetGraphicsRootSignature(renderBase->GetRootSignature());
	//	renderBase->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);
	//	break;

	//case AlphaRenderTexture:
	//	renderBase->GetCommandList()->SetPipelineState(renderBase->GetPipelineStateAlphaRenderTexture());
	//	renderBase->GetCommandList()->SetGraphicsRootSignature(renderBase->GetRootSignature());
	//	renderBase->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//	break;

	//case ParticleAlpha:
	//	renderBase->GetCommandList()->SetPipelineState(renderBase->GetPipelineStateParticleAlpha());
	//	renderBase->GetCommandList()->SetGraphicsRootSignature(renderBase->GetRootSignature());
	//	renderBase->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST);
	//	break;

	//default:
	//	break;
	//}
}