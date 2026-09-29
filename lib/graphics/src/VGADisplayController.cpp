#include "VGADisplayController.hpp"
#include "esp_heap_caps.h"
#include "freertos/FreeRTOS.h"
#include <Vec2.hpp>
#include <cmath>
#include <cstring>
#include <functional>
#include <ConfigConstants.hpp>

constexpr bool rightOfLine(Vec2i p1, Vec2i p2, Vec2i point)
{
	// Dot product to perpendicular p2 - p1 line
	return (point.x - p1.x) * (p2.y - p1.y) + (point.y - p1.y) * (p1.x - p2.x) >= 0;
}

constexpr bool pointInsideQuad(const QuadCoords& corners, Vec2i point)
{
	bool b0 = rightOfLine(corners.c0, corners.c1, point);
	bool b1 = rightOfLine(corners.c1, corners.c2, point);
	bool b2 = rightOfLine(corners.c2, corners.c3, point);
	bool b3 = rightOfLine(corners.c3, corners.c0, point);

	return b0 == b1 && b1 == b2 && b2 == b3;
}

constexpr Vec2f inverseBiLerp(Vec2i a, Vec2i b, Vec2i c, Vec2i d, Vec2i p)
{
	// Inverse bilinear interpolation over four points of a quad
	// From: https://iquilezles.org/articles/ibilinear/

	Vec2i e = b - a;
	Vec2i f = d - a;
	Vec2i g = a - b + c - d;
	Vec2i h = p - a;

    // TEST
    if (g.x == 0) {
        g.x = 1;
    }
    if (g.y == 0) {
        g.y = 1;
    }

	float k2 = g.cross(f);
	float k1 = e.cross(f) + h.cross(g);
	float k0 = h.cross(e);

	float w = k1 * k1 - 4.0f * k0 * k2;

    // Needed because bugs with detecting pixel inside quad
    if (k2 == 0.0f) {
        return {0.5f, 0.5f};
    }

    // Needed because bugs with detecting pixel inside quad
	if (w < 0.0f)
	{
		// return {-1.0f, -1.0f};
        return {0.5f, 0.5f};
	}
	w = std::sqrtf(w);

	float denominator0 = 2.0f * k2;
	float v = (-k1 - w) / denominator0;
	float u = (h.x - f.x * v) / (e.x + g.x * v);

	if (v < 0.0f || v > 1.0f || u < 0.0f || u > 1.0f)
	{
		v = (-k1 + w) / denominator0;
		u = (h.x - f.x * v) / (e.x + g.x * v);
	}

    // Needed because bugs with detecting pixel inside quad
	if (v < 0.0f || v > 1.0f || u < 0.0f || u > 1.0f)
	{
		// return {-1.0f, -1.0f};
        return {0.5f, 0.5f};
	}

	return {u, v};
}

constexpr void screenBoundCheck(int16_t& x, int16_t& y)
{
    x = x > 0 ? x : 0;
    y = y > 0 ? y : 0;

    x = x < 640 / SCALE_DIV ? x : 640 / SCALE_DIV;
    y = y < 480 / SCALE_DIV ? y : 480 / SCALE_DIV;
}

template <typename Func> void VGADisplayController::drawShaderQuad(const QuadCoords& corners, Func shaderFunc)
{
	int16_t y_min = std::min(corners.c0.y, std::min(corners.c1.y, std::min(corners.c2.y, corners.c3.y)));
	int16_t y_max = std::max(corners.c0.y, std::max(corners.c1.y, std::max(corners.c2.y, corners.c3.y)));

	int16_t x_min = std::min(corners.c0.x, std::min(corners.c1.x, std::min(corners.c2.x, corners.c3.x)));
	int16_t x_max = std::max(corners.c0.x, std::max(corners.c1.x, std::max(corners.c2.x, corners.c3.x)));

    screenBoundCheck(x_min, y_min);
    screenBoundCheck(x_max, y_max);

	for (int16_t y = y_min; y < y_max; y++)
	{
		for (int16_t x = x_min; x < x_max; x++)
		{
			if (pointInsideQuad(corners, {x, y}))
			{
				Vec2f uv = inverseBiLerp(corners.c0, corners.c1, corners.c2, corners.c3, {x, y});
				drawPixel(x, y, shaderFunc(uv.x, uv.y));
			}
		}
	}
}

void VGADisplayController::drawSolidRect(uint16_t x, uint16_t y, uint16_t width, uint8_t height, uint8_t color)
{
	if (draw_frame_buffer != nullptr)
	{
		for (uint16_t line = y; line < y + height; line++)
		{
			memset(draw_frame_buffer + (mode.width / scale_div) * line + x, color, width);
		}
	}
}

void VGADisplayController::drawTextureRect(uint16_t x, uint16_t y, const Texture* texture)
{
	auto texture_data = texture->getData();

	for (uint16_t image_line_index = 0; image_line_index < texture->getHeight(); image_line_index++)
	{
		auto image_line = texture_data + image_line_index * texture->getWidth();
		auto display_line = draw_frame_buffer + (mode.width / scale_div) * (y + image_line_index) + x;
		memcpy(display_line, image_line, texture->getWidth());
	}
}

void VGADisplayController::drawSolidQuadSlow(const QuadCoords& corners, uint8_t color)
{
	drawShaderQuad(corners, [color](float u, float v) { return color; });
}

void VGADisplayController::drawSolidQuad(const QuadCoords& corners, uint8_t color)
{
	// TODO

	// split quad into top/bottom triangles
	// memset fill row by row

	drawSolidQuadSlow(corners, color);
}

void VGADisplayController::drawTextureQuad(const QuadCoords& corners, const Texture* texture)
{
	auto texture_data = texture->getData();
	auto line_width = texture->getWidth();
	auto height = texture->getHeight();

	drawShaderQuad(corners, [texture_data, line_width, height](float u, float v) {
		uint16_t texture_x = (uint16_t)(u * line_width);
		uint16_t texture_y = (uint16_t)(v * height);
		uint8_t color = texture_data[texture_y * line_width + texture_x];
		return color;
	});
}

void VGADisplayController::drawBackground(uint8_t color)
{
	memset(draw_frame_buffer, color, total_draw_pixels);
}