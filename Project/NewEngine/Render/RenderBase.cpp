#include "RenderBase.h"
#include "RenderWindow.h"
#include "Viewport.h"
#include "ScissorRectangle.h"
#include "TextureManager.h"
#include "ShaderObjectManager.h"
#include <cassert>
#include <string>
#include <d3dcompiler.h>

DirectX12WarningDisableBegin
#include <d3dx12.h>
DirectX12WarningDisableEnd

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"d3dcompiler.lib")
using namespace Microsoft::WRL;

float RenderBase::sClearColor[4] = { 0.1f,0.25f,0.5f,0.0f };

void RenderBase::Init()
{
	// 繝・ヰ繝・げ譎ゅ・縺ｿ螳溯｡・
	ProcessAtDebugBulid([]()
		{
			//繝・ヰ繝・げ繝ｬ繧､繝､繝ｼ繧偵が繝ｳ縺ｫ
			ComPtr<ID3D12Debug1> debugController;
			if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(debugController.GetAddressOf()))))
			{
				debugController->EnableDebugLayer();
				debugController->SetEnableGPUBasedValidation(false);
			}
		});

	DeviceInit();	// 繝・ヰ繧､繧ｹ縺ｮ蛻晄悄蛹・

	// 繝・ヰ繝・げ譎ゅ・縺ｿ螳溯｡・
	ProcessAtDebugBulid([]()
		{
			ComPtr<ID3D12InfoQueue> infoQueue;
			if (SUCCEEDED(RenderBase::GetInstance()->
				GetDevice()->QueryInterface(IID_PPV_ARGS(&infoQueue))))
			{
				infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, true);	// 繧・・縺・お繝ｩ繝ｼ荳譎ゅ↓豁｢縺ｾ繧・
				infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, true);		// 繧ｨ繝ｩ繝ｼ譎ゅ↓豁｢縺ｾ繧・
				infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, true);	// 繝ｯ繝ｼ繝九Φ繧ｰ譎ゅ↓豁｢縺ｾ繧・
			}

			//縲謚大宛縺吶ｋ繧ｨ繝ｩ繝ｼ
			D3D12_MESSAGE_ID denyIds[] = {
				D3D12_MESSAGE_ID_RESOURCE_BARRIER_MISMATCHING_COMMAND_LIST_TYPE
			};

			//縲謚大宛縺輔ｌ繧玖｡ｨ遉ｺ繝ｬ繝吶Ν
			D3D12_MESSAGE_SEVERITY severities[] = { D3D12_MESSAGE_SEVERITY_INFO };
			D3D12_INFO_QUEUE_FILTER filter{};
			filter.DenyList.NumIDs = _countof(denyIds);
			filter.DenyList.pIDList = denyIds;
			filter.DenyList.NumSeverities = _countof(severities);
			filter.DenyList.pSeverityList = severities;
			//縲謖・ｮ壹＠縺溘お繝ｩ繝ｼ縺ｮ陦ｨ遉ｺ繧呈椛蛻ｶ縺吶ｋ
			infoQueue->PushStorageFilter(&filter);
		});

	mRenderWindow = RenderWindow::GetInstance().get();
	mViewport = std::make_unique<Viewport>();
	mScissorRectangle = std::make_unique<ScissorRectangle>();

	mFenceValue = 0;

	DescriptorHeapInit();	// 繝・ぅ繧ｹ繧ｯ繝ｪ繝励ち繝ｼ繝偵・繝励・蛻晄悄蛹・
	CommandInit();			// 繧ｳ繝槭Φ繝蛾未騾｣縺ｮ蛻晄悄蛹・
	SwapChainInit();		// 繧ｹ繝ｯ繝・・繝√ぉ繝ｳ縺ｮ蛻晄悄蛹・
	FenceInit();			// 繝輔ぉ繝ｳ繧ｹ縺ｮ蛻晄悄蛹・
	DepthBufferInit();		// 豺ｱ蠎ｦ繝舌ャ繝輔ぃ縺ｮ蛻晄悄蛹・
	ShaderCompilerInit();	// 繧ｷ繧ｧ繝ｼ繝繝ｼ繧ｳ繝ｳ繝代う繝ｩ繝ｼ縺ｮ蛻晄悄蛹・
	GraphicsPipelineInit();	// 繧ｰ繝ｩ繝輔ぅ繝・け繧ｹ繝代う繝励Λ繧､繝ｳ縺ｮ蛻晄悄蛹・
	ComputePipelineInit();	// 繧ｳ繝ｳ繝斐Η繝ｼ繝医ヱ繧､繝励Λ繧､繝ｳ縺ｮ蛻晄悄蛹・
}
void RenderBase::PreDraw()
{
	//---------------------- 繝ｪ繧ｽ繝ｼ繧ｹ繝舌Μ繧｢縺ｮ螟画峩繧ｳ繝槭Φ繝・----------------------//
	// 繝舌ャ繧ｯ繝舌ャ繝輔ぃ縺ｮ逡ｪ蜿ｷ繧貞叙蠕・
	UINT bbIndex = mSwapChain->GetCurrentBackBufferIndex();
	// ・托ｼ弱Μ繧ｽ繝ｼ繧ｹ繝舌Μ繧｢縺ｧ譖ｸ縺崎ｾｼ縺ｿ蜿ｯ閭ｽ縺ｫ螟画峩
	mBarrierDesc.Transition.pResource = mBackBuffers[bbIndex]->GetBufferResource()->buffer.Get();	// 繝舌ャ繧ｯ繝舌ャ繝輔ぃ繧呈欠螳・
	mBarrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;	// 陦ｨ遉ｺ迥ｶ諷九°繧・
	mBarrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET; // 謠冗判迥ｶ諷九∈
	mCommandList->ResourceBarrier(1, &mBarrierDesc);

	//--------------------------- 謠冗判蜈域欠螳壹さ繝槭Φ繝・---------------------------//
	// ・抵ｼ取緒逕ｻ蜈医・螟画峩
	// 繝ｬ繝ｳ繝繝ｼ繧ｿ繝ｼ繧ｲ繝・ヨ繝薙Η繝ｼ縺ｮ繝上Φ繝峨Ν繧貞叙蠕・
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = mBackBuffers[bbIndex]->GetBufferResource()->rtvHandle.cpu;

	// 豺ｱ蠎ｦ繧ｹ繝・Φ繧ｷ繝ｫ繝薙Η繝ｼ逕ｨ繝・せ繧ｯ繝ｪ繝励ち繝偵・繝励・繝上Φ繝峨Ν繧貞叙蠕・
	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = mDepthBuffer->GetBufferResource()->dsvHandle.cpu;
	mCommandList->OMSetRenderTargets(1, &rtvHandle, false, &dsvHandle);

	// 逕ｻ髱｢繧ｯ繝ｪ繧｢ R G B A
	mCommandList->ClearRenderTargetView(rtvHandle, sClearColor, 0, nullptr);

	// 豺ｱ蠎ｦ繝舌ャ繝輔ぃ繧ｯ繝ｪ繧｢
	mCommandList->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

	// 繝薙Η繝ｼ繝昴・繝医・蜃ｦ逅・
	mViewport->SetViewport(
		{ 0,0 },
		{
			(float)mRenderWindow->GetWindowSize().x,
			(float)mRenderWindow->GetWindowSize().y
		});
	mViewport->Update();

	// 繧ｷ繧ｶ繝ｼ遏ｩ蠖｢縺ｮ蜃ｦ逅・
	mScissorRectangle->Update();
}
void RenderBase::PostDraw()
{
	HRESULT result;

	//---------------------- 繝ｪ繧ｽ繝ｼ繧ｹ繝舌Μ繧｢縺ｮ蠕ｩ蟶ｰ繧ｳ繝槭Φ繝・----------------------//
	// ・包ｼ弱Μ繧ｽ繝ｼ繧ｹ繝舌Μ繧｢繧呈綾縺・
	mBarrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET; // 謠冗判迥ｶ諷九°繧・
	mBarrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT; // 陦ｨ遉ｺ迥ｶ諷九∈
	mCommandList->ResourceBarrier(1, &mBarrierDesc);

	//-------------------------- 繧ｳ繝槭Φ繝峨・繝輔Λ繝・す繝･ --------------------------//
	// 蜻ｽ莉､縺ｮ繧ｯ繝ｭ繝ｼ繧ｺ
	result = mCommandList->Close();
	assert(SUCCEEDED(result));
	// 繧ｳ繝槭Φ繝峨Μ繧ｹ繝医・螳溯｡・
	ID3D12CommandList* mCommandLists[] = { mCommandList.Get() };
	mCommandQueue->ExecuteCommandLists(1, mCommandLists);

	// 逕ｻ髱｢縺ｫ陦ｨ遉ｺ縺吶ｋ繝舌ャ繝輔ぃ繧偵ヵ繝ｪ繝・・・郁｣剰｡ｨ縺ｮ蜈･譖ｿ縺茨ｼ・
	result = mSwapChain->Present(1, 0);
	assert(SUCCEEDED(result));

	// 繧ｳ繝槭Φ繝峨・螳溯｡悟ｮ御ｺ・ｒ蠕・▽
	mCommandQueue->Signal(mFence.Get(), ++mFenceValue);
	if (mFence.Get()->GetCompletedValue() != mFenceValue)
	{
		HANDLE event = CreateEvent(nullptr, false, false, nullptr);
		mFence->SetEventOnCompletion(mFenceValue, event);
		WaitForSingleObject(event, INFINITE);
		CloseHandle(event);
	}

	// 繧ｭ繝･繝ｼ繧偵け繝ｪ繧｢
	result = mCommandAllocator->Reset();
	assert(SUCCEEDED(result));
	// 蜀阪・繧ｳ繝槭Φ繝峨Μ繧ｹ繝医ｒ雋ｯ繧√ｋ貅門ｙ
	result = mCommandList.Get()->Reset(mCommandAllocator.Get(), nullptr);
	assert(SUCCEEDED(result));
}

void RenderBase::TransitionBufferState(
	BufferResource* bufferResource,
	const D3D12_RESOURCE_STATES currentState,
	const D3D12_RESOURCE_STATES targetState)
{
	CD3DX12_RESOURCE_BARRIER barrier =
		CD3DX12_RESOURCE_BARRIER::Transition(
			bufferResource->buffer.Get(),
			currentState,
			targetState,
			D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES);
	bufferResource->bufferState = targetState;
	mCommandList->ResourceBarrier(1, &barrier);
}

// --- 蛻晄悄蛹夜未騾｣ ------------------------------------------------------------ //
void RenderBase::DeviceInit()
{
	HRESULT result;

	// DXGI繝輔ぃ繧ｯ繝医Μ繝ｼ縺ｮ逕滓・
	result = CreateDXGIFactory(IID_PPV_ARGS(&mDxgiFactory));
	assert(SUCCEEDED(result));

	// 繧｢繝繝励ち繝ｼ縺ｮ蛻玲嫌逕ｨ
	std::vector<ComPtr<IDXGIAdapter4>> adapters;
	// 縺薙％縺ｫ迚ｹ螳壹・蜷榊燕繧呈戟縺､繧｢繝繝励ち繝ｼ繧ｪ繝悶ず繧ｧ繧ｯ繝医′蜈･繧・
	ComPtr<IDXGIAdapter4> tmpAdapter;

	// 繝代ヵ繧ｩ繝ｼ繝槭Φ繧ｹ縺碁ｫ倥＞繧ゅ・縺九ｉ鬆・↓縲∝・縺ｦ縺ｮ繧｢繝繝励ち繝ｼ繧貞・謖吶☆繧・
	for (UINT i = 0;
		mDxgiFactory->EnumAdapterByGpuPreference(i,
			DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE,
			IID_PPV_ARGS(&tmpAdapter)) != DXGI_ERROR_NOT_FOUND;
		i++)
	{
		// 蜍慕噪驟榊・縺ｫ霑ｽ蜉縺吶ｋ
		adapters.push_back(tmpAdapter);
	}

	// 螯･蠖薙↑繧｢繝繝励ち繧帝∈蛻･縺吶ｋ
	for (size_t i = 0; i < adapters.size(); i++)
	{
		DXGI_ADAPTER_DESC3 adapterDesc;
		// 繧｢繝繝励ち繝ｼ縺ｮ諠・ｱ繧貞叙蠕励☆繧・
		adapters[i]->GetDesc3(&adapterDesc);
		// 繧ｽ繝輔ヨ繧ｦ繧ｧ繧｢繝・ヰ繧､繧ｹ繧貞屓驕ｿ
		if (!(adapterDesc.Flags & DXGI_ADAPTER_FLAG3_SOFTWARE))
		{
			// 繝・ヰ繧､繧ｹ繧呈治逕ｨ縺励※繝ｫ繝ｼ繝励ｒ謚懊￠繧・
			tmpAdapter = adapters[i].Get();
			break;
		}
	}

	// 蟇ｾ蠢懊Ξ繝吶Ν縺ｮ驟榊・
	D3D_FEATURE_LEVEL levels[] =
	{
		D3D_FEATURE_LEVEL_12_1,
		D3D_FEATURE_LEVEL_12_0,
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
	};

	D3D_FEATURE_LEVEL featureLevel;
	for (size_t i = 0; i < _countof(levels); i++)
	{
		// 謗｡逕ｨ縺励◆繧｢繝繝励ち繝ｼ縺ｧ繝・ヰ繧､繧ｹ繧堤函謌・
		result = D3D12CreateDevice(tmpAdapter.Get(), levels[i],
			IID_PPV_ARGS(mDevice.GetAddressOf()));
		if (result == S_OK) {
			// 繝・ヰ繧､繧ｹ繧堤函謌舌〒縺阪◆譎らせ縺ｧ繝ｫ繝ｼ繝励ｒ謚懊￠繧・
			featureLevel = levels[i];
			break;
		}
	}

}
void RenderBase::DescriptorHeapInit()
{
	DescriptorHeapSetting setting;
	// SRV譬ｼ邏咲畑
	setting.maxSize = 2048;
	setting.startIndex = 1;
	setting.heapType = DescriptorHeapSetting::CBV_SRV_UAV;
	DescriptorHeapManager::Create(setting, "SRV");

	// RTV譬ｼ邏咲畑
	setting.maxSize = 64;
	setting.heapType = DescriptorHeapSetting::RTV;
	DescriptorHeapManager::Create(setting, "RTV");

	// DSV譬ｼ邏咲畑
	setting.maxSize = 64;
	setting.heapType = DescriptorHeapSetting::DSV;
	DescriptorHeapManager::Create(setting, "DSV");

	// SRV_UAV譬ｼ邏咲畑
	setting.maxSize = 512;
	setting.heapType = DescriptorHeapSetting::CBV_SRV_UAV;
	DescriptorHeapManager::Create(setting, "SRV_UAV");
}
void RenderBase::CommandInit()
{
	HRESULT result;

	// 繧ｳ繝槭Φ繝峨い繝ｭ繧ｱ繝ｼ繧ｿ繧堤函謌・
	result = mDevice->CreateCommandAllocator
	(
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		IID_PPV_ARGS(&mCommandAllocator)
	);
	assert(SUCCEEDED(result));

	// 繧ｳ繝槭Φ繝峨Μ繧ｹ繝医ｒ逕滓・
	result = mDevice->CreateCommandList
	(
		0,
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		mCommandAllocator.Get(), nullptr,
		IID_PPV_ARGS(&mCommandList)
	);
	assert(SUCCEEDED(result));

	//繧ｳ繝槭Φ繝峨く繝･繝ｼ縺ｮ險ｭ螳・
	D3D12_COMMAND_QUEUE_DESC mCommandQueueDesc{};
	//繧ｳ繝槭Φ繝峨く繝･繝ｼ繧堤函謌・
	result = mDevice.Get()->CreateCommandQueue(&mCommandQueueDesc, IID_PPV_ARGS(&mCommandQueue));
	assert(SUCCEEDED(result));
}
void RenderBase::SwapChainInit()
{
	HRESULT result;

	mBackBuffers[0] = std::make_unique<RenderTarget>();
	mBackBuffers[1] = std::make_unique<RenderTarget>();

	// 繝ｪ繧ｽ繝ｼ繧ｹ縺ｮ險ｭ螳・
	DXGI_SWAP_CHAIN_DESC1 mSwapChainDesc{};
	mSwapChainDesc.Width = (UINT)mRenderWindow->GetWindowSize().x;
	mSwapChainDesc.Height = (UINT)mRenderWindow->GetWindowSize().y;
	mSwapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;				 // 濶ｲ諠・ｱ縺ｮ譖ｸ蠑・
	mSwapChainDesc.SampleDesc.Count = 1;							 // 繝槭Ν繝√し繝ｳ繝励Ν縺励↑縺・
	mSwapChainDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER;			 // 繝舌ャ繧ｯ繝舌ャ繝輔ぃ逕ｨ
	mSwapChainDesc.BufferCount = 2;									 // 繝舌ャ繝輔ぃ謨ｰ繧抵ｼ偵▽縺ｫ險ｭ螳・
	mSwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;		 // 繝輔Μ繝・・蠕後・遐ｴ譽・
	mSwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	// 繧ｹ繝ｯ繝・・繝√ぉ繝ｼ繝ｳ縺ｮ逕滓・
	ComPtr<IDXGISwapChain1> mSwapChain1;
	result = mDxgiFactory->CreateSwapChainForHwnd
	(
		mCommandQueue.Get(),
		mRenderWindow->GetHwnd(),
		&mSwapChainDesc,
		nullptr,
		nullptr,
		&mSwapChain1
	);
	mSwapChain1.As(&mSwapChain);
	assert(SUCCEEDED(result));

	// 繝ｬ繝ｳ繝繝ｼ繧ｿ繝ｼ繧ｲ繝・ヨ繝薙Η繝ｼ縺ｮ險ｭ螳・
	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};
	// 繧ｷ繧ｧ繝ｼ繝繝ｼ縺ｮ險育ｮ礼ｵ先棡繧担RGB縺ｫ螟画鋤縺励※譖ｸ縺崎ｾｼ繧
	rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;

	// 繧ｹ繝ｯ繝・・繝√ぉ繝ｼ繝ｳ縺ｮ蜈ｨ縺ｦ縺ｮ繝舌ャ繝輔ぃ縺ｫ縺､縺・※蜃ｦ逅・☆繧・
	for (size_t i = 0; i < mBackBuffers.size(); i++)
	{
		// 繧ｹ繝ｯ繝・・繝√ぉ繝ｼ繝ｳ縺九ｉ繝舌ャ繝輔ぃ繧貞叙蠕・
		mSwapChain->GetBuffer((UINT)i, IID_PPV_ARGS(mBackBuffers[i]->GetBufferResource()->buffer.GetAddressOf()));

		// RTV菴懈・
		DescriptorHeapManager::GetDescriptorHeap("RTV")->CreateRTV(mBackBuffers[i]->GetBufferResource());
	}
}
void RenderBase::FenceInit()
{
	HRESULT result;

	// 繝輔ぉ繝ｳ繧ｹ縺ｮ逕滓・
	result = mDevice->CreateFence(
		mFenceValue, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(mFence.GetAddressOf()));
}
void RenderBase::DepthBufferInit()
{
	mDepthBuffer = std::make_unique<DepthBuffer>();
	mDepthBuffer->Create();
	DescriptorHeapManager::GetDescriptorHeap("DSV")->CreateDSV(mDepthBuffer->GetBufferResource());
}
void RenderBase::ShaderCompilerInit()
{
	std::string path1 = "NewEngine/Shader/";
	std::string path2 = "Application/Shader/";

	// Object3D逕ｨ繧ｷ繧ｧ繝ｼ繝繝ｼ
	ShaderObjectManager::Create("Object3D");
	ShaderObjectManager::GetShaderObject("Object3D")->AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	ShaderObjectManager::GetShaderObject("Object3D")->AddInputLayout("NORMAL", DXGI_FORMAT_R32G32B32_FLOAT);
	ShaderObjectManager::GetShaderObject("Object3D")->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	ShaderObjectManager::GetShaderObject("Object3D")->CompileVertexShader(path1 + "Object3DVS.hlsl", "main");
	ShaderObjectManager::GetShaderObject("Object3D")->CompilePixelShader(path1 + "Object3DPS.hlsl", "main");

	// Fbx繝｢繝・Ν逕ｨ繧ｷ繧ｧ繝ｼ繝繝ｼ
	ShaderObjectManager::Create("FbxModel");
	ShaderObjectManager::GetShaderObject("FbxModel")->AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	ShaderObjectManager::GetShaderObject("FbxModel")->AddInputLayout("NORMAL", DXGI_FORMAT_R32G32B32_FLOAT);
	ShaderObjectManager::GetShaderObject("FbxModel")->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	ShaderObjectManager::GetShaderObject("FbxModel")->AddInputLayout("BONEINDICES", DXGI_FORMAT_R32G32B32A32_UINT);
	ShaderObjectManager::GetShaderObject("FbxModel")->AddInputLayout("BONEWEIGHTS", DXGI_FORMAT_R32G32B32A32_FLOAT);
	ShaderObjectManager::GetShaderObject("FbxModel")->CompileVertexShader(path1 + "FbxModelVS.hlsl", "main");
	ShaderObjectManager::GetShaderObject("FbxModel")->CompilePixelShader(path1 + "FbxModelPS.hlsl", "main");

	// 繧ｹ繝励Λ繧､繝育畑繧ｷ繧ｧ繝ｼ繝繝ｼ
	ShaderObjectManager::Create("Sprite");
	ShaderObjectManager::GetShaderObject("Sprite")->AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	ShaderObjectManager::GetShaderObject("Sprite")->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	ShaderObjectManager::GetShaderObject("Sprite")->CompileVertexShader(path1 + "SpriteVS.hlsl", "main");
	ShaderObjectManager::GetShaderObject("Sprite")->CompilePixelShader(path1 + "SpritePS.hlsl", "main");

	// 蜀・ご繝ｼ繧ｸ繧ｹ繝励Λ繧､繝育畑繧ｷ繧ｧ繝ｼ繝繝ｼ
	ShaderObjectManager::Create("CircleGaugeSprite");
	ShaderObjectManager::GetShaderObject("CircleGaugeSprite")->AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	ShaderObjectManager::GetShaderObject("CircleGaugeSprite")->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	ShaderObjectManager::GetShaderObject("CircleGaugeSprite")->CompileVertexShader(path1 + "CircleGaugeSpriteVS.hlsl", "main");
	ShaderObjectManager::GetShaderObject("CircleGaugeSprite")->CompilePixelShader(path1 + "CircleGaugeSpritePS.hlsl", "main");

	// 繝ｬ繝ｳ繝繝ｼ繝・け繧ｹ繝√Ε繝ｼ縺ｮ繧ｷ繧ｧ繝ｼ繝繝ｼ
	ShaderObjectManager::Create("RenderTexture");
	ShaderObjectManager::GetShaderObject("RenderTexture")->AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	ShaderObjectManager::GetShaderObject("RenderTexture")->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	ShaderObjectManager::GetShaderObject("RenderTexture")->CompileVertexShader(path1 + "RenderTextureVS.hlsl", "main");
	ShaderObjectManager::GetShaderObject("RenderTexture")->CompilePixelShader(path1 + "RenderTexturePS.hlsl", "main");

	// 繧ｷ繝ｫ繧ｨ繝・ヨ逕ｨ繧ｷ繧ｧ繝ｼ繝繝ｼ
	ShaderObjectManager::Create("Silhouette");
	ShaderObjectManager::GetShaderObject("Silhouette")->AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	ShaderObjectManager::GetShaderObject("Silhouette")->AddInputLayout("NORMAL", DXGI_FORMAT_R32G32B32_FLOAT);
	ShaderObjectManager::GetShaderObject("Silhouette")->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	ShaderObjectManager::GetShaderObject("Silhouette")->CompileVertexShader(path1 + "SilhouetteVS.hlsl", "main");
	ShaderObjectManager::GetShaderObject("Silhouette")->CompilePixelShader(path1 + "SilhouettePS.hlsl", "main");

	// 繧｢繧ｦ繝医Λ繧､繝ｳObject逕ｨ繧ｷ繧ｧ繝ｼ繝繝ｼ
	ShaderObjectManager::Create("Outline");
	ShaderObjectManager::GetShaderObject("Outline")->AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	ShaderObjectManager::GetShaderObject("Outline")->AddInputLayout("NORMAL", DXGI_FORMAT_R32G32B32_FLOAT);
	ShaderObjectManager::GetShaderObject("Outline")->CompileVertexShader(path1 + "OutLineVS.hlsl", "main");
	ShaderObjectManager::GetShaderObject("Outline")->CompilePixelShader(path1 + "OutLinePS.hlsl", "main");

	// 繝医ぇ繝ｼ繝ｳ繝ｬ繝ｳ繝繝ｼ繝ｪ繝ｳ繧ｰ逕ｨ
	ShaderObjectManager::Create("ToonRendering");
	ShaderObjectManager::GetShaderObject("ToonRendering")->AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	ShaderObjectManager::GetShaderObject("ToonRendering")->AddInputLayout("NORMAL", DXGI_FORMAT_R32G32B32_FLOAT);
	ShaderObjectManager::GetShaderObject("ToonRendering")->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	ShaderObjectManager::GetShaderObject("ToonRendering")->CompileVertexShader(path1 + "ToonRenderVS.hlsl", "main");
	ShaderObjectManager::GetShaderObject("ToonRendering")->CompilePixelShader(path1 + "ToonRenderPS.hlsl", "main");

	// 繝ｩ繧､繝ｳ逕ｨ
	ShaderObjectManager::Create("Line");
	ShaderObjectManager::GetShaderObject("Line")->AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	ShaderObjectManager::GetShaderObject("Line")->CompileVertexShader(path1 + "LineVS.hlsl", "main");
	ShaderObjectManager::GetShaderObject("Line")->CompilePixelShader(path1 + "LinePS.hlsl", "main");

	// 繧ｨ繝溘ャ繧ｿ繝ｼ逕ｨ
	ShaderObjectManager::Create("Emitter");
	ShaderObjectManager::GetShaderObject("Emitter")->AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);	// 蠎ｧ讓・
	ShaderObjectManager::GetShaderObject("Emitter")->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);		// 繧ｹ繧ｱ繝ｼ繝ｫ
	ShaderObjectManager::GetShaderObject("Emitter")->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32_FLOAT, 1);		// 蝗櫁ｻ｢
	ShaderObjectManager::GetShaderObject("Emitter")->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32_FLOAT, 2);		// 霈昴″蠎ｦ
	ShaderObjectManager::GetShaderObject("Emitter")->AddInputLayout("COLOR", DXGI_FORMAT_R32G32B32A32_FLOAT);	// 濶ｲ
	ShaderObjectManager::GetShaderObject("Emitter")->CompileComputeShader(path1 + "EmitterCS.hlsl", "main");
	ShaderObjectManager::GetShaderObject("Emitter")->CompileVertexShader(path1 + "EmitterVS.hlsl", "main");
	ShaderObjectManager::GetShaderObject("Emitter")->CompileGeometryShader(path1 + "EmitterGS.hlsl", "main");
	ShaderObjectManager::GetShaderObject("Emitter")->CompilePixelShader(path1 + "EmitterPS.hlsl", "main");

	// GPU繧ｨ繝溘ャ繧ｿ繝ｼ逕ｨ
	ShaderObjectManager::Create("GPUEmitter");
	ShaderObjectManager::GetShaderObject("GPUEmitter")->CompileComputeShader(path2 + "RespawnPointEffectCS.hlsl", "main");
	ShaderObjectManager::GetShaderObject("GPUEmitter")->CompileVertexShader(path2 + "RespawnPointEffectVS.hlsl", "main");
	ShaderObjectManager::GetShaderObject("GPUEmitter")->CompileGeometryShader(path1 + "EmitterGS.hlsl", "main");
	ShaderObjectManager::GetShaderObject("GPUEmitter")->CompilePixelShader(path1 + "EmitterPS.hlsl", "main");

	// ColliderObject逕ｨ繧ｷ繧ｧ繝ｼ繝繝ｼ
	ShaderObjectManager::Create("ColliderObject");
	ShaderObjectManager::GetShaderObject("ColliderObject")->AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	ShaderObjectManager::GetShaderObject("ColliderObject")->AddInputLayout("NORMAL", DXGI_FORMAT_R32G32B32_FLOAT);
	ShaderObjectManager::GetShaderObject("ColliderObject")->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	ShaderObjectManager::GetShaderObject("ColliderObject")->CompileVertexShader(path1 + "ColliderObjectVS.hlsl", "main");
	ShaderObjectManager::GetShaderObject("ColliderObject")->CompilePixelShader(path1 + "ColliderObjectPS.hlsl", "main");
}
void RenderBase::GraphicsPipelineInit()
{
	D3D12_DEPTH_STENCIL_DESC  depthStencilDesc1{};
	depthStencilDesc1.DepthEnable = true; // 豺ｱ蠎ｦ繝・せ繝医ｒ陦後≧
	depthStencilDesc1.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;	// 譖ｸ縺崎ｾｼ縺ｿ險ｱ蜿ｯ
	depthStencilDesc1.DepthFunc = D3D12_COMPARISON_FUNC_LESS;	// 蟆上＆縺・⊇縺・ｒ謗｡逕ｨ

	D3D12_DEPTH_STENCIL_DESC  depthStencilDesc2{};
	depthStencilDesc2.DepthEnable = false; // 豺ｱ蠎ｦ繝・せ繝医ｒ陦後ｏ縺ｪ縺・

	D3D12_DEPTH_STENCIL_DESC  depthStencilDesc3{};
	depthStencilDesc3.DepthEnable = true; // 豺ｱ蠎ｦ繝・せ繝医ｒ陦後≧
	depthStencilDesc3.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;	// 譖ｸ縺崎ｾｼ縺ｿ荳榊庄
	depthStencilDesc3.DepthFunc = D3D12_COMPARISON_FUNC_GREATER;	// 螟ｧ縺阪＞縺ｻ縺・ｒ謗｡逕ｨ

	D3D12_DEPTH_STENCIL_DESC  depthStencilDesc4{};
	depthStencilDesc4.DepthEnable = true; // 豺ｱ蠎ｦ繝・せ繝医ｒ陦後≧
	depthStencilDesc4.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;	// 譖ｸ縺崎ｾｼ縺ｿ險ｱ蜿ｯ
	depthStencilDesc4.DepthFunc = D3D12_COMPARISON_FUNC_LESS;	// 蟆上＆縺・⊇縺・ｒ謗｡逕ｨ

	GraphicsPipelineSetting setting;

	// 3D繧ｪ繝悶ず繧ｧ繧ｯ繝育畑
	setting.pipelineBlend = GraphicsPipelineSetting::Alpha;
	setting.shaderObject = ShaderObjectManager::GetShaderObject("Object3D");
	setting.cullMode = CullMode::Back;
	setting.topologyType = TopologyType::TriangleList;
	setting.depthStencilDesc = depthStencilDesc1;
	setting.rtvNum = 2;
	setting.rootSignatureSetting.maxCbvRootParameter = 8;
	setting.rootSignatureSetting.maxSrvDescritorRange = 3;
	PipelineManager::CreateGraphicsPipeline(setting, "Object3D");

	// FBX繝｢繝・Ν逕ｨ
	setting.pipelineBlend = GraphicsPipelineSetting::Alpha;
	setting.shaderObject = ShaderObjectManager::GetShaderObject("FbxModel");
	setting.cullMode = CullMode::Back;
	setting.topologyType = TopologyType::TriangleList;
	setting.depthStencilDesc = depthStencilDesc1;
	setting.rtvNum = 1;
	setting.rootSignatureSetting.maxCbvRootParameter = 7;
	setting.rootSignatureSetting.maxSrvDescritorRange = 2;
	PipelineManager::CreateGraphicsPipeline(setting, "FbxModel");

	// 繧ｹ繝励Λ繧､繝育畑
	setting.pipelineBlend = GraphicsPipelineSetting::Alpha;
	setting.shaderObject = ShaderObjectManager::GetShaderObject("Sprite");
	setting.cullMode = CullMode::None;
	setting.topologyType = TopologyType::TriangleStrip;
	setting.depthStencilDesc = depthStencilDesc2;
	setting.rtvNum = 2;
	setting.rootSignatureSetting.maxCbvRootParameter = 3;
	setting.rootSignatureSetting.maxSrvDescritorRange = 1;
	PipelineManager::CreateGraphicsPipeline(setting, "Sprite");

	// 蜀・ｽ｢繧ｲ繝ｼ繧ｸ繧ｹ繝励Λ繧､繝育畑
	setting.pipelineBlend = GraphicsPipelineSetting::Alpha;
	setting.shaderObject = ShaderObjectManager::GetShaderObject("CircleGaugeSprite");
	setting.cullMode = CullMode::None;
	setting.topologyType = TopologyType::TriangleStrip;
	setting.depthStencilDesc = depthStencilDesc2;
	setting.rtvNum = 1;
	setting.rootSignatureSetting.maxCbvRootParameter = 3;
	setting.rootSignatureSetting.maxSrvDescritorRange = 1;
	PipelineManager::CreateGraphicsPipeline(setting, "CircleGaugeSprite");

	// 繝ｬ繝ｳ繝繝ｼ繝・け繧ｹ繝√Ε逕ｨ
	setting.pipelineBlend = GraphicsPipelineSetting::Alpha;
	setting.shaderObject = ShaderObjectManager::GetShaderObject("RenderTexture");
	setting.cullMode = CullMode::None;
	setting.topologyType = TopologyType::TriangleStrip;
	setting.depthStencilDesc = depthStencilDesc2;
	setting.rtvNum = 2;
	setting.rootSignatureSetting.maxCbvRootParameter = 2;
	setting.rootSignatureSetting.maxSrvDescritorRange = 2;
	PipelineManager::CreateGraphicsPipeline(setting, "RenderTexture");

	// 繧ｷ繝ｫ繧ｨ繝・ヨ逕ｨ
	setting.pipelineBlend = GraphicsPipelineSetting::Alpha;
	setting.shaderObject = ShaderObjectManager::GetShaderObject("Silhouette");
	setting.cullMode = CullMode::Back;
	setting.topologyType = TopologyType::TriangleList;
	setting.depthStencilDesc = depthStencilDesc3;
	setting.rtvNum = 1;
	setting.rootSignatureSetting.maxCbvRootParameter = 7;
	setting.rootSignatureSetting.maxSrvDescritorRange = 2;
	PipelineManager::CreateGraphicsPipeline(setting, "Silhouette");

	// 繧｢繧ｦ繝医Λ繧､繝ｳ逕ｨ
	setting.pipelineBlend = GraphicsPipelineSetting::Alpha;
	setting.shaderObject = ShaderObjectManager::GetShaderObject("Outline");
	setting.cullMode = CullMode::Front;
	setting.topologyType = TopologyType::TriangleList;
	setting.depthStencilDesc = depthStencilDesc4;
	setting.rtvNum = 1;
	setting.rootSignatureSetting.maxCbvRootParameter = 7;
	setting.rootSignatureSetting.maxSrvDescritorRange = 2;
	PipelineManager::CreateGraphicsPipeline(setting, "Outline");

	// 繝医ぇ繝ｼ繝ｳ繝ｬ繝ｳ繝繝ｪ繝ｳ繧ｰ逕ｨ
	setting.pipelineBlend = GraphicsPipelineSetting::Alpha;
	setting.shaderObject = ShaderObjectManager::GetShaderObject("ToonRendering");
	setting.cullMode = CullMode::Back;
	setting.topologyType = TopologyType::TriangleList;
	setting.depthStencilDesc = depthStencilDesc4;
	setting.rtvNum = 1;
	setting.rootSignatureSetting.maxCbvRootParameter = 7;
	setting.rootSignatureSetting.maxSrvDescritorRange = 2;
	PipelineManager::CreateGraphicsPipeline(setting, "ToonRendering");

	// 繝ｩ繧､繝ｳ逕ｨ
	setting.pipelineBlend = GraphicsPipelineSetting::Alpha;
	setting.shaderObject = ShaderObjectManager::GetShaderObject("Line");
	setting.cullMode = CullMode::None;
	setting.topologyType = TopologyType::LineStrip;
	setting.depthStencilDesc = depthStencilDesc1;
	setting.rtvNum = 1;
	setting.rootSignatureSetting.maxCbvRootParameter = 2;
	setting.rootSignatureSetting.maxSrvDescritorRange = 0;
	PipelineManager::CreateGraphicsPipeline(setting, "Line");

	// 繧ｨ繝溘ャ繧ｿ繝ｼ逕ｨ
	setting.pipelineBlend = GraphicsPipelineSetting::Alpha;
	setting.shaderObject = ShaderObjectManager::GetShaderObject("Emitter");
	setting.cullMode = CullMode::None;
	setting.topologyType = TopologyType::Point;
	setting.depthStencilDesc = depthStencilDesc1;
	setting.rtvNum = 1;
	setting.rootSignatureSetting.maxCbvRootParameter = 3;
	setting.rootSignatureSetting.maxSrvDescritorRange = 2;
	PipelineManager::CreateGraphicsPipeline(setting, "Emitter");

	// GPU繧ｨ繝溘ャ繧ｿ繝ｼ逕ｨ
	setting.pipelineBlend = GraphicsPipelineSetting::Alpha;
	setting.shaderObject = ShaderObjectManager::GetShaderObject("GPUEmitter");
	setting.cullMode = CullMode::None;
	setting.topologyType = TopologyType::Point;
	setting.depthStencilDesc = depthStencilDesc1;
	setting.rtvNum = 1;
	setting.rootSignatureSetting.maxCbvRootParameter = 3;
	setting.rootSignatureSetting.maxSrvDescritorRange = 2;
	PipelineManager::CreateGraphicsPipeline(setting, "GPUEmitter");

	// ColliderObject逕ｨ
	setting.pipelineBlend = GraphicsPipelineSetting::Alpha;
	setting.shaderObject = ShaderObjectManager::GetShaderObject("ColliderObject");
	setting.cullMode = CullMode::None;
	setting.fillMode = GraphicsPipelineSetting::Wireframe;
	setting.topologyType = TopologyType::TriangleList;
	setting.depthStencilDesc = depthStencilDesc1;
	setting.rtvNum = 1;
	setting.rootSignatureSetting.maxCbvRootParameter = 2;
	setting.rootSignatureSetting.maxSrvDescritorRange = 0;
	PipelineManager::CreateGraphicsPipeline(setting, "ColliderObject");
}
void RenderBase::ComputePipelineInit()
{
	ComputePipelineSetting setting;
	setting.shaderObject = ShaderObjectManager::GetShaderObject("GPUEmitter");
	setting.rootSignatureSetting.maxCbvRootParameter = 0;
	setting.rootSignatureSetting.maxSrvDescritorRange = 0;
	setting.rootSignatureSetting.maxUavDescritorRange = 1;
	PipelineManager::CreateComputePipeline(setting, "GPUEmitter");
}

// --- 繧ｲ繝・ち繝ｼ -------------------------------------------------------------- //
ID3D12Device* RenderBase::GetDevice() const
{
	return mDevice.Get();
}
ID3D12GraphicsCommandList* RenderBase::GetCommandList() const
{
	return mCommandList.Get();
}
ID3D12CommandQueue* RenderBase::GetCommandQueue() const
{
	return mCommandQueue.Get();
}
ID3D12CommandAllocator* RenderBase::GetCommandAllocator() const
{
	return mCommandAllocator.Get();
}
ID3D12Fence* RenderBase::GetFence() const
{
	return mFence.Get();
}
DepthBuffer* RenderBase::GetDepthBuffer() const
{
	return mDepthBuffer.get();
}
Viewport* RenderBase::GetViewport() const
{
	return mViewport.get();
}
UINT64 RenderBase::GetFenceValue() const
{
	return mFenceValue;
}
