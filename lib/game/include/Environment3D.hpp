#ifndef ENVIRONMENT_3D
#define ENVIRONMENT_3D

#include "Player.hpp"
#include "VGADisplayController3Bit.hpp"
#include "Vec2.hpp"
#include "Vec3.hpp"
#include <vector>

class Environment3D
{
  public:
	struct Wall
	{
		Vec2f p1;
		Vec2f p2;
		float height;
        enum WallType { COLOR, TEXT } type;
        union {
            Texture* texture;
            uint8_t color;
        } value;
	};

	Player player;

	Environment3D(VGADisplayController3Bit* display);
	void addStaticWall(const Wall& wall);
	void render();

  private:
	VGADisplayController3Bit* display = nullptr;
	int64_t last_frame_update = 0;
	float dt = 1.0f; // In seconds
	std::vector<Wall> walls;

	constexpr void transformToPlayerSpace(Vec3f& v) const
	{
		v -= player.getPosition();
        v.z -= PLAYER_HEIGHT;
        v.rotateBy(player.getHeading());
	}

	void renderWall(const Wall& wall) const;
};

#endif