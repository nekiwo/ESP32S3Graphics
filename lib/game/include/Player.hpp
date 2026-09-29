#ifndef PLAYER
#define PLAYER

#include <Vec2.hpp>

class Player
{
  public:
	constexpr Player() {};

	constexpr void setForce(const Vec2f& force)
	{
		acceleration = force;
	}

	constexpr void setRotationalVelocity(float rot_vel)
	{
		xy_rotation_velocity = rot_vel;
	}

	void update(float dt);

	constexpr Vec2f getPosition() const
	{
		return position;
	}

	constexpr Vec2f getVelocity() const
	{
		return velocity;
	}

	constexpr Vec2f getAcceleration() const
	{
		return acceleration;
	}

	constexpr float getHeading() const
	{
		return xy_rotation;
	}

  private:
	Vec2f position;
	Vec2f velocity;
	Vec2f acceleration;
	float xy_rotation = 0.0f;
	float xy_rotation_velocity = 0.0f;
};

#endif