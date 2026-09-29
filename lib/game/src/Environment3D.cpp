#include "Environment3D.hpp"
#include "esp_timer.h"

Environment3D::Environment3D(VGADisplayController3Bit* display) : player(), display(display)
{
}

void Environment3D::addStaticWall(const Wall& wall)
{
	walls.push_back(wall);
}

void Environment3D::render()
{
	auto curr_time = esp_timer_get_time();
	dt = (curr_time - last_frame_update) / 1000000.0f;
	last_frame_update = curr_time;

	display->drawBackground(0x00);

	for (auto& wall : walls)
	{
		renderWall(wall);
	}

	player.update(dt);

	display->show();
}

constexpr Vec2f project3DTo2D(Vec3f point3d)
{
	return {point3d.x / point3d.y, point3d.z / point3d.y};
}

constexpr Vec2i point2DToScreen(Vec2f point2d, float fov_ratio, Vec2f screen_half)
{
	auto scalar_x = screen_half.x / fov_ratio;
	auto scalar_y = screen_half.y / fov_ratio;
	Vec2i screen_point = {(int32_t)((fov_ratio + point2d.x) * scalar_x), (int32_t)((fov_ratio - point2d.y) * scalar_y)};

	return screen_point;
}

void Environment3D::renderWall(const Wall& wall) const
{
	// testing
	float fov_ratio = std::tanf(FOV / 2.0f);

	Vec3f wall_points[4] = {
		{wall.p1.x, wall.p1.y, wall.height},
		{wall.p2.x, wall.p2.y, wall.height},
		{wall.p2.x, wall.p2.y, 0.0f},
		{wall.p1.x, wall.p1.y, 0.0f},
	};

	for (auto& point : wall_points)
	{
		transformToPlayerSpace(point);
		if (point.y < 0.01f)
		{
			return;
		}
	}

	Vec2f screen_half = {(float)(640 / SCALE_DIV / 2), (float)(480 / SCALE_DIV / 2)};

	QuadCoords corners = {
		point2DToScreen(project3DTo2D(wall_points[0]), fov_ratio, screen_half),
		point2DToScreen(project3DTo2D(wall_points[1]), fov_ratio, screen_half),
		point2DToScreen(project3DTo2D(wall_points[2]), fov_ratio, screen_half),
		point2DToScreen(project3DTo2D(wall_points[3]), fov_ratio, screen_half),
	};

	if (wall.type == Wall::WallType::COLOR)
	{
		display->drawSolidQuad(corners, wall.value.color);
	}
	else if (wall.type == Wall::WallType::TEXT)
	{
		display->drawTextureQuad(corners, wall.value.texture);
	}
}