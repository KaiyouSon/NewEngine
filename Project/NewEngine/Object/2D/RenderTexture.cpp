#include "RenderTexture.h"
#include "Camera.h"
#include "RenderBase.h"
#include "RenderWindow.h"
#include "TextureManager.h"
#include <memory>
using namespace std;

const float RenderTexture::sClearColor[4] = { 0.f,0.f,0.f,1.0f };

void RenderTexture::PrevDrawScene()
{
	RenderBase* renderBase = RenderBase::GetInstance();
	RenderWindow* renderWindow = RenderWindow::GetInstance().get();

	// ���\�[�X�o���A��ύX�i�V�F�[�_�[���\�[�X -> �`��\�j
	for (int i = 0; i < mBufferResources.size(); i++)
	{
		CD3DX12_RESOURCE_BARRIER resourceBarrier =
			CD3DX12_RESOURCE_BARRIER::Transition(
				mBufferResources[i].buffer.Get(),
				D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
				D3D12_RESOURCE_STATE_RENDER_TARGET);
		renderBase->GetCommandList()->ResourceBarrier(1, &resourceBarrier);
	}

	// RTV CPU�n���h��
	std::vector<D3D12_CPU_DESCRIPTOR_HANDLE> rtvCpuHandle;
	for (int i = 0; i < mBufferResources.size(); i++)
	{
		rtvCpuHandle.push_back(renderTargets[i].GetBufferResource()->rtvHandle.cpu);
	}

	// DSV CPU�n���h��
	D3D12_CPU_DESCRIPTOR_HANDLE dsvCpuHandle = depthBuffer.GetBufferResource()->dsvHandle.cpu;

	// �����_�[�^�[�Q�b�g���Z�b�g
	renderBase->GetCommandList()->OMSetRenderTargets((uint32_t)renderTargets.size(), rtvCpuHandle.data(), false, &dsvCpuHandle);

	// �r���[�|�[�g�̐ݒ�
	mViewports.resize(mBufferResources.size());
	for (int i = 0; i < mBufferResources.size(); i++)
	{
		mViewports[i] =
			CD3DX12_VIEWPORT(
				0.f, 0.f,
				size.x,
				size.y);
	}
	renderBase->GetCommandList()->RSSetViewports((uint32_t)mViewports.size(), mViewports.data());

	// �V�U�[��`�̐ݒ�
	mScissorRects.resize(mBufferResources.size());
	for (int i = 0; i < mBufferResources.size(); i++)
	{
		mScissorRects[i] =
			CD3DX12_RECT(
				0, 0,
				(LONG)size.x,
				(LONG)size.y);
	}
	renderBase->GetCommandList()->RSSetScissorRects((uint32_t)mScissorRects.size(), mScissorRects.data());

	// �S��ʃN���A
	for (int i = 0; i < rtvCpuHandle.size(); i++)
	{
		renderBase->GetCommandList()->ClearRenderTargetView(rtvCpuHandle[i], sClearColor, 0, nullptr);
	}

	// �[�x�o�b�t�@�̃N���A
	renderBase->GetCommandList()->ClearDepthStencilView(dsvCpuHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
}
void RenderTexture::PostDrawScene()
{
	RenderBase* renderBase = RenderBase::GetInstance();

	// ���\�[�X�o���A��ύX�i�`��\ -> �V�F�[�_�[���\�[�X�j
	for (int i = 0; i < mBufferResources.size(); i++)
	{
		CD3DX12_RESOURCE_BARRIER resourceBarrier =
			CD3DX12_RESOURCE_BARRIER::Transition(
				mBufferResources[i].buffer.Get(),
				D3D12_RESOURCE_STATE_RENDER_TARGET,
				D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
		renderBase->GetCommandList()->ResourceBarrier(1, &resourceBarrier);
	}
}

std::vector<BufferResource>* RenderTexture::GetBufferResources()
{
	return &mBufferResources;
}
