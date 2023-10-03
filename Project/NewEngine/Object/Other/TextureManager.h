#pragma once
#include "Texture.h"
#include "TextureAnimeiton.h"
#include "RenderTexture.h"
#include "Singleton.h"
#include <DirectXTex.h>
#include <mutex>
#include <unordered_map>

template<typename T> class Singleton;

class TextureManager : public Singleton<TextureManager>
{
private:
	std::unordered_map<std::string, std::unique_ptr<Texture>> mTextureMap;				// テクスチャーのマップ
	std::unordered_map<std::string, std::unique_ptr<Texture>> mMaterialTextureMap;		// マテリアルテクスチャーのマップ
	std::unordered_map<std::string, std::unique_ptr<RenderTexture>> mRenderTextureMap;	// レンダーテクスチャーのマップ

	std::vector<bool> mCheckSRVIndex; // index番のSRVが開いてるかどうかをチェックするため

	std::mutex mMutex;	// 排他制御

public:
	uint32_t mSrvIncrementIndex;	// srv作成時にインクリメント用

private:
	void Init();

public:	// テクスチャー関連

	// テクスチャーの取得
	static Texture* GetTexture(const std::string tag);

	// 色を指定してテクスチャを生成しマップに格納する
	static Texture* CreateTexture(const Color color, const std::string tag);

	// ファイルパスを指定してテクスチャを生成しマップの格納する
	static Texture* LoadTexture(const std::string filePath, const std::string tag);

	// objファイルからロードしたテクスチャーをロードする専用関数
	static Texture* LoadMaterialTexture(const std::string filePath, const std::string tag);

	// テクスチャーのアンロード関数
	static void UnLoadTexture(const std::string tag);

public: // レンダーテクスチャー関連

	// レンダーテクスチャーの取得
	static RenderTexture* GetRenderTexture(const std::string tag);

	// レンダーテクスチャーを生成しマップに格納する
	static RenderTexture* CreateRenderTexture(const Vec2 size, const uint32_t num, const std::string tag);

	// レンダーテクスチャーのアンロード関数
	static void UnLoadRenderTexture(const std::string tag);

public:
	// マップ
	static std::unordered_map<std::string, std::unique_ptr<Texture>>* GetTextureMap();
	static std::unordered_map<std::string, std::unique_ptr<Texture>>* GetMaterialTextureMap();
	static std::unordered_map<std::string, std::unique_ptr<RenderTexture>>* GetRenderTextureMap();

public:
	// 深度テクスチャーを生成
	static Texture* CreateDepthTexture(Vec2 size);

public:	// その他の処理
	// ディスクリプターヒープを作成する処理
	static void CreateDescriptorHeap();

	// テクスチャーロード後のコマンドリストの実行
	static void ExcuteComandList();

private:
	// シングルトン
	friend Singleton<TextureManager>;
	TextureManager();
};

