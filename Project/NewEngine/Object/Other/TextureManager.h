#pragma once
#include "Texture.h"
#include "TextureAnimeiton.h"
#include "RenderTexture.h"
#include "Singleton.h"
#include <DirectXTex.h>
#include <mutex>
#include <unordered_map>

template<typename T> class Singleton;

// テクスチャーを管理するクラス
class TextureManager : public Singleton<TextureManager>
{
private:
	std::unordered_map<std::string, std::unique_ptr<Texture>> mTextureMap;				// 繝・け繧ｹ繝√Ε繝ｼ縺ｮ繝槭ャ繝・
	std::unordered_map<std::string, std::unique_ptr<Texture>> mMaterialTextureMap;		// 繝槭ユ繝ｪ繧｢繝ｫ繝・け繧ｹ繝√Ε繝ｼ縺ｮ繝槭ャ繝・
	std::unordered_map<std::string, std::unique_ptr<RenderTexture>> mRenderTextureMap;	// 繝ｬ繝ｳ繝繝ｼ繝・け繧ｹ繝√Ε繝ｼ縺ｮ繝槭ャ繝・

	std::vector<bool> mCheckSRVIndex; // index逡ｪ縺ｮSRV縺碁幕縺・※繧九°縺ｩ縺・°繧偵メ繧ｧ繝・け縺吶ｋ縺溘ａ

	std::mutex mMutex;	// 謗剃ｻ門宛蠕｡

private:
	void Init();

public:	// テクスチャー関連

	// テクスチャーを取得する関数
	static Texture* GetTexture(const std::string tag);

	// 1x1の色テクスチャーを生成する関数
	static Texture* CreateTexture(const Color color, const std::string tag);

	// テクスチャーをロードする関数
	static Texture* LoadTexture(const std::string filePath, const std::string tag);

	// mtlファイルのテクスチャーをロードする関数
	static Texture* LoadMaterialTexture(const std::string filePath, const std::string tag);

	// アンロード
	static void UnLoadTexture(const std::string tag);

public: // 繝ｬ繝ｳ繝繝ｼ繝・け繧ｹ繝√Ε繝ｼ髢｢騾｣

	// 繝ｬ繝ｳ繝繝ｼ繝・け繧ｹ繝√Ε繝ｼ縺ｮ蜿門ｾ・
	static RenderTexture* GetRenderTexture(const std::string tag);

	// 繝ｬ繝ｳ繝繝ｼ繝・け繧ｹ繝√Ε繝ｼ繧堤函謌舌＠繝槭ャ繝励↓譬ｼ邏阪☆繧・
	static RenderTexture* CreateRenderTexture(const Vec2 size, const uint32_t num, const std::string tag);

	// 繝ｬ繝ｳ繝繝ｼ繝・け繧ｹ繝√Ε繝ｼ縺ｮ繧｢繝ｳ繝ｭ繝ｼ繝蛾未謨ｰ
	static void UnLoadRenderTexture(const std::string tag);

public:
	// 繝槭ャ繝・
	static std::unordered_map<std::string, std::unique_ptr<Texture>>* GetTextureMap();
	static std::unordered_map<std::string, std::unique_ptr<Texture>>* GetMaterialTextureMap();
	static std::unordered_map<std::string, std::unique_ptr<RenderTexture>>* GetRenderTextureMap();

public:
	// 豺ｱ蠎ｦ繝・け繧ｹ繝√Ε繝ｼ繧堤函謌・
	static Texture* CreateDepthTexture();

public:	// 縺昴・莉悶・蜃ｦ逅・
	// 繝・け繧ｹ繝√Ε繝ｼ繝ｭ繝ｼ繝牙ｾ後・繧ｳ繝槭Φ繝峨Μ繧ｹ繝医・螳溯｡・
	static void ExcuteComandList();

private:
	// 繧ｷ繝ｳ繧ｰ繝ｫ繝医Φ
	friend Singleton<TextureManager>;
	TextureManager();
};
