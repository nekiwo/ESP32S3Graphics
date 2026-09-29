#ifndef CONFIG_MANAGER
#define CONFIG_MANAGER

#define _USE_MATH_DEFINES
#include <cstdint>
#include <cmath>

// Don't use GPIO 23, 19, 18, 5 (VSPI)
// Can't use 8-bit DAC (pins 25, 26) because not enough
// Available pins:
// (4, 12, 13, 14), (15, 16, 17, 21), (22, 25, 26, 27), 32, 33
// inline constexpr uint8_t RED_PINS[] = {4, 12, 13, 14};
// inline constexpr uint8_t GREEN_PINS[] = {15, 16, 17, 18};
// inline constexpr uint8_t BLUE_PINS[] = {22, 25, 26, 27};
// inline constexpr uint8_t VSYNC_PIN = 32;
// inline constexpr uint8_t HSYNC_PIN = 33;

// Physical parameters
inline constexpr uint8_t RED_PIN = 7;
inline constexpr uint8_t GREEN_PIN = 6;
inline constexpr uint8_t BLUE_PIN = 5;
inline constexpr uint8_t HSYNC_PIN = 16; // Orange wire
inline constexpr uint8_t VSYNC_PIN = 15; // Yellow wire

// DMA optimizations
inline constexpr uint32_t MAX_INTERNAL_BUFFER_SIZE_B = 300000;
inline constexpr uint32_t MAX_TEXTURE_CACHE_B = 50000;

// Rendering
inline constexpr uint8_t SCALE_DIV = 2;
inline constexpr float FOV = 60.0f * M_PI / 180.0f;

// Game constants
inline constexpr float WALL_HEIGHT = 3.0f;
inline constexpr float PLAYER_HEIGHT = 1.5f;
inline constexpr float PLAYER_FRICTION = 1.0f;

#endif