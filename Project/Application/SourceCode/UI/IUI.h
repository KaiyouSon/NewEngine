#pragma once

// UIのインターフェース
class IUI
{
protected:
	bool mIsActive;

public:
	virtual ~IUI() {}
	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;

public: // セッター
	void SetisActive(const bool isActive);

public:	// ゲッター
	bool GetisActive();
};