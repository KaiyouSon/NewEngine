#include "NewEngine.h"
#include "main2.h"
#include "InputManager.h"
#include "Util.h"
#include "ObjectManager.h"
#include "DataOperator.h"

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	DataOperator* dataOperator = DataOperator::GetInstance().get();
	FrameRate* frameRate = FrameRate::GetInstance().get();

	//SetBackGroundColor(25.5, 63.75, 127.5);	// 背景色
	SetBackGroundColor(0, 0, 0);	// 背景色
	NewEngineInit();	// エンジンの初期化
	dataOperator->LoadData();
	Load();			// ゲーム内のロード処理
	Initialize();	// ゲーム内の初期化処理

	frameRate->Initialize(60);

	// ゲームループ
	while (true)
	{
		////////////////////////////////////////////////////
		// ----------- ここから更新処理を記述 ----------- //
		////////////////////////////////////////////////////
		NewEngineUpda();	// エンジンの更新処理
		Update();			// ゲーム内の更新処理
		////////////////////////////////////////////////////
		// ---------- ここまでに更新処理を記述 ---------- //
		////////////////////////////////////////////////////
		NewEnginePreDraw();		// エンジン描画前処理
		////////////////////////////////////////////////////
		// ----------- ここから描画処理を記述 ----------- //
		////////////////////////////////////////////////////
		NewEneineDraw();		// エンジンの描画処理
		Draw3D();
		DrawLine();
		////////////////////////////////////////////////////
		// ---------- ここまでに描画処理を記述 ---------- //
		////////////////////////////////////////////////////
		NewEnginePostDraw();	// エンジン描画後処理

		// XボタンもしくはESCキーでゲームループを抜ける
		if (ProcessMessage() || Key::GetKey(DIK_ESCAPE))
		{
			dataOperator->SaveData();
			break;
		}

		frameRate->Update();
	}

	Destroy();	// インスタンスの破棄
	NewEngineEnd();	// エンジンのエンド処理

	// 正常終了
	return 0;
}