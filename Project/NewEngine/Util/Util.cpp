#include "Util.h"
#include "RenderBase.h"
#include "Camera.h"

// 浮動小数点数の符号を取得する
uint32_t Sign(const float a)
{
	return a >= 0 ? 1 : -1;
}

// 2次元ベクトルの各要素に対して符号を取得する
Vec2 Sign(const Vec2 a)
{
	return { (float)Sign(a.x), (float)Sign(a.y) };
}

// 3次元ベクトルの各要素に対して符号を取得する
Vec3 Sign(const Vec3 a)
{
	return { (float)Sign(a.x), (float)Sign(a.y), (float)Sign(a.z) };
}

// 整数の桁数を取得する
uint32_t GetDigit(const uint32_t value)
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

// 現在の時間を取得する
uint64_t GetNowTime(const TimeUnit timeUnit)
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

// ワールド座標をスクリーン座標に変換する
Vec2 WorldToScreen(const Vec3 worldPos)
{
	Mat4 viewportMat =
		ConvertViewportMat(*RenderBase::GetInstance()->GetViewport());

	Mat4 finalMat =
		Camera::current.GetViewLookToMat() *
		Camera::current.GetPerspectiveProjectionMat() *
		viewportMat;

	Vec3 result = Vec3MulMat4(worldPos, finalMat, true);

	return { result.x, result.y };
}

// スクリーン座標をワールド座標に変換する
Vec3 ScreenToWorld(const Vec2 screenPos)
{
	Mat4 viewMat = Camera::current.GetViewLookToMat();
	Mat4 projMat = Camera::current.GetPerspectiveProjectionMat();
	Mat4 viewportMat = ConvertViewportMat(*RenderBase::GetInstance()->GetViewport());
	Mat4 conpositeMat = viewMat * projMat * viewportMat;

	Vec3 worldPos = Vec3MulMat4(Vec3(screenPos.x, screenPos.y, 1), conpositeMat.Inverse(), true);

	return worldPos;
}

// 3つの点から三角形の法線ベクトルを計算する
Vec3 GetTriangleNormal(const Vec3 p0, const Vec3 p1, const Vec3 p2)
{
	Vec3 v1 = p1 - p0;
	Vec3 v2 = p2 - p0;

	Vec3 normal = Vec3::Cross(v1, v2);

	return normal.Norm();
}

// Debugビルド時にのみ実行される処理
void ProcessAtDebugBuild(std::function<void()> lambdaFunc)
{
#ifdef _DEBUG
	lambdaFunc();
#endif
}

// Releaseビルド時にのみ実行される処理
void ProcessAtReleaseBuild(std::function<void()> lambdaFunc)
{
#ifndef _DEBUG
	lambdaFunc();
#endif
}

// デバッグログを出力する
void OutputDebugLog(const char* fmt, ...)
{
#ifdef _DEBUG
	char buffer[1024]{};

	// 可変引数を処理し、文字列を生成する
	va_list args;
	va_start(args, fmt);
	vsnprintf(buffer, sizeof(buffer), fmt, args);
	va_end(args);

	// バッファの最後に改行文字を追加する
	uint32_t len = (uint32_t)strlen(buffer);
	if (len < sizeof(buffer) - 2)
	{
		buffer[len] = '\n';
		buffer[len + 1] = '\0';
	}

	// デバッグログを出力する
	OutputDebugStringA(buffer);
#endif

#ifdef NDEBUG
	fmt;
#endif

}

// ファイルを指定した場所にコピペする処理
void CopyFileToDestination(const WCHAR* srcPath, const WCHAR* destFolder)
{
	WCHAR szFileName[MAX_PATH];
	wcscpy_s(szFileName, srcPath);
	PathStripPath(const_cast<LPWSTR>(szFileName));

	std::wstring szDestination = destFolder;
	szDestination += L"\\";
	szDestination += szFileName;

	CopyFile(srcPath, szDestination.c_str(), false);
}
