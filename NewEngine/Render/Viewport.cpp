#include "Viewport.h"
#include "RenderBase.h"
#include "RenderWindow.h"
#include <memory>
using namespace std;

Viewport::Viewport() :
	leftTopPos_(0), size_(0), minDepth_(0), maxDepth_(0)
{
}

void Viewport::Update()
{
	RenderBase* renderBase = RenderBase::GetInstance();// .get();

	//----------------------- �r���[�|�[�g�̐ݒ�R�}���h -----------------------//
	// �r���[�|�[�g�ݒ�R�}���h
	D3D12_VIEWPORT viewport;
	viewport.TopLeftX = leftTopPos_.x;
	viewport.TopLeftY = leftTopPos_.y;
	viewport.Width = size_.x;
	viewport.Height = size_.y;
	viewport.MinDepth = minDepth_;
	viewport.MaxDepth = maxDepth_;

	// �r���[�|�[�g�ݒ�R�}���h���A�R�}���h���X�g�ɐς�
	renderBase->GetCommandList()->RSSetViewports(1, &viewport);
}
