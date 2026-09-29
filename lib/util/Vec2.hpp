#ifndef VEC2
#define VEC2

#include <cmath>
#include <cstdint>

template <typename T> struct Vec2
{
  public:
	static inline constexpr Vec2<T> ZERO = {0, 0};
    
	T x;
	T y;

	constexpr float getMagnitude() const
	{
		// TODO improve
		return std::sqrtf(x * x + y * y);
	};

	constexpr Vec2 getNormalized() const
	{
		// TODO improve
		float mag = getMagnitude();
		Vec2 normalized(x / mag, y / mag);
		return normalized;
	};

	constexpr Vec2 normalize()
	{
		Vec2 normalized = getNormalized();
		x = normalized.x;
		y = normalized.y;
		return normalized;
	};

    constexpr Vec2<float> getRotated(float rad) {
        Vec2<float> rotated = {x * std::cosf(rad) - y * std::sinf(rad), x * std::sinf(rad) + y * std::cosf(rad)};
        return rotated;
    }

    constexpr Vec2<float> rotateBy(float rad) {
        auto rotated = getRotated(rad);
        x = rotated.x;
        y = rotated.y;
        return rotated;
    }

	constexpr int32_t cross(const Vec2<int32_t> other) const
	{
		return x * other.y - y * other.x;
	};

	constexpr Vec2& operator=(const Vec2& v)
	{
		x = v.x;
		y = v.y;
		return *this;
	};

	constexpr const Vec2<int32_t> operator+(const Vec2<int32_t> v) const
	{
		return {(int32_t)(x + v.x), (int32_t)(y + v.y)};
	};

	constexpr const Vec2<int32_t> operator-(const Vec2<int32_t> v) const
	{
		return {(int32_t)(x - v.x), (int32_t)(y - v.y)};
	};

	constexpr const Vec2<float> operator+(const Vec2<float> v) const
	{
		return {x + v.x, y + v.y};
	};

	constexpr const Vec2<float> operator-(const Vec2<float> v) const
	{
		return {x - v.x, y - v.y};
	};

	constexpr Vec2& operator+=(const Vec2& v)
	{
		x += v.x;
		y += v.y;
		return *this;
	};

	constexpr Vec2& operator-=(const Vec2& v)
	{
		x -= v.x;
		y -= v.y;
		return *this;
	};

	// constexpr Vec2 operator*(const int32_t s)
	// {
	// 	return Vec2(x * s, y * s);
	// };

	constexpr Vec2<float> operator*(const float s)
	{
		return Vec2<float>(x * s, y * s);
	};

	// constexpr Vec2<float> operator/(const float s)
	// {
	// 	return Vec2<float>(x / s, y / s);
	// };

	constexpr Vec2<float>& operator*=(const float s)
	{
		x *= s;
		y *= s;
		return *this;
	};

	// constexpr Vec2<float>& operator/=(const float s)
	// {
	// 	x /= s;
	// 	y /= s;
	// 	return *this;
	// };

	// constexpr float operator*(const Vec2& v)
	// {
	// 	// TODO improve
	// 	return x * v.x + y * v.y;
	// };

	constexpr bool operator==(const Vec2& v)
	{
		return x == v.x && y == v.y;
	};

	constexpr bool operator!=(const Vec2& v)
	{
		return !(*this == v);
	};
};

typedef Vec2<float> Vec2f;
typedef Vec2<int32_t> Vec2i;

#endif