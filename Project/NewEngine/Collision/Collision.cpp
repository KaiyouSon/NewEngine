#include "Collision.h"
#include "Util.h"

// 蜀・→蜀・
bool Collision::CircleHitCircle(const CircleCollider& circle1, const CircleCollider& circle2)
{
	if (circle1.isActive == false || circle2.isActive == false)
	{
		return false;
	}

	return
	{
		(circle2.centerPos.x - circle1.centerPos.x) *
		(circle2.centerPos.x - circle1.centerPos.x) +
		(circle2.centerPos.y - circle1.centerPos.y) *
		(circle2.centerPos.y - circle1.centerPos.y) <=
		(circle2.radius + circle1.radius) *
		(circle2.radius + circle1.radius)
	};
}

// 逅・→逅・
bool Collision::SphereHitSphere(const SphereCollider& sphere1, const SphereCollider& sphere2)
{
	if (sphere1.isActive == false || sphere2.isActive == false)
	{
		return false;
	}

	return
	{
		(sphere2.centerPos.x - sphere1.centerPos.x) *
		(sphere2.centerPos.x - sphere1.centerPos.x) +
		(sphere2.centerPos.y - sphere1.centerPos.y) *
		(sphere2.centerPos.y - sphere1.centerPos.y) +
		(sphere2.centerPos.z - sphere1.centerPos.z) *
		(sphere2.centerPos.z - sphere1.centerPos.z) <=
		(sphere2.radius + sphere1.radius) *
		(sphere2.radius + sphere1.radius)
	};
}

// 繧ｭ繝･繝ｼ繝悶→繧ｭ繝･繝ｼ繝・
bool Collision::CubeHitCube(const CubeCollider& cube1, const CubeCollider& cube2)
{
	if (cube1.isActive == false || cube2.isActive == false)
	{
		return false;
	}

	// 驥阪↑繧雁愛螳・
	for (int i = 0; i < 8; ++i)
	{
		for (int j = 0; j < 8; ++j)
		{
			bool isXHit =
				std::fabs(cube1.points[i].x - cube2.points[j].x) <= (cube1.size.x + cube2.size.x) / 2.0;

			bool isYHit =
				std::fabs(cube1.points[i].y - cube2.points[j].y) <= (cube1.size.y + cube2.size.y) / 2.0;

			bool isZHit =
				std::fabs(cube1.points[i].z - cube2.points[j].z) <= (cube1.size.z + cube2.size.z) / 2.0;

			if (isXHit && isYHit && isZHit)
			{
				return true;  // 驥阪↑繧翫′縺ゅｋ蝣ｴ蜷医・蠖薙◆繧翫→縺吶ｋ
			}
		}
	}

	return false;  // 驥阪↑繧翫′縺ｪ縺・ｴ蜷医・蠖薙◆繧翫→縺励↑縺・
}

// 繧ｭ繝･繝ｼ繝悶→繧ｫ繝励そ繝ｫ
bool Collision::CubeHitCapsule(CubeCollider& cube, CapsuleCollider& capsule)
{
	if (cube.isActive == false || capsule.isActive == false)
	{
		return false;
	}

	// 繧ｫ繝励そ繝ｫ縺ｮ蟋狗せ縺ｨ邨らせ繧偵・繧ｯ繝医Ν蠖｢蠑上↓螟画鋤
	Vec3 capsuleDir = capsule.endPos - capsule.startPos;
	Vec3 capsuleStartToEnd = capsule.endPos - capsule.startPos;

	// 繧ｫ繝励そ繝ｫ縺ｮ蟋狗せ縺九ｉ繧ｭ繝･繝ｼ繝悶・荳ｭ蠢・∪縺ｧ縺ｮ繝吶け繝医Ν
	Vec3 capsuleToCube = cube.centerPos - capsule.startPos;

	// 繧ｫ繝励そ繝ｫ縺ｮ蟋狗せ縺九ｉ繧ｭ繝･繝ｼ繝悶・譛霑第磁轤ｹ繧定ｨ育ｮ・
	Vec3 closestPoint;
	float d1 = Vec3::Dot(capsuleToCube, capsuleDir);
	float d2 = Vec3::Dot(capsuleDir, capsuleDir);

	float t = Clamp<float>(d1 / d2, 0.0f, 1.0f);
	closestPoint = capsule.startPos + t * capsuleDir;

	// 繧ｭ繝･繝ｼ繝悶・蜷・ｾｺ縺ｫ蟇ｾ縺吶ｋ譛霑第磁轤ｹ繧定ｨ育ｮ・
	Vec3 closestCubePoint = closestPoint;
	closestCubePoint.x = Clamp(closestCubePoint.x, cube.centerPos.x - cube.size.x / 2.0f, cube.centerPos.x + cube.size.x / 2.0f);
	closestCubePoint.y = Clamp(closestCubePoint.y, cube.centerPos.y - cube.size.y / 2.0f, cube.centerPos.y + cube.size.y / 2.0f);
	closestCubePoint.z = Clamp(closestCubePoint.z, cube.centerPos.z - cube.size.z / 2.0f, cube.centerPos.z + cube.size.z / 2.0f);

	// 繧ｫ繝励そ繝ｫ縺ｨ繧ｭ繝･繝ｼ繝悶・譛霑第磁轤ｹ髢薙・霍晞屬繧定ｨ育ｮ・
	float distanceSq = Vec3::Dot(closestCubePoint - closestPoint, closestCubePoint - closestPoint);

	// 霍晞屬縺後き繝励そ繝ｫ蜊雁ｾ・→繧ｭ繝･繝ｼ繝門ｯｾ隗堤ｷ壹・蜊雁・縺ｮ蜥後ｈ繧雁ｰ上＆縺・ｴ蜷医∬｡晉ｪ√＠縺ｦ縺・ｋ縺ｨ縺ｿ縺ｪ縺・
	float capsuleRadius = capsule.radius;
	float cubeDiagonal = sqrtf(cube.size.Length()) / 2.0f;

	bool isHit = distanceSq * distanceSq <= (capsuleRadius + cubeDiagonal) * (capsuleRadius + cubeDiagonal);

	return isHit;
}

bool Collision::CubeHitCapsule(const CubeCollider& cube, const CapsuleCollider& capsule, Vec3& hitPoint)
{
	if (cube.isActive == false || capsule.isActive == false)
	{
		return false;
	}

	// 繧ｫ繝励そ繝ｫ縺ｮ蟋狗せ縺ｨ邨らせ繧偵・繧ｯ繝医Ν蠖｢蠑上↓螟画鋤
	Vec3 capsuleDir = capsule.endPos - capsule.startPos;

	// 繧ｫ繝励そ繝ｫ縺ｮ蟋狗せ縺九ｉ繧ｭ繝･繝ｼ繝悶・荳ｭ蠢・∪縺ｧ縺ｮ繝吶け繝医Ν
	Vec3 capsuleToCube = cube.centerPos - capsule.startPos;

	// 繧ｫ繝励そ繝ｫ縺ｮ蟋狗せ縺九ｉ繧ｭ繝･繝ｼ繝悶・譛霑第磁轤ｹ繧定ｨ育ｮ・
	Vec3 closestPoint;
	float d1 = Vec3::Dot(capsuleToCube, capsuleDir);
	float d2 = Vec3::Dot(capsuleDir, capsuleDir);

	float t = Clamp<float>(d1 / d2, 0.0f, 1.0f);
	closestPoint = capsule.startPos + t * capsuleDir;

	// 繧ｭ繝･繝ｼ繝悶・蜷・ｾｺ縺ｫ蟇ｾ縺吶ｋ譛霑第磁轤ｹ繧定ｨ育ｮ・
	Vec3 closestCubePoint = closestPoint;
	closestCubePoint.x = Clamp(closestCubePoint.x, cube.centerPos.x - cube.size.x, cube.centerPos.x + cube.size.x);
	closestCubePoint.y = Clamp(closestCubePoint.y, cube.centerPos.y - cube.size.y, cube.centerPos.y + cube.size.y);
	closestCubePoint.z = Clamp(closestCubePoint.z, cube.centerPos.z - cube.size.z, cube.centerPos.z + cube.size.z);

	// 繧ｫ繝励そ繝ｫ縺ｨ繧ｭ繝･繝ｼ繝悶・譛霑第磁轤ｹ髢薙・霍晞屬繧定ｨ育ｮ・
	float distanceSq = Vec3::Dot(closestCubePoint - closestPoint, closestCubePoint - closestPoint);

	// 霍晞屬縺後き繝励そ繝ｫ蜊雁ｾ・→繧ｭ繝･繝ｼ繝門ｯｾ隗堤ｷ壹・蜊雁・縺ｮ蜥後ｈ繧雁ｰ上＆縺・ｴ蜷医∬｡晉ｪ√＠縺ｦ縺・ｋ縺ｨ縺ｿ縺ｪ縺・
	float capsuleRadius = capsule.radius;
	float cubeDiagonal = cube.size.Length() / 2;

	bool isHit = distanceSq * distanceSq <= (capsuleRadius + cubeDiagonal) * (capsuleRadius + cubeDiagonal);

	if (isHit)
	{
		// 陦晉ｪ√＠縺溷ｺｧ讓吶ｒ險育ｮ励＠縲∝ｼ墓焚縺ｫ莉｣蜈･
		hitPoint = closestCubePoint;
		return true;
	}

	return false;

}

// 逅・→蟷ｳ髱｢
bool Collision::SphereHitPlane(const SphereCollider& sphere, const PlaneCollider& plane, Vec3* hitPoint)
{
	if (sphere.isActive == false || plane.isActive == false)
	{
		return false;
	}

	// 蠎ｧ讓咏ｳｻ縺ｮ蜴溽せ縺九ｉ逅・・荳ｭ蠢・ｺｧ讓吶∈縺ｮ霍晞屬
	float dot1 = Vec3::Dot(sphere.centerPos, plane.normal);
	// 蟷ｳ髱｢縺ｮ蜴溽せ霍晞屬
	float dot2 = Vec3::Dot(plane.centerPos, plane.normal);
	float dis = dot1 - dot2;

	if (fabsf(dis) > sphere.radius)
	{
		return false;
	}

	if (hitPoint != nullptr)
	{
		*hitPoint = dis * plane.normal + sphere.centerPos;
	}

	return true;
}

// 逅・→荳芽ｧ貞ｽ｢
bool Collision::SphereHitTriangle(const SphereCollider& sphere, const TriangleCollider& triangle, Vec3* hitPoint)
{
	if (sphere.isActive == false || triangle.isActive == false)
	{
		return false;
	}

	Vec3 p = ClosestPointOfPointAndTriangle(sphere.centerPos, triangle);

	Vec3 v = p - sphere.centerPos;

	float value = Vec3::Dot(v, v);

	if (value > sphere.radius * sphere.radius)
	{
		return false;
	}

	if (hitPoint != nullptr)
	{
		*hitPoint = p;
	}

	return true;
}

// 逅・→繧ｫ繝励そ繝ｫ
bool Collision::SphereHitCapsule(const SphereCollider& sphere, const CapsuleCollider& capsule)
{
	if (sphere.isActive == false || capsule.isActive == false)
	{
		return false;
	}

	// 繧ｫ繝励そ繝ｫ縺ｮ霆ｸ繝吶け繝医Ν縺ｨ逅・・荳ｭ蠢・ｽ咲ｽｮ繝吶け繝医Ν縺ｮ蟾ｮ繧定ｨ育ｮ・
	Vec3 capsuleAxis = capsule.endPos - capsule.startPos;
	Vec3 sphereToCapsule = sphere.centerPos - capsule.startPos;

	Vec3 n = capsuleAxis.Norm();

	// 繧ｫ繝励そ繝ｫ縺ｮ霆ｸ繝吶け繝医Ν荳翫・譛霑第磁轤ｹ繧定ｨ育ｮ・
	float dot = Vec3::Dot(sphereToCapsule, n);
	float lengthRate = dot * dot / capsuleAxis.LengthSq();

	float dis = 0;
	if (lengthRate < 0.0f)
	{
		dis = Vec3::DistanceSq(capsule.startPos, sphere.centerPos);
	}
	else if (lengthRate <= 1.0f)
	{
		Vec3 p = n * dot;
		dis = Vec3::DistanceSq(capsule.startPos + p, sphere.centerPos);
	}
	else
	{
		dis = Vec3::DistanceSq(capsule.endPos, sphere.centerPos);
	}

	// 霍晞屬縺悟濠蠕・・蜷郁ｨ医ｈ繧翫ｂ蟆上＆縺・ｴ蜷医・陦晉ｪ√＠縺ｦ縺・ｋ縺ｨ縺ｿ縺ｪ縺・
	float totalRadius = sphere.radius + capsule.radius;

	if (dis <= totalRadius * totalRadius)
	{
		return true;
	}

	return false;
}

bool Collision::SphereHitCapsule(const SphereCollider& sphere, const CapsuleCollider& capsule, Vec3& hitPoint)
{
	if (sphere.isActive == false || capsule.isActive == false)
	{
		return false;
	}

	// 繧ｫ繝励そ繝ｫ縺ｮ霆ｸ繝吶け繝医Ν縺ｨ逅・・荳ｭ蠢・ｽ咲ｽｮ繝吶け繝医Ν縺ｮ蟾ｮ繧定ｨ育ｮ・
	Vec3 capsuleAxis = capsule.endPos - capsule.startPos;
	Vec3 sphereToCapsule = sphere.centerPos - capsule.startPos;

	Vec3 n = capsuleAxis.Norm();

	// 繧ｫ繝励そ繝ｫ縺ｮ霆ｸ繝吶け繝医Ν荳翫・譛霑第磁轤ｹ繧定ｨ育ｮ・
	float dot = Vec3::Dot(sphereToCapsule, n);
	Vec3 closestPoint = capsule.startPos + n * dot;

	float dis = 0;
	float lengthRate = dot * dot / capsuleAxis.LengthSq();
	if (lengthRate < 0.0f)
	{
		dis = Vec3::DistanceSq(capsule.startPos, sphere.centerPos);
	}
	else if (lengthRate <= 1.0f)
	{
		dis = Vec3::DistanceSq(closestPoint, sphere.centerPos);
	}
	else
	{
		dis = Vec3::DistanceSq(capsule.endPos, sphere.centerPos);
	}

	// 霍晞屬縺悟濠蠕・・蜷郁ｨ医ｈ繧翫ｂ蟆上＆縺・ｴ蜷医・陦晉ｪ√＠縺ｦ縺・ｋ縺ｨ縺ｿ縺ｪ縺・
	float totalRadius = sphere.radius + capsule.radius;

	if (dis <= totalRadius * totalRadius)
	{
		Vec3 v = sphere.centerPos - closestPoint;
		hitPoint = closestPoint + v.Norm() * capsule.radius;
		return true;
	}

	return false;
}

// 繝ｬ繧､縺ｨ蟷ｳ髱｢
bool Collision::RayHitPlane(const RayCollider& ray, const PlaneCollider& plane)
{
	if (ray.isActive == false || plane.isActive == false)
	{
		return false;
	}

	const float epsilon = 1.0e-5f;

	// 蠎ｧ讓咏ｳｻ縺ｮ蜴溽せ縺九ｉ繝ｬ繧､縺ｮ蟋狗せ蠎ｧ讓吶∈縺ｮ霍晞屬
	float dot1 = Vec3::Dot(plane.normal, ray.dirVec);

	// 陬城擇蛻､螳・
	if (dot1 > -epsilon)
	{
		return false;
	}

	// 蟷ｳ髱｢縺ｮ蜴溽せ霍晞屬
	float dot2 = Vec3::Dot(plane.normal, ray.startPos);

	float dis = dot2 - Vec3::Dot(plane.centerPos, plane.normal);

	float t = dis / -dot1;
	if (t < 0)
	{
		return false;
	}

	return true;
}

// 繝ｬ繧､縺ｨ逅・
bool Collision::RayHitSphere(const RayCollider& ray, const SphereCollider& sphere)
{
	if (ray.isActive == false || sphere.isActive == false)
	{
		return false;
	}

	Vec3 v = ray.startPos - sphere.centerPos;
	float dot1 = Vec3::Dot(v, ray.dirVec);
	float dot2 = Vec3::Dot(v, v) - (sphere.radius * sphere.radius);

	// 繝ｬ繧､縺檎帥縺九ｉ髮｢繧後※縺・￥譁ｹ蜷代ｒ謖・＠縺ｦ縺・ｋ蝣ｴ蜷・b > 0)縲√Ξ繧､縺ｮ蟋狗せ縺茎phere縺ｮ螟門・縺ｫ縺ゅｊ(c > 0)
	if (dot1 > 0.f && dot2 > 0.f)
	{
		return false;
	}

	// 雋縺ｮ蛻､蛻･蠑上・繝ｬ繧､縺檎帥繧貞､悶ｌ縺ｦ縺・ｋ縺薙→縺ｫ荳閾ｴ
	float discr = (dot1 * dot1) - dot2;
	if (discr < 0.f)
	{
		return false;
	}

	return true;
}

// 繧ｫ繝励そ繝ｫ縺ｨ繧ｫ繝励そ繝ｫ縺ｮ蠖薙◆繧雁愛螳・
bool Collision::CapsuleHitCapsule(const CapsuleCollider& capsule1, const CapsuleCollider& capsule2)
{
	if (capsule1.isActive == false || capsule2.isActive == false)
	{
		return false;
	}

	Vec3 empty = 0;
	return CapsuleHitCapsule(capsule1, capsule2, empty);
}

// 繧ｫ繝励そ繝ｫ縺ｨ繧ｫ繝励そ繝ｫ
bool Collision::CapsuleHitCapsule(const CapsuleCollider& capsule1, const CapsuleCollider& capsule2, Vec3& hitPoint)
{
	if (capsule1.isActive == false || capsule2.isActive == false)
	{
		return false;
	}

	Vec3 d1 = capsule1.endPos - capsule1.startPos;
	Vec3 d2 = capsule2.endPos - capsule2.startPos;

	Vec3 r = capsule1.startPos - capsule2.startPos;

	float a = Vec3::Dot(d1, d1);
	float b = Vec3::Dot(d1, d2);
	float e = Vec3::Dot(d2, d2);

	float c = Vec3::Dot(d1, r);
	float f = Vec3::Dot(d2, r);

	float s = 0.f;
	float t = 0.f;

	float denominator = a * e - b * b;

	// s 縺ｮ蛟､繧呈ｱゅａ繧・
	if (denominator != 0.0f)
	{
		s = (b * f - c * e) / denominator;
		s = Clamp(s);
	}
	else
	{
		s = 0;
	}

	// t 縺ｮ蛟､繧呈ｱゅａ繧・
	t = (f + b * s) / e;

	// t 縺啓 0.0, 1.0 ]縺ｮ遽・峇縺ｧ縺ゅｌ縺ｰ縲《 繧貞・險育ｮ・
	if (t < 0.0f)
	{
		t = 0.0f;
		s = Clamp(-c / a);
	}
	else if (t > 1.0f)
	{
		s = Clamp((b - c) / a);
		t = 1.0f;
	}

	Vec3 c1 = capsule1.startPos + d1 * s;
	Vec3 c2 = capsule2.startPos + d2 * t;

	float disPow2 =
		((c2.x - c1.x) * (c2.x - c1.x)) +
		((c2.y - c1.y) * (c2.y - c1.y)) +
		((c2.z - c1.z) * (c2.z - c1.z));

	// 莠後▽縺ｮ繧ｫ繝励そ繝ｫ縺ｮ蜊雁ｾ・・蜥・
	float radius = capsule1.radius + capsule2.radius;

	if (disPow2 < radius * radius)
	{
		// Calculate the collision point
		float distance = sqrt(disPow2);
		float penetration = radius - distance;

		Vec3 direction = (c2 - c1) / distance;
		hitPoint = c1 + direction * (capsule1.radius - 0.5f * penetration);

		return true;
	}

	return false;
}

// 轤ｹ縺ｨ荳芽ｧ貞ｽ｢縺ｮ譛霑第磁轤ｹ
Vec3 Collision::ClosestPointOfPointAndTriangle(const Vec3 point, const TriangleCollider& triangle)
{
	if (triangle.isActive == false)
	{
		return false;
	}

	// point縺継0縺ｮ螟門・縺ｮ鬆らせ鬆伜沺縺ｮ荳ｭ縺ｫ縺ゅｋ縺九←縺・°繝√ぉ繝・け
	Vec3 p0_p1 = triangle.p1 - triangle.p0;
	Vec3 p0_p2 = triangle.p2 - triangle.p0;
	Vec3 p0_pt = point - triangle.p0;

	float d1 = Vec3::Dot(p0_p1, p0_pt);
	float d2 = Vec3::Dot(p0_p2, p0_pt);

	if (d1 <= 0.0f && d2 <= 0.0f)
	{
		// p0縺梧怙霑大ｍ
		return triangle.p0;
	}

	// point縺継1縺ｮ螟門・縺ｮ鬆らせ鬆伜沺縺ｮ荳ｭ縺ｫ縺ゅｋ縺九←縺・°繝√ぉ繝・け
	Vec3 p1_pt = point - triangle.p1;

	float d3 = Vec3::Dot(p0_p1, p1_pt);
	float d4 = Vec3::Dot(p0_p2, p1_pt);

	if (d3 >= 0.0f && d4 <= d3)
	{
		// p1縺梧怙霑大ｍ
		return triangle.p1;
	}

	// point縺継0_p1縺ｮ霎ｺ鬆伜沺縺ｮ荳ｭ縺ｫ縺ゅｋ縺九←縺・°繝√ぉ繝・け縺励√≠繧後・point縺ｮp0_p1荳翫↓蟇ｾ縺吶ｋ蟆・ｽｱ繧定ｿ斐☆
	float vc = d1 * d4 - d3 * d2;
	if (vc <= 0.0f && d1 >= 0.0f && d3 <= 0.0f)
	{
		float v = d1 / (d1 - d3);
		return triangle.p0 + p0_p1 * v;
	}

	// point縺継2縺ｮ螟門・縺ｮ鬆らせ鬆伜沺縺ｮ荳ｭ縺ｫ縺ゅｋ縺九←縺・°繝√ぉ繝・け
	Vec3 p2_pt = point - triangle.p2;

	float d5 = Vec3::Dot(p0_p1, p2_pt);
	float d6 = Vec3::Dot(p0_p2, p2_pt);
	if (d6 >= 0.0f && d5 <= d6)
	{
		return triangle.p2;
	}

	// point縺継0_p2縺ｮ霎ｺ鬆伜沺縺ｮ荳ｭ縺ｫ縺ゅｋ縺九←縺・°繝√ぉ繝・け縺励√≠繧後・point縺ｮp0_p2荳翫↓蟇ｾ縺吶ｋ蟆・ｽｱ繧定ｿ斐☆
	float vb = d5 * d2 - d1 * d6;
	if (vb <= 0.0f && d2 >= 0.0f && d6 <= 0.0f)
	{
		float w = d2 / (d2 - d6);
		return triangle.p0 + p0_p2 * w;
	}

	// point縺継1_p2縺ｮ霎ｺ鬆伜沺縺ｮ荳ｭ縺ｫ縺ゅｋ縺九←縺・°繝√ぉ繝・け縺励√≠繧後・point縺ｮp1_p2荳翫↓蟇ｾ縺吶ｋ蟆・ｽｱ繧定ｿ斐☆
	float va = d3 * d6 - d5 * d4;
	if (va <= 0.0f && (d4 - d3) >= 0.0f && (d5 - d6) >= 0.0f)
	{
		float w = (d4 - d3) / ((d4 - d3) + (d5 - d6));
		return triangle.p1 + (triangle.p2 - triangle.p1) * w;
	}

	float denom = 1.0f / (va + vb + vc);
	float v = vb * denom;
	float w = vc * denom;

	return triangle.p0 + p0_p1 * v + p0_p2 * w;
}

