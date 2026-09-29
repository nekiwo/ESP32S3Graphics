#include "VGADisplayController3Bit.hpp"
#include "esp_heap_caps.h"
#include <cstring>

VGADisplayController3Bit* VGADisplayController3Bit::createVGAController(VGAMode mode, uint8_t scale_div)
{
	return new VGADisplayController3Bit(mode, scale_div);
}

void VGADisplayController3Bit::show()
{
	for (uint32_t draw_line_index = 0; draw_line_index < mode.height / scale_div; draw_line_index++)
	{
		auto active_line = active_frame_buffer + (draw_line_index * scale_div) * mode.width;
		auto draw_line = draw_frame_buffer + draw_line_index * (mode.width / scale_div);

		// Copy over one line
		for (uint16_t draw_pixel_index = 0; draw_pixel_index < mode.width / scale_div; draw_pixel_index++)
		{
			memset(active_line + draw_pixel_index * scale_div, *(draw_line + draw_pixel_index), scale_div);
		}

		// Duplicate the line
		for (uint8_t dup_line_index = 1; dup_line_index < scale_div; dup_line_index++)
		{
			auto dup_line = active_line + mode.width * dup_line_index;
			memcpy(dup_line, active_line, mode.width);
		}
	}
}

VGADisplayController3Bit::VGADisplayController3Bit(VGAMode mode, uint8_t scale_div)
	: VGADisplayController(mode, scale_div)
{
	esp_lcd_rgb_panel_config_t panel_config = {// Should be either 480/2 or 320/2 Mhz depending on PLL frequency
											   // divided by two (?)
											   .clk_src = LCD_CLK_SRC_PLL240M,
											   .timings = {.pclk_hz = mode.pixel_clock,
														   .h_res = mode.width,
														   .v_res = mode.height,
														   .hsync_pulse_width = mode.h_sync_pulse,
														   .hsync_back_porch = mode.h_back_porch,
														   .hsync_front_porch = mode.h_front_porch,
														   .vsync_pulse_width = mode.v_sync_pulse,
														   .vsync_back_porch = mode.v_back_porch,
														   .vsync_front_porch = mode.v_front_porch,
														   .flags =
															   {
																   .hsync_idle_low = 0,
																   .vsync_idle_low = 0,
																   .de_idle_high = 0,
																   .pclk_active_neg = 1,
																   .pclk_idle_high = 0,
															   }},
											   .data_width = 8,
											   .bits_per_pixel = 8,
											   .num_fbs = 0,
											   .bounce_buffer_size_px =
												   (uint32_t)mode.width * 60, // Unsure what size to use
											   .sram_trans_align = 0,
											   .psram_trans_align = 64,
											   // .dma_burst_size = 16,
											   .hsync_gpio_num = HSYNC_PIN,
											   .vsync_gpio_num = VSYNC_PIN,
											   .de_gpio_num = -1,
											   .pclk_gpio_num = -1,
											   .disp_gpio_num = -1,
											   .data_gpio_nums = {-1, -1, -1, -1, -1, BLUE_PIN, GREEN_PIN, RED_PIN},
											   .flags = {.disp_active_low = 0,
														 .refresh_on_demand = 0,
														 .fb_in_psram = 0,
														 .double_fb = 0,
														 .no_fb = 1,
														 .bb_invalidate_cache = 0}};

	// Initiate the panel object
	esp_lcd_new_rgb_panel(&panel_config, &panel_handle);

	// Allocating frame buffers manually on controller's PSRAM, of which there's 8MB
	active_frame_buffer = (uint8_t*)heap_caps_malloc(total_pixels, MALLOC_CAP_SPIRAM | MALLOC_CAP_DMA);

	// Allocates on internal memory if possible, otherwise PSRAM
	if (total_draw_pixels <= MAX_INTERNAL_BUFFER_SIZE_B)
	{
		draw_frame_buffer = (uint8_t*)malloc(total_draw_pixels);
	}
	else
	{
		draw_frame_buffer = (uint8_t*)heap_caps_malloc(total_draw_pixels, MALLOC_CAP_SPIRAM | MALLOC_CAP_DMA);
	}

	if (active_frame_buffer == nullptr || draw_frame_buffer == nullptr)
	{
		printf("DEBUG: Mallocing frame buffers failed (PSRAM problem).\n");
	}

	// White background
	drawBackground(0xE0);

	esp_lcd_rgb_panel_event_callbacks_t callbacks = {.on_color_trans_done = placeholderEvent,
													 .on_vsync = placeholderEvent,
													 .on_bounce_empty = bounceBufFillEvent,
													 .on_frame_buf_complete = placeholderEvent};

	esp_lcd_rgb_panel_register_event_callbacks(panel_handle, &callbacks, this);
	esp_lcd_panel_reset(panel_handle);
	esp_lcd_panel_init(panel_handle);
}

bool VGADisplayController3Bit::bounceBufFillEvent(esp_lcd_panel_handle_t panel, void* bounce_buf, int pos_px,
												  int len_bytes, void* user_ctx)
{
	auto vga = reinterpret_cast<VGADisplayController3Bit*>(user_ctx);
	auto dest_bounce_buffer = reinterpret_cast<uint8_t*>(bounce_buf);

	uint32_t bb_start_line_index = pos_px / vga->mode.width;

	// Vertical sync blank space
	if (bb_start_line_index >= vga->mode.height)
	{
		memset(bounce_buf, 0x00, len_bytes);
		return true;
	}

	// Copy over lines for entire bounce buffer
	uint8_t* active_fb_line = vga->active_frame_buffer + (bb_start_line_index * vga->mode.width);
	memcpy(dest_bounce_buffer, active_fb_line, len_bytes);

	return true;
}

bool VGADisplayController3Bit::placeholderEvent(esp_lcd_panel_handle_t panel,
												const esp_lcd_rgb_panel_event_data_t* edata, void* user_ctx)
{
	return true;
}