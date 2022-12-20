#include "Viewport.h"
#include "RenderBase.h"
#include "RenderWindow.h"
#include <memory>
using namespace std;

Viewport::Viewport() :
	leftTopPos(0), size(0), minDepth(0), maxDepth(0)
{
}

void Viewport::Update()
{
	RenderBase* renderBase = RenderBase::GetInstance();// .get();

	//----------------------- ビューポートの設定コマンド -----------------------//
	// ビューポート設定コマンド
	D3D12_VIEWPORT viewport;
	viewport.TopLeftX = leftTopPos.x;
	viewport.TopLeftY = leftTopPos.y;
	viewport.Width = size.x;
	viewport.Height = size.y;
	viewport.MinDepth = minDepth;
	viewport.MaxDepth = maxDepth;

	// ビューポート設定コマンドを、コマンドリストに積む
	renderBase->GetCommandList()->RSSetViewports(1, &viewport);
}
