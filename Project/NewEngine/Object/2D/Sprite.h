#pragma once
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"
#include "Transform.h"
#include "Material.h"
#include "TextureManager.h"
#include "NewEngineEnum.h"
#include <vector>

#include "GameObject.h"

// 前方宣言
class TextureAnimation;

// スプライトのクラス
class Sprite : public GameObject
{
private:
	std::vector<VertexBufferData::VSprite> mVertices;
	std::unique_ptr<VertexBuffer<VertexBufferData::VSprite>> mVertexBuffer;
	Material* mMaterial;

private:
	// コンポーネント関連
	SpriteInfo* mInfo;

private: // マテリアル関連
	void MaterialInit();
	void MaterialTransfer();

private:
	void TransferVertexCoord();
	void TransferUVCoord(const Vec2 leftTopPos, const Vec2 rightDownPos);

private:
	void InitComponents();
	void DrawCommands();

public:
	Sprite();
	Sprite(const std::string& name);
	void Update() override;
	void ExecuteCS() override;
	void AppedToRenderer() override;
	void Draw(const std::string& layerTag, const BlendMode blendMode = BlendMode::Alpha);
	void Copy([[maybe_unused]] GameObject* gameObj) override {};

	template<typename T>
	void AddMaterial()
	{
		std::unique_ptr<IConstantBuffer> iConstatnBuffer = std::make_unique<T>();
		iConstatnBuffer->Create();
		mMaterial->constantBuffers.push_back(std::move(iConstatnBuffer));
	}

public: //セッター

	void SetTexture(Texture* texture, [[maybe_unused]] const bool isChangeSize = true);
	void SetTexture(const std::string& textureTag, [[maybe_unused]] const bool isChangeSize = true) override;

	// 描画範囲
	void SetTextureRect(const Vec2 leftTopPos, const Vec2 rightDownPos);

	//　サイズ
	void SetSize(const Vec2 size);

	// アンカーポイント
	void SetAnchorPoint(const Vec2 anchorPoint);

	// 画像反転
	void SetFlipType(const FlipType flipType);

	// グラフィックスパイプライン
	void SetGraphicsPipeline(GraphicsPipeline* graphicsPipeline);

	template<typename T>
	void SetTransferBuffer(const uint32_t bufferNum, const T& data)
	{
		uint32_t bNum = Min<uint32_t>(bufferNum, (uint32_t)mMaterial->constantBuffers.size());
		TransferDataToConstantBuffer(mMaterial->constantBuffers[bNum].get(), data);
	}

private:
	friend TextureAnimation;
};

