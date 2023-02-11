#include "TitleScene.h"
#include "TextureManager.h"

void TitleScene::Init()
{
	Camera::current.pos = { 0,1,-15 };
	Camera::current.rot = { Radian(0),0,0 };

	//obj.model = Model("AttackEnemy", true);
	//obj.rot.y = Radian(180);

	//skyDome.model = Model("SkyDome");

	//sprite.texture = *TextureManager::GetTexture("White");
	sprite.texture = *TextureManager::GetTexture("pic");
	sprite.size = { 256,256 };
	sprite.pos = GetWindowHalfSize();


	//ID3D12GraphicsCommandList* iCommandList = RenderBase::GetInstance()->GetCommandList();

	//iCommandList->Close();
	//ID3D12CommandQueue* iCommandQueue = RenderBase::GetInstance()->GetCommandQueue();

	//ID3D12CommandList* list[] = { iCommandList };
	//iCommandQueue->ExecuteCommandLists(1, list);

	//// コマンドの実行完了を待つ
	//iCommandQueue->Signal(RenderBase::GetInstance()->GetFence(), ++sprite.texture.index);
	//if (RenderBase::GetInstance()->GetFence()->GetCompletedValue() != sprite.texture.index)
	//{
	//	HANDLE event = CreateEvent(nullptr, false, false, nullptr);
	//	RenderBase::GetInstance()->GetFence()->SetEventOnCompletion(sprite.texture.index, event);
	//	WaitForSingleObject(event, INFINITE);
	//	CloseHandle(event);
	//}

	//HRESULT result;
	//// キューをクリア
	//result = RenderBase::GetInstance()->GetCommandAllocator()->Reset();
	//assert(SUCCEEDED(result));
	//// 再びコマンドリストを貯める準備
	//result = iCommandList->Reset(RenderBase::GetInstance()->GetCommandAllocator(), nullptr);
	//assert(SUCCEEDED(result));
}

static bool isDraw = false;

void TitleScene::Update()
{
	//if (Key::GetKeyTrigger(DIK_SPACE))
	//{
	//	if (isDraw == false)
	//	{
	//		isDraw = true;
	//	}
	//	else
	//	{
	//		isDraw = false;
	//	}
	//}

	////obj.rot.y += Radian(1);
	//const float speed = 10;

	//sprite.pos.x += (Key::GetKey(DIK_RIGHT) - Key::GetKey(DIK_LEFT)) * speed;
	//sprite.pos.y += (Key::GetKey(DIK_DOWN) - Key::GetKey(DIK_UP)) * speed;
	//sprite.color.a += Key::GetKey(DIK_Q) - Key::GetKey(DIK_E);

	//obj.pos.x += (Key::GetKey(DIK_D) - Key::GetKey(DIK_A));
	//obj.pos.y += (Key::GetKey(DIK_W) - Key::GetKey(DIK_S));

	//if (Pad::GetButton(PadCodo::ButtonA))
	//{
	//	static int a = 0;
	//	a++;
	//}

	//obj.pos.x = +Pad::GetStick(PadCodo::LeftStick, 300).x / 1000;
	//obj.pos.y = -Pad::GetStick(PadCodo::LeftStick, 300).y / 1000;

	//sprite.pos.x += Pad::GetStick(PadCodo::LeftStick, 300, 1).x / 1000;
	//sprite.pos.y += Pad::GetStick(PadCodo::LeftStick, 300, 1).y / 1000;

	sprite.Update();

	/*	sprite.graphicsPipeline = GraphicsPipelineManager::GetGraphicsPipeline("RenderTexture");
		skyDome.Update();
		obj.Update()*/;
}

void TitleScene::DrawRenderTexture()
{
	/*renderTexture.PreDrawScene();
	if (isDraw == true)
	{
		skyDome.Draw();
	}
	obj.Draw();
	renderTexture.PostDrawScene();*/
}

void TitleScene::DrawBackSprite()
{
	sprite.Draw();
}

void TitleScene::DrawModel()
{
	//obj.Draw();
}

void TitleScene::DrawFrontSprite()
{
}

void TitleScene::DrawDebugGui()
{
}
