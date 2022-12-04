#pragma once
class Vec4
{
public:
	// 変数
	float x; // ベクトルの x 成分
	float y; // ベクトルの y 成分
	float z; // ベクトルの z 成分
	float w; // ベクトルの w 成分

	// コンストラクタ
	Vec4() : x(0), y(0), z(0), w(0) {};
	Vec4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {};

	// 関数
	float Magnitude() const;    // ベクトルの大きさ
	float SqrMagnitude() const; // ベクトルの大きさの二乗
	Vec4 Normalized() const;    // 正規化にしたベクトル
	Vec4 Inverse();

	// static関数
	static float Dot(const Vec4& v1, const Vec4& v2);      // 二つのベクトルの内積
	static float Distance(const Vec4& v1, const Vec4& v2); // 二つのベクトルの距離

	// 二つのベクトルで各成分の一番大きな値を使用してベクトルを作成する
	static Vec4 Max(const Vec4& v1, const Vec4& v2);
	// 二つのベクトルで各成分の一番小さな値を使用してベクトルを作成する
	static Vec4 Min(const Vec4& v1, const Vec4& v2);


	// 算術演算子のオーバーロード
	Vec4 operator+(const Vec4& other) const; // もう一方のベクトルとの足し算
	Vec4 operator-(const Vec4& other) const; // もう一方のベクトルとの引き算
	Vec4 operator*(const Vec4& other) const; // もう一方のベクトルとの掛け算
	Vec4 operator/(const Vec4& other) const; // もう一方のベクトルとの割り算
	Vec4 operator*(float num) const;         // 一つの値との掛け算
	Vec4 operator/(float num) const;         // 一つの値との割り算

	// 複合代入演算子のオーバーロード
	Vec4& operator+=(const Vec4& other); // 複合代入演算 +=
	Vec4& operator-=(const Vec4& other); // 複合代入演算 -=
	Vec4& operator+=(float num);         // 複合代入演算 +=
	Vec4& operator-=(float num);         // 複合代入演算 -=
	Vec4& operator*=(float num);         // 複合代入演算 *=
	Vec4& operator/=(float num);         // 複合代入演算 /=

	// 代入演算子のオーバーロード
	Vec4& operator=(float num); // 一つの値を代入

	// インクリメント/デクリメント演算子のオーバーロード
	Vec4& operator++();   // 前置インクリメント
	Vec4 operator++(int); // 後置インクリメント
	Vec4& operator--();   // 前置デクリメント
	Vec4 operator--(int); // 後置デクリメント

	// 比較演算子のオーバーロード
	bool operator==(const Vec4& other);
	bool operator!=(const Vec4& other);
	bool operator>=(const Vec4& other);
	bool operator<=(const Vec4& other);
	bool operator==(float num);
	bool operator!=(float num);
	bool operator>=(float num);
	bool operator<=(float num);
};