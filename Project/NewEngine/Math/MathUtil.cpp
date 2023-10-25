#include "MathUtil.h"
#include "Util.h"
#include "Camera.h"
#include "Viewport.h"

Vec3 Vec3MulMat4(const Vec3 v, const Mat4& m, const bool isMulW)
{
	Vec3 result = Vec3::zero;
	if (isMulW == true)
	{
		float w = v.x * m.m[0][3] + v.y * m.m[1][3] + v.z * m.m[2][3] + m.m[3][3];

		result =
		{
		  (v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0] + m.m[3][0]) / w,
		  (v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1] + m.m[3][1]) / w,
		  (v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2] + m.m[3][2]) / w
		};
	}
	else
	{
		result =
		{
			 (v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0] + m.m[3][0]),
			 (v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1] + m.m[3][1]),
			 (v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2] + m.m[3][2])
		};
	}

	return result;
}

Mat4 ConvertScalingMat(Vec3 scale)
{
	return
	{
		scale.x,0,0,0,
		0,scale.y,0,0,
		0,0,scale.z,0,
		0,0,0,1
	};
}
Mat4 ConvertRotationXAxisMat(float angle)
{
	return
	{
		1,0,0,0,
		0, cosf(angle),sinf(angle),0,
		0,-sinf(angle),cosf(angle),0,
		0,0,0,1
	};
}
Mat4 ConvertRotationYAxisMat(float angle)
{
	return
	{
		cosf(angle),0,-sinf(angle),0,
		0,1,0,0,
		sinf(angle),0, cosf(angle),0,
		0,0,0,1
	};
}
Mat4 ConvertRotationZAxisMat(float angle)
{
	return
	{
		 cosf(angle),sinf(angle),0,0,
		-sinf(angle),cosf(angle),0,0,
		0,0,1,0,
		0,0,0,1
	};
}
Mat4 ConvertTranslationMat(const Vec3 pos)
{
	return
	{
		1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		pos.x,pos.y,pos.z,1
	};
}

Mat4 ConvertRotationMat(const Quaternion q)
{
	Mat4 result = Mat4::Identity();

	float xx = q.x * q.x;
	float yy = q.y * q.y;
	float zz = q.z * q.z;
	float ww = q.w * q.w;
	float xy = q.x * q.y * 2.0f;
	float xz = q.x * q.z * 2.0f;
	float yz = q.y * q.z * 2.0f;
	float wx = q.w * q.x * 2.0f;
	float wy = q.w * q.y * 2.0f;
	float wz = q.w * q.z * 2.0f;

	result =
	{
		ww + xx - yy - zz, xy + wz          , xz - wy          ,0.0f,
		xy - wz          , ww - xx + yy - zz, yz + wx          ,0.0f,
		xz + wy          , yz - wx          , ww - xx - yy + zz,0.0f,
		0.0f             ,0.0f              ,0.0f              ,1.0f
	};


	return result;
}

Mat4 ConvertBillBoardXAxis()
{
	Mat4 tempMat = Camera::current.GetViewLookToMat().Inverse();

	tempMat.m[0][0] = 1;
	tempMat.m[0][1] = 0;
	tempMat.m[0][2] = 0;
	tempMat.m[0][3] = 0;

	tempMat.m[3][0] = 0;
	tempMat.m[3][1] = 0;
	tempMat.m[3][2] = 0;
	tempMat.m[3][3] = 1;

	return tempMat;
}
Mat4 ConvertBillBoardYAxis()
{
	Mat4 tempMat = Camera::current.GetViewLookToMat().Inverse();

	tempMat.m[1][0] = 0;
	tempMat.m[1][1] = 1;
	tempMat.m[1][2] = 0;
	tempMat.m[1][3] = 0;

	tempMat.m[3][0] = 0;
	tempMat.m[3][1] = 0;
	tempMat.m[3][2] = 0;
	tempMat.m[3][3] = 1;

	return tempMat;
}
Mat4 ConvertBillBoardZAxis()
{
	Mat4 tempMat = Camera::current.GetViewLookToMat().Inverse();

	tempMat.m[2][0] = 0;
	tempMat.m[2][1] = 0;
	tempMat.m[2][2] = 1;
	tempMat.m[2][3] = 0;

	tempMat.m[3][0] = 0;
	tempMat.m[3][1] = 0;
	tempMat.m[3][2] = 0;
	tempMat.m[3][3] = 1;

	return tempMat;
}
Mat4 ConvertBillBoardAllAxis()
{
	Mat4 tempMat = Camera::current.GetViewLookToMat().Inverse();

	tempMat.m[3][0] = 0;
	tempMat.m[3][1] = 0;
	tempMat.m[3][2] = 0;
	tempMat.m[3][3] = 1;

	return tempMat;
}

Mat4 ConvertViewProjectionMatLookAt(const Vec3 pos, const Vec3 target, const Vec3 up)
{
	// 蜊倅ｽ崎｡悟・縺ｧ蛻晄悄蛹・
	Mat4 view = Mat4::Identity();

	// ・夊ｻｸ
	Vec3 zAxis = target - pos;
	view.m[2][0] = zAxis.Norm().x;
	view.m[2][1] = zAxis.Norm().y;
	view.m[2][2] = zAxis.Norm().z;

	// ・倩ｻｸ
	Vec3 xAxis = Vec3::Cross(up, zAxis);
	view.m[0][0] = xAxis.Norm().x;
	view.m[0][1] = xAxis.Norm().y;
	view.m[0][2] = xAxis.Norm().z;

	// ・呵ｻｸ
	Vec3 yAxis = Vec3::Cross(zAxis, xAxis);
	view.m[1][0] = yAxis.Norm().x;
	view.m[1][1] = yAxis.Norm().y;
	view.m[1][2] = yAxis.Norm().z;

	// 蟷ｳ陦檎ｧｻ蜍・
	view.m[3][0] = Vec3::Dot(pos, xAxis.Norm());
	view.m[3][1] = Vec3::Dot(pos, yAxis.Norm());
	view.m[3][2] = -Vec3::Dot(pos, zAxis.Norm());

	return view;
}
Mat4 ConvertViewProjectionMatLookTo(const Vec3 pos, const Vec3 zAxis, const Vec3 yAxis)
{
	// 蜊倅ｽ崎｡悟・縺ｧ蛻晄悄蛹・
	Mat4 view = Mat4::Identity();

	Vec3 xAxisVec = Vec3::Cross(yAxis, zAxis).Norm();
	Vec3 yAxisVec = Vec3::Cross(zAxis, xAxisVec).Norm();

	return
	{
		xAxisVec.x,yAxisVec.x,zAxis.x,0,
		xAxisVec.y,yAxisVec.y,zAxis.y,0,
		xAxisVec.z,yAxisVec.z,zAxis.z,0,
		Vec3::Dot(-pos, xAxisVec.Norm()),Vec3::Dot(-pos, yAxisVec.Norm()),Vec3::Dot(-pos, zAxis.Norm()),1,
	};
}
Mat4 ConvertPerspectiveProjectionMat(float fovAngle, float aspect, float nearZ, float farZ)
{
	// 蜊倅ｽ崎｡悟・縺ｧ蛻晄悄蛹・
	Mat4 perspective = Mat4::Zero();

	float scaleY = 1 / tanf(fovAngle / 2);
	float scaleX = 1 / tanf(fovAngle / 2) / aspect;
	float scaleZ = 1 / (farZ - nearZ) * farZ;
	float TransZ = -nearZ / (farZ - nearZ) * farZ;

	perspective.m[1][1] = scaleY;
	perspective.m[0][0] = scaleX;
	perspective.m[2][2] = scaleZ;
	perspective.m[3][2] = TransZ;
	perspective.m[2][3] = 1;

	return perspective;
}
Mat4 ConvertOrthographicProjectionMat(float WIN_WIDTH, float WIN_HEIGHT)
{
	return
	{
		2 / WIN_WIDTH,0,0,0,
		0,-2 / WIN_HEIGHT,0,0,
		0,0,1,0,
		-1,1,0,1
	};
}

Mat4 ConvertOrthographicProjectionMat(const RectAngle rect, const float nearZ, const float farZ)
{
	Mat4 orthoGrphic = Mat4::Identity();
	orthoGrphic.m[0][0] = 2 / (rect.right - rect.left);
	orthoGrphic.m[1][1] = 2 / (rect.top - rect.bottom);
	orthoGrphic.m[2][2] = 1 / (farZ - nearZ);

	orthoGrphic.m[3][0] = (rect.left + rect.right) / (rect.left - rect.right);
	orthoGrphic.m[3][1] = (rect.top + rect.bottom) / (rect.bottom - rect.top);
	orthoGrphic.m[3][2] = -nearZ / (farZ - nearZ);

	return orthoGrphic;
}

Mat4 ConvertViewportMat(Viewport& viewport)
{
	Mat4 viewportMat = Mat4::Identity();
	viewportMat.m[0][0] = viewport.GetSize().x / 2;
	viewportMat.m[1][1] = -viewport.GetSize().y / 2;
	viewportMat.m[2][2] = viewport.GetMaxDepth() - viewport.GetMinDepth();
	viewportMat.m[3][0] = viewport.GetLeftTopPos().x + viewport.GetSize().x / 2;
	viewportMat.m[3][1] = viewport.GetLeftTopPos().y + viewport.GetSize().y / 2;
	viewportMat.m[3][2] = viewport.GetMinDepth();
	return viewportMat;
}

Mat4 CalculateWorldMat(const Vec3 pos, const Vec3 scale, const Vec3 rot)
{
	Mat4 result = Mat4::Identity();

	// 蟷ｳ陦檎ｧｻ蜍輔√せ繧ｱ繝ｼ繝ｪ繝ｳ繧ｰ縲∝屓霆｢陦悟・菴懈・
	Mat4 transMat = Mat4::Identity();
	Mat4 scaleMat = Mat4::Identity();
	Mat4 rotMat = Mat4::Identity();

	transMat = ConvertTranslationMat(pos);	// 蟷ｳ陦檎ｧｻ蜍・
	scaleMat = ConvertScalingMat(scale);		// 繧ｹ繧ｱ繝ｼ繝ｪ繝ｳ繧ｰ
	rotMat *= ConvertRotationZAxisMat(rot.z);	// z霆ｸ蝗櫁ｻ｢
	rotMat *= ConvertRotationXAxisMat(rot.x);	// x霆ｸ蝗櫁ｻ｢
	rotMat *= ConvertRotationYAxisMat(rot.y);	// y霆ｸ蝗櫁ｻ｢

	result = scaleMat * rotMat * transMat;

	return result;
}
Mat4 CalculateWorldMat(const Vec3 pos, const Vec3 scale, const Quaternion rot)
{
	Mat4 result = Mat4::Identity();

	// 蟷ｳ陦檎ｧｻ蜍輔√せ繧ｱ繝ｼ繝ｪ繝ｳ繧ｰ縲∝屓霆｢陦悟・菴懈・
	Mat4 transMat = Mat4::Identity();
	Mat4 scaleMat = Mat4::Identity();
	Mat4 rotMat = Mat4::Identity();

	transMat = ConvertTranslationMat(pos);	// 蟷ｳ陦檎ｧｻ蜍・
	scaleMat = ConvertScalingMat(scale);	// 繧ｹ繧ｱ繝ｼ繝ｪ繝ｳ繧ｰ
	rotMat = ConvertRotationMat(rot);		// 蝗櫁ｻ｢

	result = scaleMat * rotMat * transMat;

	return result;
}

Vec3 operator+(const float num, const Vec3 v) { return { num + v.x,num + v.y,num + v.z }; }
Vec3 operator-(const float num, const Vec3 v) { return { num - v.x,num - v.y,num - v.z }; }
Vec3 operator*(const float num, const Vec3 v) { return { num * v.x,num * v.y,num * v.z }; }
Vec3 operator/(const float num, const Vec3 v) { return { num / v.x,num / v.y,num / v.z }; }

