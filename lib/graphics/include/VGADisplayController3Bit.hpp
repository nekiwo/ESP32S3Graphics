#ifndef VGA_DISPLAY_CONTROLLER_3BIT
#define VGA_DISPLAY_CONTROLLER_3BIT

#include "ConfigConstants.hpp"
#include "Texture.hpp"
#include "VGADisplayController.hpp"
#include <cstdint>
#include <esp_lcd_panel_ops.h>
#include <esp_lcd_panel_rgb.h>

// Note: CONFIG_SPIRAM_XIP_FROM_PSRAM has been enabled
// See:
// https://docs.espressif.com/projects/esp-idf/en/stable/esp32s3/api-reference/peripherals/lcd/rgb_lcd.html#bounce-buffer-with-single-psram-frame-buffer

/**
 * @brief Simple 3 bit VGA display controller.
 *        Uses 1 pin per color.
 *
 * Sources:
 * - https://docs.espressif.com/projects/esp-idf/en/v5.5.3/esp32s3/api-reference/peripherals/lcd/rgb_lcd.html
 *
 */
class VGADisplayController3Bit : public VGADisplayController
{
  public:
	/**
	 * @brief Initializing VGA controller in "Bounce Buffer with Single PSRAM Frame Buffer" mode.
	 *        See RGB LCD ESP32-S3 ESP-IDF Espressif docs for more information.
	 *        This method simply allocates the object on heap.
	 * https://docs.espressif.com/projects/esp-idf/en/stable/esp32s3/api-reference/peripherals/lcd/rgb_lcd.html#bounce-buffer-with-single-psram-frame-buffer
	 *
	 * @param mode VGA mode
	 */
	static VGADisplayController3Bit* createVGAController(VGAMode mode, uint8_t scale_div);

	/**
	 * @brief Should be run after all drawing is done.
	 *        Upscales the draw buffer frame into active buffer frame.
	 *
	 */
	void show();

  private:
	esp_lcd_panel_handle_t panel_handle = NULL;

	VGADisplayController3Bit(VGAMode mode, uint8_t scale_div);
	static bool bounceBufFillEvent(esp_lcd_panel_handle_t panel, void* bounce_buf, int pos_px, int len_bytes,
								   void* user_ctx);
	static bool placeholderEvent(esp_lcd_panel_handle_t panel, const esp_lcd_rgb_panel_event_data_t* edata,
								 void* user_ctx);
};

#endif