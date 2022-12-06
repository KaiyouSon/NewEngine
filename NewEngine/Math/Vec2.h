#pragma once
#include "Vec3.h"

class Vec3;

class Vec2
{
public:
	// 変数
	float x; // ベクトルの x 成分
	float y; // ベクトルの y 成分

	// static変数
	const static Vec2 left;		// Vector2(-1, 0) と同じ意味
	const static Vec2 right;	// Vector2(1, 0) と同じ意味
	const static Vec2 up;		// Vector2(0, 1) と同じ意味
	const static Vec2 down;		// Vector2(0, -1) と同じ意味
	const static Vec2 one;		// Vector2(1, 1) と同じ意味
	const static Vec2 zero;		// Vector2(0, 0) と同じ意味

	// コンストラクタ
	constexpr Vec2() : x(0), y(0) {};
	constexpr Vec2(float x, float y) : x(x), y(y) {};

	// 関数
	float Lenght() const;	// ベクトルの大きさ
	Vec2 Norm() const;	// 正規化にしたベクトル

	// static関数
	static float Dot(const Vec2& v1, const Vec2& v2);		// 二つのベクトルの内積
	static float Cross(const Vec2& v1, const Vec2& v2);		// 二つのベクトルの外積
	static float Distance(const Vec2& v1, const Vec2& v2);	// 二つのベクトルの距離

	// 二つのベクトルで各成分の一番大きな値を使用してベクトルを作成する
	static Vec2 Max(const Vec2& v1, const Vec2& v2);
	// 二つのベクトルで各成分の一番小さな値を使用してベクトルを作成する
	static Vec2 Min(const Vec2& v1, const Vec2& v2);

	// 算術演算子のオーバーロード
	Vec2 operator+(const Vec2& other) const; // もう一方のベクトルとの足し算
	Vec2 operator-(const Vec2& other) const; // もう一方のベクトルとの引き算
	Vec2 operator*(const Vec2& other) const; // もう一方のベクトルとの掛け算
	Vec2 operator/(const Vec2& other) const; // もう一方のベクトルとの割り算
	Vec2 operator*(float num) const;	// 一つの値との掛け算
	Vec2 operator/(float num) const;	// 一つの値との割り算

	// 複合代入演算子のオーバーロード
	Vec2& operator +=(const Vec2& other); // 複合代入演算 +=
	Vec2& operator -=(const Vec2& other); // 複合代入演算 -=
	Vec2& operator +=(float num); // 複合代入演算 +=
	Vec2& operator -=(float num); // 複合代入演算 -=
	Vec2& operator *=(float num); // 複合代入演算 *=
	Vec2& operator /=(float num); // 複合代入演算 /=

	// 代入演算子のオーバーロード
	Vec2& operator=(float num);		  // 一つの値を代入
	Vec2& operator=(const Vec3& vec); // 一つの値を代入

	// インクリメント/デクリメント演算子のオーバーロード
	Vec2& operator++(); // 前置インクリメント
	Vec2 operator++(int); // 後置インクリメント
	Vec2& operator--(); // 前置デクリメント
	Vec2 operator--(int); // 後置デクリメント

	// 比較演算子のオーバーロード
	bool operator ==(const Vec2& other);
	bool operator !=(const Vec2& other);
	bool operator >=(const Vec2& other);
	bool operator <=(const Vec2& other);
	bool operator ==(float num);
	bool operator !=(float num);
	bool operator >=(float num);
	bool operator <=(float num);

};