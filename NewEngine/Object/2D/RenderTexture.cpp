#include "RenderTexture.h"
#include "Camera.h"
#include "RenderBase.h"
#include "RenderWindow.h"
#include "TextureManager.h"
#include <memory>
using namespace std;

//const float RenderTexture::sClearColor[4] = { 0.25f,0.5f,0.1f,1.0f };
const float RenderTexture::sClearColor[4] = { 0.f,0.f,0.f,1.0f };

void RenderTexture::PrevDrawScene()
{
	RenderBase* renderBase = RenderBase::GetInstance();
	RenderWindow* renderWindow = RenderWindow::GetInstance().get();

	// ���\�[�X�o���A��ύX�i�V�F�[�_�[���\�[�X -> �`��\�j
	for (int i = 0; i < buffers.size(); i++)
	{
		CD3DX12_RESOURCE_BARRIER resourceBarrier =
			CD3DX12_RESOURCE_BARRIER::Transition(
				buffers[i].Get(),
				D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
				D3D12_RESOURCE_STATE_RENDER_TARGET);
		renderBase->GetCommandList()->ResourceBarrier(1, &resourceBarrier);
	}

	// RTV CPU�n���h��
	std::vector<D3D12_CPU_DESCRIPTOR_HANDLE> rtvCpuHandle;
	for (int i = 0; i < buffers.size(); i++)
	{
		rtvCpuHandle.push_back(renderTargets[i].GetCpuHandle());
	}

	// DSV CPU�n���h��
	D3D12_CPU_DESCRIPTOR_HANDLE dsvCpuHandle = depthBuffer.GetCpuHandle();

	// �����_�[�^�[�Q�b�g���Z�b�g
	renderBase->GetCommandList()->OMSetRenderTargets((UINT)renderTargets.size(), rtvCpuHandle.data(), false, &dsvCpuHandle);

	// �r���[�|�[�g�̐ݒ�
	viewports_.resize(buffers.size());
	for (int i = 0; i < buffers.size(); i++)
	{
		viewports_[i] =
			CD3DX12_VIEWPORT(
				0.f, 0.f,
				renderWindow->GetWindowSize().x,
				renderWindow->GetWindowSize().y);
	}
	renderBase->GetCommandList()->RSSetViewports((UINT)viewports_.size(), viewports_.data());

	// �V�U�[��`�̐ݒ�
	scissorRects_.resize(buffers.size());
	for (int i = 0; i < buffers.size(); i++)
	{
		scissorRects_[i] =
			CD3DX12_RECT(
				0, 0,
				(LONG)renderWindow->GetWindowSize().x,
				(LONG)renderWindow->GetWindowSize().y);
	}
	renderBase->GetCommandList()->RSSetScissorRects((UINT)scissorRects_.size(), scissorRects_.data());

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
	for (int i = 0; i < buffers.size(); i++)
	{
		CD3DX12_RESOURCE_BARRIER resourceBarrier =
			CD3DX12_RESOURCE_BARRIER::Transition(
				buffers[i].Get(),
				D3D12_RESOURCE_STATE_RENDER_TARGET,
				D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
		renderBase->GetCommandList()->ResourceBarrier(1, &resourceBarrier);
	}
}