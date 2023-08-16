#include "Util.h"
#include "RenderBase.h"
#include "Camera.h"

uint32_t Sign(const float a)
{
	return a >= 0 ? 1 : -1;
}

Vec2 Sign(const Vec2 a)
{
	return { (float)Sign(a.x),(float)Sign(a.y) };
}

Vec3 Sign(const Vec3 a)
{
	return { (float)Sign(a.x),(float)Sign(a.y),(float)Sign(a.z) };
}

uint32_t GetDight(const uint32_t value)
{
	uint32_t tempValue = value;
	uint32_t digit = 0;
	while (tempValue != 0)
	{
		tempValue /= 10;
		digit++;
	}
	return digit;
}

unsigned long GetNowTime(const TimeUnit timeUnit)
{
	switch (timeUnit)
	{
	case TimeUnit::MilliSecond:
		return (unsigned long)(GetTickCount64());
		break;
	case TimeUnit::Second:
		return (unsigned long)((GetTickCount64() / 1000) % 60);
		break;
	case TimeUnit::Minute:
		return (unsigned long)((GetTickCount64() / 60000) % 60);
		break;
	default:
		break;
	}

	return -1;
}
Vec2 WorldToScreen(const Vec3 worldPos)
{
	Mat4 viewportMat =
		ConvertViewportMat(*RenderBase::GetInstance()->GetViewport());

	Mat4 finalMat =
		Camera::current.GetViewLookAtMat() *
		Camera::current.GetPerspectiveProjectionMat() *
		viewportMat;

	Vec3 result = Vec3MulMat4(worldPos, finalMat, true);

	return { result.x,result.y };
}

// 三角形の法線を求める
Vec3 GetTriangleNormal(const Vec3 p0, const Vec3 p1, const Vec3 p2)
{
	Vec3 v1 = p1 - p0;
	Vec3 v2 = p2 - p0;

	Vec3 normal = Vec3::Cross(v1, v2);

	return normal.Norm();
}

// Debugビルドのみ実行する
void ProcessAtDebugBulid(std::function<void()> lambdaFunc)
{
#ifdef _DEBUG

	lambdaFunc();

#endif
}

// Releaseビルド身の実行する
void ProcessAtReleaseBulid(std::function<void()> lambdaFunc)
{
#ifdef NDEBUG

	lambdaFunc();

#endif
}

void OutputDebugLog(const char* fmt ...)
{
#ifdef _DEBUG
	char buffer[1024]{};

	// 可変長引数を取得するための処理
	va_list args;
	va_start(args, fmt);

	// フォーマットされた文字列をbufferに格納します
	vsnprintf(buffer, sizeof(buffer), fmt, args);

	// 可変長引数の処理を終了します
	va_end(args);

	// 最後に改行を追加します
	uint32_t len = (uint32_t)strlen(buffer);
	// バッファに十分な余裕がある場合のみ改行を追加します
	if (len < sizeof(buffer) - 2)
	{
		buffer[len] = '\n';
		buffer[len + 1] = '\0';
	}

	OutputDebugStringA(buffer);
#endif
}
