#pragma once
#include "NewEngine.h"
#include "IFieldObject.h"

class Sun : public IFieldObject
{
private:
	std::unique_ptr<ParticleObject> mSun;
	std::unique_ptr<PointLight> mPointLight;

public:
	Sun();
	void Init()  override;
	void Update() override;
	void ExecuteCS() override;
	void Draw(const bool isDrawDepth) override;

public:
	void SetTransform(const Transform& transform);

public:
	Vec3 GetPos();
};

