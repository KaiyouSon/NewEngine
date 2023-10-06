#include "Viewport.h"
#include "RenderBase.h"
#include "RenderWindow.h"

Viewport::Viewport() :
	mLeftTopPos(0), mSize(0), mMinDepth(0), mMaxDepth(0)
{
}

void Viewport::Update()
{
	RenderBase* renderBase = RenderBase::GetInstance();// .get();

	//----------------------- 繝薙Η繝ｼ繝昴・繝医・險ｭ螳壹さ繝槭Φ繝・-----------------------//
	// 繝薙Η繝ｼ繝昴・繝郁ｨｭ螳壹さ繝槭Φ繝・
	D3D12_VIEWPORT viewport;
	viewport.TopLeftX = mLeftTopPos.x;
	viewport.TopLeftY = mLeftTopPos.y;
	viewport.Width = mSize.x;
	viewport.Height = mSize.y;
	viewport.MinDepth = mMinDepth;
	viewport.MaxDepth = mMaxDepth;

	// 繝薙Η繝ｼ繝昴・繝郁ｨｭ螳壹さ繝槭Φ繝峨ｒ縲√さ繝槭Φ繝峨Μ繧ｹ繝医↓遨阪・
	renderBase->GetCommandList()->RSSetViewports(1, &viewport);
}

// 繧ｻ繝・ち繝ｼ
void Viewport::SetViewport(
	const Vec2 leftTopPos, const Vec2 size,
	const float MinDepth, const float MaxDepth)
{
	mLeftTopPos = leftTopPos;
	mSize = size;

	mMinDepth = MinDepth;
	mMaxDepth = MaxDepth;
}

// 繧ｲ繝・ち繝ｼ
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
