#pragma once
#include "NewEngine.h"

class Gate
{
private:
	std::unique_ptr<Object3D> mGateLeft;
	std::unique_ptr<Object3D> mGateRight;

public:
	Gate();
	void Init();
	void Update();
	void DrawModel();

public:
	void SetLeftTransform(const Transform& transform);
	void SetRightTransform(const Transform& transform);
	void SetLeftPos(const Vec3 pos);
	void SetRightPos(const Vec3 pos);
	void SetLeftRot(const Vec3 rot);
	void SetRightRot(const Vec3 rot);

};
