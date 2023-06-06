#pragma once
#include "Texture.h"
#include "TextureAnimeiton.h"
#include "RenderTexture.h"
#include <DirectXTex.h>
#include <mutex>
#include <map>

class TextureManager
{
private:
	static UINT srvIncrementIndex;	// srv作成時にインクリメント用
	static Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> srvDescHeap;	// srv用ディスクリプタヒープ

	static std::map<std::string, std::unique_ptr<Texture>> textureMap;					// テクスチャーのマップ
	static std::map<std::string, std::unique_ptr<RenderTexture>> renderTextureMap;		// レンダーテクスチャーのマップ

	static std::mutex mtx;	// 排他制御

public:	// テクスチャー関連

	// テクスチャーの取得
	static Texture* GetTexture(std::string textureTag);

	// 色を指定してテクスチャを生成する
	static Texture CreateTexture(Color color);

	// 色を指定してテクスチャを生成しマップに格納する
	static Texture* CreateTexture(Color color, std::string textureTag);

	// ファイルパスを指定してテクスチャを生成する
	static Texture LoadTexture(std::string filePath);

	// ファイルパスを指定してテクスチャを生成しマップの格納する
	static Texture* LoadTexture(std::string filePath, std::string textureTag);

	// objファイルからロードしたテクスチャーをロードする専用関数
	static Texture LoadMaterialTexture(std::string filePath);

public: // レンダーテクスチャー関連

	// レンダーテクスチャーの取得
	static RenderTexture* GetRenderTexture(std::string textureTag);

	// レンダーテクスチャーを生成しマップに格納する
	static RenderTexture* CreateRenderTexture(Vec2 size, size_t num, std::string textureTag);

public:	// その他の処理
	// ディスクリプターヒープを作成する処理
	static void CreateDescriptorHeap();

	// SRVを作成する処理
	static void CreateSRV(Texture& texture, ID3D12Resource* buffer);

	// テクスチャーロード後のコマンドリストの実行
	static void ExcuteComandList();

public:
	// SRV用のディスクリプターヒープを取得
	static inline ID3D12DescriptorHeap* GetSrvDescHeap() { return srvDescHeap.Get(); }

};

