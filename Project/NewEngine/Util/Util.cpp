#include "Util.h"
#include "RenderBase.h"
#include "Camera.h"

namespace fs = std::filesystem;

// 浮動小数点数の符号を取得する
int32_t Sign(const float a, const float limit)
{
	int32_t result = 0;

	if (a > limit)
	{
		result = 1;
	}
	else if (a < -limit)
	{
		result = -1;
	}
	return result;
}

// 2次元ベクトルの各要素に対して符号を取得する
Vec2 Sign(const Vec2 a, const Vec2 limit)
{
	return { (float)Sign(a.x,limit.x), (float)Sign(a.y,limit.y) };
}

// 3次元ベクトルの各要素に対して符号を取得する
Vec3 Sign(const Vec3 a, const Vec3 limit)
{
	return { (float)Sign(a.x,limit.x), (float)Sign(a.y,limit.y), (float)Sign(a.z,limit.z) };
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
		Camera::current.GetViewMat() *
		Camera::current.GetPerspectiveMat() *
		viewportMat;

	Vec3 result = Vec3MulMat4(worldPos, finalMat, true);

	return { result.x, result.y };
}

// スクリーン座標をワールド座標に変換する
Vec3 ScreenToWorld(const Vec2 screenPos)
{
	Mat4 viewMat = Camera::current.GetViewMat();
	Mat4 projMat = Camera::current.GetPerspectiveMat();
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
void CopyFileToDestination(const WCHAR* srcPath, const WCHAR* destFolder, std::wstring* newPath)
{
	WCHAR szFileName[MAX_PATH];
	wcscpy_s(szFileName, srcPath);
	PathStripPath(const_cast<LPWSTR>(szFileName));

	std::wstring szDestination = destFolder;
	szDestination += szFileName;

	CopyFile(srcPath, szDestination.c_str(), false);

	if (newPath)
	{
		*newPath = szDestination;
	}
}

void CopyFolderToDestination(const std::wstring srcFolder, const std::wstring destFolder, std::wstring* newPath)
{
	fs::create_directories(destFolder);

	for (const auto& entry : fs::directory_iterator(srcFolder))
	{
		if (entry.is_regular_file())
		{
			std::wstring fileName = entry.path().filename();
			std::wstring srcPath = srcFolder + L"/" + fileName;
			std::wstring destPath = destFolder + L"/" + fileName;

			// ファイルをコピー
			CopyFile(srcPath.c_str(), destPath.c_str(), FALSE);
		}
	}

	if (newPath)
	{
		*newPath = destFolder;
	}
}

void Loop(const uint32_t count, std::function<void()> func)
{
	for (uint32_t i = 0; i < count; i++)
	{
		func();
	}
}

std::string WStrToStr(const std::wstring& wstr)
{
	int32_t bufferSize = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, NULL, 0, NULL, NULL);

	std::string str;
	str.resize(bufferSize - 1); // NULL終端文字を含まないように調整
	WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, &str[0], bufferSize, NULL, NULL);

	return str;
}

std::string SubString(const std::string& mainStr, const std::string& subStr)
{
	std::string result = mainStr;

	uint32_t pos = (uint32_t)result.find(subStr);
	if (pos != std::string::npos)
	{
		// 文字列が見つかった場合
		result.erase(pos, subStr.length()); // 見つかった位置からtoRemoveの長さ分削除する
	}

	return result;
}
