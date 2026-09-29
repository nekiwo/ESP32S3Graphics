#include "Texture.hpp"
#include "esp_heap_caps.h"
#include <ConfigConstants.hpp>
#include <cstring>
#include <fstream>

size_t Texture::cache_size = 0;
std::vector<Texture*> Texture::cached_textures{};

Texture::Texture(const std::string& filename, MemoryType memory_type)
{
	std::ifstream file(filename);
	if (!file.is_open())
	{
		printf("DEBUG: Could not open file.\n");
	}

	char header_buffer[sizeof(TextureFileHeader)];
	file.read(header_buffer, sizeof(TextureFileHeader));
	auto header = reinterpret_cast<TextureFileHeader*>(header_buffer);

	width = header->width;
	height = header->height;
	size = width * height;

	switch (memory_type)
	{
	case INTERNAL:
		data = (uint8_t*)malloc(sizeof(uint8_t) * header->size);
		allocated_internally = true;
		break;
	case PSRAM:
		data = (uint8_t*)heap_caps_malloc(sizeof(uint8_t) * header->size, MALLOC_CAP_SPIRAM);
		break;
	default:
		data = nullptr;
		break;
	}

	if (data == nullptr)
	{
		printf("DEBUG: Allocating texture failed.\n");
	}

	file.read(reinterpret_cast<char*>(data), header->size);

	if (!allocated_internally)
	{
		cacheTexture();
	}
}

Texture::~Texture()
{
    // TODO remove from queue
    // TODO free cache
    // TODO free original
}

void Texture::cacheTexture()
{
	if (allocated_internally || cached_data != nullptr)
	{
		return;
	}

	if (cache_size + size > MAX_TEXTURE_CACHE_B)
	{
		freeQueueBack();
	}

	cached_data = (uint8_t*)malloc(size);
	memcpy(cached_data, data, size);

	cached_textures.push_back(this);
	cache_size += size;
}

void Texture::freeAllCache()
{
	while (cached_textures.size() > 0)
	{
		freeQueueBack();
	}
}

void Texture::freeQueueBack()
{
    // TODO REWRITE

	auto first_texture = cached_textures.front();
	free(first_texture->cached_data);
	first_texture->cached_data = nullptr;
	cache_size -= first_texture->size;
    cached_textures.erase(cached_textures.begin());
}