#include "Player.hpp"
#include "ConfigConstants.hpp"

void Player::update(float dt)
{
    acceleration.rotateBy(-xy_rotation);

	velocity += acceleration * dt;
	velocity *= (1.0f - PLAYER_FRICTION * dt);
	position += velocity * dt;
	acceleration = Vec2f::ZERO;

	xy_rotation += xy_rotation_velocity * dt;
	if (xy_rotation < 0.0f)
	{
		xy_rotation = 2.0f * M_PI + xy_rotation;
	}
	else if (xy_rotation > 2.0f * M_PI)
	{
		xy_rotation -= 2.0f * M_PI;
	}
}