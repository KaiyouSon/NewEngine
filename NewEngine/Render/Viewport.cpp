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

	//----------------------- ビューポートの設定コマンド -----------------------//
	// ビューポート設定コマンド
	D3D12_VIEWPORT viewport;
	viewport.TopLeftX = leftTopPos_.x;
	viewport.TopLeftY = leftTopPos_.y;
	viewport.Width = size_.x;
	viewport.Height = size_.y;
	viewport.MinDepth = minDepth_;
	viewport.MaxDepth = maxDepth_;

	// ビューポート設定コマンドを、コマンドリストに積む
	renderBase->GetCommandList()->RSSetViewports(1, &viewport);
}
