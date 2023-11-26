#pragma once
#include "Vec3.h"

// 前方宣言
struct Vec3;

// ベクター2の構造体
struct Vec2
{
    // メンバ変数
    float x; // x座標
    float y; // y座標

    // 静的メンバ変数
    const static Vec2 left;     // Vector2(-1, 0) および左方向ベクトル
    const static Vec2 right;    // Vector2(1, 0) および右方向ベクトル
    const static Vec2 up;       // Vector2(0, 1) および上方向ベクトル
    const static Vec2 down;     // Vector2(0, -1) および下方向ベクトル
    const static Vec2 one;      // Vector2(1, 1) および(1, 1)ベクトル
    const static Vec2 zero;     // Vector2(0, 0) および(0, 0)ベクトル

    // コンストラクタ
    constexpr Vec2() : x(0), y(0) {};
    constexpr Vec2(const float num) : x(num), y(num) {};
    constexpr Vec2(const float x, const float y) : x(x), y(y) {};

    // 長さを取得
    float Length() const;   // ベクトルの長さを返す
    Vec2 Norm() const;      // 正規化ベクトルを返す
    float Area() const;     // 面積を返す

    // 静的メソッド
    static float Dot(const Vec2 v1, const Vec2 v2);        // 二つのベクトルの内積
    static float Cross(const Vec2 v1, const Vec2 v2);      // 二つのベクトルの外積
    static float Distance(const Vec2 v1, const Vec2 v2);   // 二つのベクトル間の距離

    // 二つのベクトルの要素ごとの最大値を返す
    static Vec2 Max(const Vec2 v1, const Vec2 v2);
    // 二つのベクトルの要素ごとの最小値を返す
    static Vec2 Min(const Vec2 v1, const Vec2 v2);

    // ベクトルの加算、減算、乗算、除算演算子
    Vec2 operator+(const Vec2 other) const; // ベクトルの加算
    Vec2 operator-(const Vec2 other) const; // ベクトルの減算
    Vec2 operator*(const Vec2 other) const; // ベクトルの乗算
    Vec2 operator/(const Vec2 other) const; // ベクトルの除算
    Vec2 operator*(float num) const;        // スカラー倍
    Vec2 operator/(float num) const;        // スカラー除算

    // ベクトルの加算、減算、乗算、除算代入演算子
    Vec2& operator+=(const Vec2 other);     // ベクトルの加算代入
    Vec2& operator-=(const Vec2 other);     // ベクトルの減算代入
    Vec2& operator+=(float num);            // スカラーの加算代入
    Vec2& operator-=(float num);            // スカラーの減算代入
    Vec2& operator*=(float num);            // スカラーの乗算代入
    Vec2& operator/=(float num);            // スカラーの除算代入

    // 代入演算子
    Vec2& operator=(float num);            // スカラーの代入
    Vec2& operator=(const Vec3 vec);       // 他のベクトルからの代入

    // 前置および後置インクリメント演算子
    Vec2& operator++();                     // 前置インクリメント演算子
    Vec2 operator++(int);                   // 後置インクリメント演算子
    Vec2& operator--();                     // 前置デクリメント演算子
    Vec2 operator--(int);                   // 後置デクリメント演算子

    // 比較演算子
    bool operator==(const Vec2 other);     // 等しいかどうか
    bool operator!=(const Vec2 other);     // 等しくないかどうか
    bool operator>=(const Vec2 other);     // 以上
    bool operator<=(const Vec2 other);     // 以下
    bool operator>(const Vec2 other);
    bool operator<(const Vec2 other);
    bool operator==(float num);            // 等しいかどうか
    bool operator!=(float num);            // 等しくないかどうか
    bool operator>=(float num);            // 以上
    bool operator<=(float num);            // 以下
    bool operator>(float num);
    bool operator<(float num);
};