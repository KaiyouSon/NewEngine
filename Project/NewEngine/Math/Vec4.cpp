#include "Vec4.h"
#include "Vec3.h"

// コンストラクタ
Vec4::Vec4() : x(0), y(0), z(0), w(0) {}
Vec4::Vec4(const float num) : x(num), y(num), z(num), w(num) {}
Vec4::Vec4(const Vec3 vec3) : x(vec3.x), y(vec3.y), z(vec3.z), w(0) {}
Vec4::Vec4(const Vec3 vec3, const float num) : x(vec3.x), y(vec3.y), z(vec3.z), w(num) {}
Vec4::Vec4(const float x, const float y, const float z, const float w) : x(x), y(y), z(z), w(w) {}