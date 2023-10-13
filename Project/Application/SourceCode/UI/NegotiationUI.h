#pragma once
#include "NewEngine.h"
#include "IUI.h"

// 前方宣言
class UIManager;

// 交渉UIのクラス
class NegotiationUI : public IUI
{
public:
	// 交渉タイプ
	enum class TextType
	{
		ReadMessageText,
		RestInLightText,
		OpenText,
	};

private:
	UIManager* mUiManager;
	std::unique_ptr<Sprite> mBackFrame;
	std::unique_ptr<Sprite> mButton;
	std::unique_ptr<Sprite> mText;
	std::unique_ptr<Sprite> mColon;

	float mAlpha;

	TextType mType;

private:
	void AlphaUpdate();

public:
	NegotiationUI();
	void Init() override;
	void Update() override;
	void Draw() override;

public:	// セッター
	void SetTextType(const TextType type);

public: // ゲッター
	TextType GetTextType();
};

