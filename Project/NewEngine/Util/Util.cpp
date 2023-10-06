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
		return 0;
		break;
	}

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

// 荳芽ｧ貞ｽ｢縺ｮ豕慕ｷ壹ｒ豎ゅａ繧・
Vec3 GetTriangleNormal(const Vec3 p0, const Vec3 p1, const Vec3 p2)
{
	Vec3 v1 = p1 - p0;
	Vec3 v2 = p2 - p0;

	Vec3 normal = Vec3::Cross(v1, v2);

	return normal.Norm();
}

// Debug繝薙Ν繝峨・縺ｿ螳溯｡後☆繧・
void ProcessAtDebugBulid(std::function<void()> lambdaFunc)
{
#ifdef _DEBUG

	lambdaFunc();

#endif
}

// Release繝薙Ν繝芽ｺｫ縺ｮ螳溯｡後☆繧・
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

	// 蜿ｯ螟蛾聞蠑墓焚繧貞叙蠕励☆繧九◆繧√・蜃ｦ逅・
	va_list args;
	va_start(args, fmt);

	// 繝輔か繝ｼ繝槭ャ繝医＆繧後◆譁・ｭ怜・繧鍛uffer縺ｫ譬ｼ邏阪＠縺ｾ縺・
	vsnprintf(buffer, sizeof(buffer), fmt, args);

	// 蜿ｯ螟蛾聞蠑墓焚縺ｮ蜃ｦ逅・ｒ邨ゆｺ・＠縺ｾ縺・
	va_end(args);

	// 譛蠕後↓謾ｹ陦後ｒ霑ｽ蜉縺励∪縺・
	uint32_t len = (uint32_t)strlen(buffer);
	// 繝舌ャ繝輔ぃ縺ｫ蜊∝・縺ｪ菴呵｣輔′縺ゅｋ蝣ｴ蜷医・縺ｿ謾ｹ陦後ｒ霑ｽ蜉縺励∪縺・
	if (len < sizeof(buffer) - 2)
	{
		buffer[len] = '\n';
		buffer[len + 1] = '\0';
	}

	OutputDebugStringA(buffer);
#endif
}

