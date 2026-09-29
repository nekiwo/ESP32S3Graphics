#ifndef VEC3
#define VEC3

#include <cmath>
#include <cstdint>
#include "Vec2.hpp"

template <typename T> struct Vec3
{
  public:
	static inline constexpr Vec3<T> ZERO = {0, 0, 0};

	T x = 0;
	T y = 0;
	T z = 0;

	constexpr float getMagnitude() const
	{
		return std::sqrtf(x * x + y * y + z * z);
	};

	constexpr Vec3 getNormalized() const
	{
		float mag = getMagnitude();
		Vec3 normalized(x / mag, y / mag, z / mag);
		return normalized;
	};

	constexpr Vec3 normalize()
	{
		Vec3 normalized = getNormalized();
		x = normalized.x;
        y = normalized.y;
        z = normalized.z;
        return normalized;
	};

    constexpr Vec3<float> getRotated(float rad) {
        Vec3<float> rotated = {x * std::cosf(rad) - y * std::sinf(rad), x * std::sinf(rad) + y * std::cosf(rad), z};
        return rotated;
    }

    constexpr Vec3<float> rotateBy(float rad) {
        auto rotated = getRotated(rad);
        // printf(
        //     "DEBUG: rad %f rotatedby (%f %f %f) -> (%f %f %f)\n",
        //     rad, x, y, z, rotated.x, rotated.y, rotated.z
        // );
        x = rotated.x;
        y = rotated.y;
        z = rotated.z;
        return rotated;
    }

	// constexpr int32_t cross(const Vec3<int32_t> other) const
	// {
	// 	return x * other.y - y * other.x;
	// };

	constexpr Vec3& operator=(const Vec3& v)
	{
		x = v.x;
		y = v.y;
		z = v.z;
		return *this;
	};

	constexpr const Vec3<int32_t> operator+(const Vec3<int32_t> v) const
	{
		return {x + v.x, y + v.y, z + v.z};
	};

	constexpr const Vec3<int32_t> operator-(const Vec3<int32_t> v) const
	{
		return {x - v.x, y - v.y, z - v.z};
	};

	constexpr const Vec3<float> operator+(const Vec3<float> v) const
	{
		return {x + v.x, y + v.y, z + v.z};
	};

	constexpr const Vec3<float> operator-(const Vec3<float> v) const
	{
		return {x - v.x, y - v.y, z - v.z};
	};

	constexpr Vec3& operator+=(const Vec3& v)
	{
		x += v.x;
		y += v.y;
		z += v.z;
		return *this;
	};

	constexpr Vec3& operator-=(const Vec3& v)
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;
		return *this;
	};

    constexpr Vec3<T>& operator-=(const Vec2<T>& v)
	{
		x -= v.x;
		y -= v.y;
		return *this;
	};

	constexpr Vec3 operator*(const int32_t s)
	{
		return {x * s, y * s, z * s};
	};

	constexpr Vec3<float> operator*(const float s)
	{
		return {x * s, y * s, z * s};
	};

	// constexpr Vec3<float> operator/(const float s)
	// {
	// 	return Vec3<float>(x / s, y / s);
	// };

	constexpr Vec3<float>& operator*=(const float s)
	{
		x *= s;
		y *= s;
		z *= s;
		return *this;
	};

	// constexpr Vec3<float>& operator/=(const float s)
	// {
	// 	x /= s;
	// 	y /= s;
	// 	return *this;
	// };

	// constexpr float operator*(const Vec3& v)
	// {
	// 	// TODO improve
	// 	return x * v.x + y * v.y;
	// };

	constexpr bool operator==(const Vec3& v)
	{
		return x == v.x && y == v.y && z == v.z;
	};

	constexpr bool operator!=(const Vec3& v)
	{
		return !(*this == v);
	};
};

typedef Vec3<float> Vec3f;
typedef Vec3<int32_t> Vec3i;

#endif