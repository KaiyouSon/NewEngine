#include "Viewport.h"
#include "RenderBase.h"
#include "RenderWindow.h"

Viewport::Viewport() :
	mLeftTopPos(0), mSize(0), mMinDepth(0), mMaxDepth(0)
{
}

void Viewport::DrawCommands()
{
	RenderBase* renderBase = RenderBase::GetInstance();// .get();

	// 設定構造体
	D3D12_VIEWPORT viewport;
	viewport.TopLeftX = mLeftTopPos.x;
	viewport.TopLeftY = mLeftTopPos.y;
	viewport.Width = mSize.x;
	viewport.Height = mSize.y;
	viewport.MinDepth = mMinDepth;
	viewport.MaxDepth = mMaxDepth;

	// ビューポートセット
	renderBase->GetCommandList()->RSSetViewports(1, &viewport);
}

// セッター
void Viewport::SetViewport(
	const Vec2 leftTopPos, const Vec2 size,
	const float MinDepth, const float MaxDepth)
{
	mLeftTopPos = leftTopPos;
	mSize = size;

	mMinDepth = MinDepth;
	mMaxDepth = MaxDepth;
}

// ゲッター
Vec2 Viewport::GetLeftTopPos()
{
	return mLeftTopPos;
}
Vec2 Viewport::GetSize()
{
	return mSize;
}
float Viewport::GetMinDepth()
{
	return mMinDepth;
}
float Viewport::GetMaxDepth()
{
	return mMaxDepth;
}
