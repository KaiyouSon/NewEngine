#include "Transform.h"
#include "MathUtil.h"

Transform::Transform() :
	pos(0, 0, 0), scale(1, 1, 1), rot(0, 0, 0), parent(nullptr),
	mWorldMat(Mat4::Identity()), mScaleMat(Mat4::Identity()),
	mRotMat(Mat4::Identity()), mTransMat(Mat4::Identity())
{
	mComponentInfo.type = ComponentType::Transform;
}

Transform::Transform(GameObject* gameObj) :
	Component(gameObj),
	pos(0, 0, 0), scale(1, 1, 1), rot(0, 0, 0), parent(nullptr),
	mWorldMat(Mat4::Identity()), mScaleMat(Mat4::Identity()),
	mRotMat(Mat4::Identity()), mTransMat(Mat4::Identity())
{
}

Transform::Transform(const Vec3 pos, const Vec3 scale, const Vec3 rot) :
	pos(pos), scale(scale), rot(rot), parent(nullptr),
	mWorldMat(Mat4::Identity()), mScaleMat(Mat4::Identity()),
	mRotMat(Mat4::Identity()), mTransMat(Mat4::Identity())
{
	mComponentInfo.type = ComponentType::Transform;
}

void Transform::Update()
{
	mScaleMat = ConvertScalingMat(scale); // スケーリング行列
	mRotMat = Mat4::Identity();
	mRotMat *= ConvertRotationZAxisMat(rot.z); // Z軸回転行列
	mRotMat *= ConvertRotationXAxisMat(rot.x); // X軸回転行列
	mRotMat *= ConvertRotationYAxisMat(rot.y); // Y軸回転行列
	mTransMat = ConvertTranslationMat(pos);    // 位置行列

	// ビルボード行列の計算
	mBillboard.CalculateBillboardMat();

	// ワールド行列の計算
	mWorldMat = Mat4::Identity();

	if (mBillboard.GetBillboardType() != BillboardType::None)
	{
		mWorldMat *= mBillboard.GetMat();
	}

	mWorldMat *= mScaleMat;
	mWorldMat *= mRotMat;
	mWorldMat *= mTransMat;

	// 親行列との計算
	if (parent)
	{
		mWorldMat *= parent->mWorldMat;
	}
}

void Transform::CopyComponent(Component* component)
{
	auto castComponent = dynamic_cast<Transform*>(component);

	mGameObj->pos = castComponent->pos;;
	mGameObj->rot = castComponent->rot;
	mGameObj->scale = castComponent->scale;
}

nlohmann::json Transform::SaveToJson()
{
	nlohmann::json transformData;
	transformData["transform"] = mComponentInfo.SaveToJson();
	transformData["transform"]["pos"] = { pos.x,pos.y,pos.z };
	transformData["transform"]["rot"] = { rot.x,rot.y,rot.z };
	transformData["transform"]["scale"] = { scale.x,scale.y,scale.z };

	return transformData;
}

void Transform::LoadToJson(const nlohmann::json& componentField)
{
	if (!componentField.contains("transform"))
	{
		return;
	}

	nlohmann::json transformField = componentField["transform"];

	mComponentInfo.LoadToJson(transformField);

	pos.x = transformField["pos"][0];
	pos.y = transformField["pos"][1];
	pos.z = transformField["pos"][2];

	rot.x = transformField["rot"][0];
	rot.y = transformField["rot"][1];
	rot.z = transformField["rot"][2];

	scale.x = transformField["scale"][0];
	scale.y = transformField["scale"][1];
	scale.z = transformField["scale"][2];

	mGameObj->pos = pos;
	mGameObj->rot = rot;
	mGameObj->scale = scale;
}

void Transform::ShowDataToInspector()
{
	if (Gui::DrawCollapsingHeader("Transform", true))
	{
		std::string text = "Parent : empty";
		if (parent)
		{
			text = "Parent : " + parent->mGameObj->name;
		}

		Gui::DrawString(text.c_str());

		const float moveSpeed = 1.f;
		Gui::DrawSlider3("Pos", mGameObj->pos, moveSpeed);

		Vec3 angle = Angle(mGameObj->rot);
		Gui::DrawSlider3("Rot", angle, moveSpeed);
		mGameObj->rot = Radian(angle);

		Gui::DrawSlider3("Scale", mGameObj->scale, moveSpeed);
	}
}

// 以下、行列の取得関数
Mat4 Transform::GetTransMat() { return mTransMat; }    // 位置行列
Mat4 Transform::GetScaleMat() { return mScaleMat; }    // スケーリング行列
Mat4 Transform::GetRotMat() { return mRotMat; }        // 回転行列
Mat4 Transform::GetWorldMat() { return mWorldMat; }    // ワールド行列

// ビルボード設定関数
void Transform::SetWorldMat(Mat4 worldMat) { mWorldMat = worldMat; }
void Transform::SetBillboardType(const BillboardType type) { mBillboard.SetBillboardType(type); }