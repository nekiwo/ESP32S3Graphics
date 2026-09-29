#include "VGADisplayController3Bit.hpp"
#include "esp_littlefs.h"
#include "freertos/FreeRTOS.h"
#include <Debug.hpp>
#include <esp_timer.h>
#include <stdio.h>
#include <Environment3D.hpp>
#include <driver/gpio.h>

VGADisplayController3Bit* vga = nullptr;
Texture* test_image = nullptr;
Environment3D* world = nullptr;

void setup()
{
	// LittleFS setup
	esp_vfs_littlefs_conf_t conf = {
		.base_path = "/data",
		.partition_label = "littlefs",
		.format_if_mount_failed = true,
		.dont_mount = false,
	};
	esp_vfs_littlefs_register(&conf);

	vga = VGADisplayController3Bit::createVGAController(MODE_640X480, SCALE_DIV);
	test_image = Texture::loadTexture("/data/textures/test.tx3b", INTERNAL);
    world = new Environment3D(vga);

    Environment3D::Wall walls[] = {
        {
            {0.0f, 3.0f},
            {3.0f, 3.0f},
            WALL_HEIGHT,
            Environment3D::Wall::WallType::TEXT,
            {.texture = test_image}
        },
        {
            {3.0f, 2.0f},
            {3.0f, -2.0f},
            WALL_HEIGHT,
            Environment3D::Wall::WallType::COLOR,
            {.color = 0x80}
        },
        {
            {0.0f, -3.0f},
            {3.0f, -3.0f},
            WALL_HEIGHT,
            Environment3D::Wall::WallType::COLOR,
            {.color = 0x80}
        }
    };

    for (auto& wall : walls) {
        world->addStaticWall(wall);
    }

    // input testing
    // 21 right
    // 47 left
    // 48 forward
    gpio_config_t io_conf0 = {
        .pin_bit_mask = (1ULL << 21), 
        .mode = GPIO_MODE_INPUT,              
        .pull_up_en = GPIO_PULLUP_ENABLE,     
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE        
    };

    gpio_config_t io_conf1 = {
        .pin_bit_mask = (1ULL << 21), 
        .mode = GPIO_MODE_INPUT,              
        .pull_up_en = GPIO_PULLUP_ENABLE,     
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE        
    };

    gpio_config_t io_conf2 = {
        .pin_bit_mask = (1ULL << 21), 
        .mode = GPIO_MODE_INPUT,              
        .pull_up_en = GPIO_PULLUP_ENABLE,     
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE        
    };

    gpio_config(&io_conf0);
    gpio_config(&io_conf1);
    gpio_config(&io_conf2);
}

uint32_t x, y = 0;

void loop()
{
	uint32_t start_draw_time = esp_timer_get_time();

	// QuadCoords corners = {{20 / SCALE_DIV, 20 / SCALE_DIV},
	// 					  {400 / SCALE_DIV, 30 / SCALE_DIV},
	// 					  {300 / SCALE_DIV, 350 / SCALE_DIV},
	// 					  {40 / SCALE_DIV, 440 / SCALE_DIV}};

	// QuadCoords corners2 = {{520 / SCALE_DIV, 380 / SCALE_DIV},
	// 					   {551 / SCALE_DIV, 178 / SCALE_DIV},
	// 					   {435 / SCALE_DIV, 46 / SCALE_DIV},
	// 					   {407 / SCALE_DIV, 212 / SCALE_DIV}};

	// vga->drawTextureQuad(corners, test_image);
	// vga->drawTextureQuad(corners2, test_image);

	// vga->drawSolidQuadSlow(corners, 0x80);
	// vga->drawSolidQuadSlow(corners2, 0x80);

	// vga->drawSolidQuad(corners, 0x80);
	// vga->drawSolidQuad(corners2, 0x80);

    // test input
    // printf(
    //     "DEBUG: io forward: %d left: %d right: %d\n",
    //     gpio_get_level(GPIO_NUM_48),
    //     gpio_get_level(GPIO_NUM_47),
    //     gpio_get_level(GPIO_NUM_21)
    // );

    Vec2f force = {
        // 3.0f * (float)(-gpio_get_level(GPIO_NUM_47) + gpio_get_level(GPIO_NUM_21)),
        0.0f,
        8.0f * (float)gpio_get_level(GPIO_NUM_48)
    };

    float rot_vel = 8.0f * 0.261799387799f * (float)(-gpio_get_level(GPIO_NUM_47) + gpio_get_level(GPIO_NUM_21));

    world->player.setForce(force);
    world->player.setRotationalVelocity(rot_vel);

    printf(
        "DEBUG: player pos: (%f %f)\n",
        world->player.getPosition().x, world->player.getPosition().y
    );

	uint32_t end_draw_time = esp_timer_get_time();

	// vga->show();
    world->render();

	uint32_t end_copy_time = esp_timer_get_time();

	// === TESTING ===

	printf("draw dt ms: %lu\tcopy dt ms: %lu\ttotal dt ms: %lu\n", (end_draw_time - start_draw_time) / 1000,
		   (end_copy_time - end_draw_time) / 1000, (end_copy_time - start_draw_time) / 1000);

	// vTaskDelay(5000 / portTICK_PERIOD_MS);
	// printTasks();

	// === TESTING ===
}

// Source: https://docs.espressif.com/projects/esp-idf/en/v5.5.3/esp32/api-reference/system/esp_event.html
extern "C" void app_main()
{
	vTaskDelay(4000 / portTICK_PERIOD_MS);

	// Arduino style loop controls
	setup();
	while (true)
	{
		loop();
		vTaskDelay(10 / portTICK_PERIOD_MS);
	}
}
