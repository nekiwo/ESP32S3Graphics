#ifndef TEXTURE
#define TEXTURE

#include <vector>
#include <string>

// See:
// - https://docs.espressif.com/projects/esp-idf/en/stable/esp32s3/api-guides/memory-types.html
// - https://dl.espressif.com/public/esp32s3-mm.pdf
enum MemoryType
{
	INTERNAL = 0,
	PSRAM = 1,
};

class Texture
{
  public:
	/**
	 * @brief Returns texture loaded on internal heap or external SPI RAM (PSRAM).
	 *        Caches texture after loading if not allocated on internal heap.
	 *
	 * @param filename
	 * @return Texture*
	 */
	static constexpr Texture* loadTexture(const std::string& filename, MemoryType memory_type)
	{
		return new Texture(filename, memory_type);
	}

	/**
	 * @brief Returns pointer to image
	 *
	 * @param filename
	 * @return Texture*
	 */
	constexpr uint8_t* getData() const
	{
		if (cached_data == nullptr)
		{
			return data;
		}
		else
		{
			return cached_data;
		}
	}

	/**
	 * @brief Returns image size.
	 *
	 * @return bytes/pixels
	 */
	constexpr size_t getSize() const
	{
		return size;
	}

	/**
	 * @brief Returns image width in pixels.
	 *
	 * @return width
	 */
	constexpr uint32_t getWidth() const
	{
		return width;
	}

	/**
	 * @brief Returns image height in pixels.
	 *
	 * @return pixels
	 */
	constexpr uint32_t getHeight() const
	{
		return height;
	}

	/**
	 * @brief Caches this texture into internal memory.
	 *        Adjust size with `MAX_TEXTURE_CACHE_B` constant.
	 *        Cached and deallocated according to loading queue.
	 *
	 */
	void cacheTexture();

	/**
	 * @brief Checks if cached data allocated on internal heap.
	 *
	 * @return true
	 * @return false
	 */
	constexpr bool isCached()
	{
		return cached_data != nullptr;
	}

	/**
	 * @brief Cleans the cache queue.
	 *
	 */
	static void freeAllCache();

  private:
	struct TextureFileHeader
	{
		uint32_t width;
		uint32_t height;
		uint32_t size;
	};

	bool allocated_internally = false;
	uint32_t width = 0;
	uint32_t height = 0;
	uint32_t size = 0;
	uint8_t* data = nullptr;
	uint8_t* cached_data = nullptr;

	static std::vector<Texture*> cached_textures;
	static size_t cache_size;

	Texture(const std::string& filename, MemoryType memory_type);
	~Texture();
	static void freeQueueBack();
};

#endif