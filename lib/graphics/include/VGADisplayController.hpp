#ifndef VGA_DISPLAY_CONTROLLER
#define VGA_DISPLAY_CONTROLLER

#include "Texture.hpp"
#include <cstdint>
#include <Vec2.hpp>

// Vertical and horizontal sync polarity assumed both positive
struct VGAMode
{
	uint32_t pixel_clock;

	uint16_t width;
	uint16_t h_front_porch;
	uint16_t h_sync_pulse;
	uint16_t h_back_porch;

	uint16_t height;
	uint16_t v_front_porch;
	uint16_t v_sync_pulse;
	uint16_t v_back_porch;
};

// 640x480 @60Hz full resolution
// Pixel clock should be 25175000 but for some reason that creates tearing and artifacts
inline constexpr VGAMode MODE_640X480 = {25000000, 640, 16, 96, 48, 480, 11, 2, 31};

// 800x600 @60Hz but divided by 4
// Doesn't seem to work
inline constexpr VGAMode MODE_200X150 = {10000000, 200, 10, 32, 22, 150, 0, 1, 5};

// 640x480 @60Hz but divided by 8
// Doesn't seem to work
inline constexpr VGAMode MODE_80X60 = {3146875, 80, 2, 12, 6, 60, 1, 0, 4};

struct QuadCoords
{
	Vec2i c0;
	Vec2i c1;
	Vec2i c2;
	Vec2i c3;
};

/**
 * @brief Base class for a VGA controller that supports up to 8-bit color.
 *        Use the `VGADisplayController3Bit` instead.
 *
 */
class VGADisplayController
{
  public:
	/**
	 * @brief Sets a single pixel of frame.
	 *
	 * @param x screen space pixels
	 * @param y screen space pixels
	 * @param color depends on implementation
	 */
	inline constexpr void drawPixel(uint32_t x, uint32_t y, uint8_t color)
	{
		draw_frame_buffer[x + (mode.width / scale_div) * y] = color;
	}

	/**
	 * @brief Fills specified region with one color.
	 *
	 * @param x screen space pixels
	 * @param y screen space pixels
	 * @param width screen space pixels
	 * @param height screen space pixels
	 * @param color depends on implementation
	 */
	void drawSolidRect(uint16_t x, uint16_t y, uint16_t width, uint8_t height, uint8_t color);

	/**
	 * @brief Copies over texture directly to draw frame.
	 *
	 * @param x screen space pixels
	 * @param y screen space pixels
	 * @param texture loaded texture
	 */
	void drawTextureRect(uint16_t x, uint16_t y, const Texture* texture);

	/**
	 * @brief Fills quad polygon with solid color.
     *        Uses shader draw base.
	 *
	 * @param corners clockwise from top left
     * @param color
	 */
	void drawSolidQuadSlow(const QuadCoords& corners, uint8_t color);

    /**
     * @brief Fills quad with solid color.
     * 
     * @param corners clockwise from top left
     * @param color 
     */
    void drawSolidQuad(const QuadCoords& corners, uint8_t color);

	/**
	 * @brief Uses perspective correct texture mapping from a square texture.
	 *
	 * @param corners clockwise from top left
	 * @param texture loaded texture
	 */
	void drawTextureQuad(const QuadCoords& corners, const Texture* texture);

	/**
	 * @brief Draws solid color across entire image.
	 *
	 * @param color depends on implementation
	 */
	void drawBackground(uint8_t color);

	/**
	 * @brief Should be run after all drawing is done.
	 *
	 */
	virtual void show() = 0;

  protected:
	const VGAMode mode;
	const uint8_t scale_div;
	const uint32_t total_pixels;
	const uint32_t total_draw_pixels;

	uint8_t* active_frame_buffer = nullptr;
	uint8_t* draw_frame_buffer = nullptr;

	constexpr VGADisplayController(VGAMode mode, uint8_t scale_div)
		: mode(mode), scale_div(scale_div), total_pixels(mode.width * mode.height),
		  total_draw_pixels(mode.width * mode.height / (scale_div * scale_div))
	{
	}

	template <typename Func> void drawShaderQuad(const QuadCoords& corners, Func func);
};

#endif