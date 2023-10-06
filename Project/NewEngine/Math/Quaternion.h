#pragma once
struct Vec3;

struct Quaternion
{
	float x, y, z, w;

	// 繧ｳ繝ｳ繧ｹ繝医Λ繧ｯ繧ｿ
	Quaternion() : x(0), y(0), z(0), w(0) {}
	Quaternion(const Vec3 v);
	Quaternion(const float x, const float y, const float z) : x(x), y(y), z(z), w(0) {}
	Quaternion(const float x, const float y, const float z, const float w) : x(x), y(y), z(z), w(w) {}

	// 髢｢謨ｰ
	float Length() const;			// 繧ｯ繧ｪ繝ｼ繧ｿ繝九が繝ｳ縺ｮ螟ｧ縺阪＆
	Quaternion Norm() const;		// 豁｣隕丞喧繧ｯ繧ｪ繝ｼ繧ｿ繝九が繝ｳ
	Quaternion Conjugate() const;	// 蜈ｱ蠖ｹ繧ｯ繧ｪ繝ｼ繧ｿ繝九が繝ｳ
	Quaternion Inverse() const;		// 騾・け繧ｪ繝ｼ繧ｿ繝九が繝ｳ

	// 莉ｻ諢剰ｻｸ蝗櫁ｻ｢
	Quaternion AnyAxisRotation(const Vec3 v, const float radian);

	static float Dot(const Quaternion q1, const Quaternion q2);		// 莠後▽縺ｮ繧ｯ繧ｪ繝ｼ繧ｿ繝九が繝ｳ縺ｮ蜀・ｩ・
	static Quaternion Identity();	// 蜊倅ｽ阪け繧ｪ繝ｼ繧ｿ繝九が繝ｳ
	static Quaternion Slerp(const Quaternion q1, const Quaternion q2, float t);
	static Quaternion MakeAxisAngle(const Vec3 v, const float radian);
	static Quaternion DirectionToDirection(const Vec3 v1, const Vec3 v2);
	static Quaternion Lerp(const Quaternion s, const Quaternion e, const float f);

	Quaternion operator-() const;

	// 邂苓｡捺ｼ皮ｮ怜ｭ舌・繧ｪ繝ｼ繝舌・繝ｭ繝ｼ繝・
	Quaternion operator+(const Quaternion other) const;	// 繧ゅ≧荳縺､縺ｮ繧ｯ繧ｪ繝ｼ繧ｿ繝九が繝ｳ縺ｨ縺ｮ雜ｳ縺礼ｮ・
	Quaternion operator-(const Quaternion other) const;	// 繧ゅ≧荳縺､縺ｮ繧ｯ繧ｪ繝ｼ繧ｿ繝九が繝ｳ縺ｨ縺ｮ蠑輔″邂・
	Quaternion operator*(const Quaternion other) const;	// 繧ゅ≧荳縺､縺ｮ繧ｯ繧ｪ繝ｼ繧ｿ繝九が繝ｳ縺ｨ縺ｮ謗帙￠邂・
	Quaternion operator*(const float num) const;			// 荳縺､縺ｮ蛟､縺ｨ縺ｮ謗帙￠邂・
	Quaternion operator/(const float num) const;			// 荳縺､縺ｮ蛟､縺ｨ縺ｮ蜑ｲ繧顔ｮ・

	// 隍・粋莉｣蜈･貍皮ｮ怜ｭ舌・繧ｪ繝ｼ繝舌・繝ｭ繝ｼ繝・
	Quaternion& operator+=(const Quaternion other);	// 隍・粋莉｣蜈･貍皮ｮ・+=
	Quaternion& operator-=(const Quaternion other);	// 隍・粋莉｣蜈･貍皮ｮ・-=
	Quaternion& operator*=(const Quaternion other);	// 隍・粋莉｣蜈･貍皮ｮ・*=
	Quaternion& operator*=(const float num);			// 隍・粋莉｣蜈･貍皮ｮ・*=
	Quaternion& operator/=(const float num);			// 隍・粋莉｣蜈･貍皮ｮ・/=
};

Quaternion operator*(const float num, const Quaternion q);
