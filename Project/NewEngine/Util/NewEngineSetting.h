#pragma once
#include "NewEngineEnum.h"
#include "Vec2.h"
#include "Color.h"
#include <d3d12.h>
#include <string>
#include <vector>
#include <filesystem>

// 前方宣言
class ShaderCompiler;

// エンジンの初期設定構造体
struct NewEngineSetting
{
	// ウィンドウタイトル
	std::string windowTitle;

	// ウィンドウサイズ
	Vec2 windowSize;

	// 背景色
	Color bgColor;

	// フレームレート
	float frameRate;
};

// インプットレイアウトの設定構造体
struct InputLayoutSetting
{
	std::string semanticName;
	DXGI_FORMAT format;
	uint32_t index;

	InputLayoutSetting();
	InputLayoutSetting(
		const std::string semanticName,
		const DXGI_FORMAT format,
		const uint32_t index = 0);
};

// シェーダーコンパイラーの設定構造体
struct ShaderCompilerSetting
{
	std::filesystem::path folderPath;
	std::string csFilePath;
	std::string vsFilePath;
	std::string gsFilePath;
	std::string psFilePath;
	std::vector<InputLayoutSetting> mInputLayoutSettings;
};

// ルートシグネーチャの設定構造体
struct RootSignatureSetting
{
	uint32_t maxCbvRootParameter;
	uint32_t maxSrvDescritorRange;
	uint32_t maxUavDescritorRange;
	RootSignatureSetting();
	RootSignatureSetting(
		const uint32_t maxCbvRootParameter,
		const uint32_t maxSrvDescritorRange,
		const uint32_t maxUavDescritorRange);
};

// グラフィックスパイプラインの設定構造体
struct GraphicsPipelineSetting
{
	// パイプライン生成用
	enum PipelineBlend
	{
		Alpha = 0b0001,
		Add = 0b0010,
		Sub = 0b0100,
		Inv = 0b1000,
	};

	// 塗りつぶし
	enum FillMode
	{
		Solid,
		Wireframe,
	};

	// 書き込むモード
	enum RenderTargetBlendMask
	{
		WriteNone = 0,
		WriteRed = 1,
		WriteGreen = 2,
		WriteBlue = 4,
		WriteAlpha = 8,
		WriteAll = ((WriteRed | WriteGreen) | WriteBlue) | WriteAlpha,
	};

	// パイプライン種類
	uint8_t pipelineBlend;

	// 塗りつぶしモード
	FillMode fillMode;

	// 書き込むモード
	RenderTargetBlendMask renderTargetBlendMask;

	// カリング
	CullMode cullMode;

	// トポロジータイプ
	TopologyType topologyType;

	// シェーダーコンパイラー
	std::string shaderCompilerTag;

	// 深度
	D3D12_DEPTH_STENCIL_DESC depthStencilDesc;

	// RTV数
	uint32_t rtvNum;

	// RootParamter設定構造体
	RootSignatureSetting rootSignatureSetting;

	GraphicsPipelineSetting();
};

// コンピュートパイプラインの設定構造体
struct ComputePipelineSetting
{
	// シェーダーコンパイラオブジェクト
	ShaderCompiler* shaderObject;

	// RootSignature設定
	RootSignatureSetting rootSignatureSetting;
};

// ディスクリプタヒープの設定構造体
struct DescriptorHeapSetting
{
	enum HeapType
	{
		None,
		CBV_SRV_UAV,
		RTV,
		DSV,
	};

	// 最大サイズ
	uint32_t maxSize;

	uint32_t startIndex;

	// ヒープの種類
	HeapType heapType;

	DescriptorHeapSetting();
};

// RenderTextureの設定構造体
struct RenderTextureSetting
{
	Vec2 texSize;
	Vec2 depthSize;
	uint32_t rtvNum;
	bool isUseDepthTexture;

	RenderTextureSetting(Vec2 texSize);
	RenderTextureSetting(Vec2 texSize, uint32_t rtvNum);
	RenderTextureSetting(Vec2 texSize, Vec2 depthSize, bool isUseDepthTexture);
	RenderTextureSetting(Vec2 texSize, Vec2 depthSize, uint32_t rtvNum, bool isUseDepthTexture);
};