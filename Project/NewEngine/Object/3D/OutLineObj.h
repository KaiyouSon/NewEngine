#pragma once
#include "NewEngine.h"

// 譛霑台ｽｿ縺｣縺ｦ縺ｪ縺・◆繧√Μ繝輔ぃ繧ｯ繧ｿ繝ｪ繝ｳ繧ｰ縺ｯ蠕悟屓縺・
class OutLineObj
{
private:
	std::unique_ptr<ConstantBuffer<ConstantBufferData::CTransform3D>> mConstantBufferTransform;
	std::unique_ptr<ConstantBuffer<ConstantBufferData::CColor>> mConstantBufferColor;
	GraphicsPipeline* mGraphicsPipeline;
	Transform mTransform;
	Transform* mParent;

public:
	Object3D* obj;
	Color color;

public:
	OutLineObj();
	OutLineObj(const OutLineObj& other) = delete;

	void Update(Transform* parent = nullptr);
	void Draw();
};
