#include "ScissorRectangle.h"
#include "RenderBase.h"
#include "RenderWindow.h"

void ScissorRectangle::Update()
{
	RenderBase* renderBase = RenderBase::GetInstance();// .get();
	RenderWindow* renderWindow = RenderWindow::GetInstance().get();

	//------------------------ シザー矩形の設定コマンド ------------------------//
	// シザー矩形
	D3D12_RECT scissorRect{};
	scissorRect.left = 0; // 切り抜き座標左
	scissorRect.right = scissorRect.left + (LONG)renderWindow->GetWindowSize().x; // 切り抜き座標右
	scissorRect.top = 0; // 切り抜き座標上
	scissorRect.bottom = scissorRect.top + (LONG)renderWindow->GetWindowSize().y; // 切り抜き座標下
	// シザー矩形設定コマンドを、コマンドリストに積む
	renderBase->GetCommandList()->RSSetScissorRects(1, &scissorRect);
}
