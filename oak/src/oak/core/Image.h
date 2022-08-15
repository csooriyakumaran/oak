#pragma once

#include "oakpch.h"

namespace Oak
{
	enum class ImageFormat
	{
		None = 0,
		RGBA,
		RGBA32F
	};

	
    static uint32_t BytesPerPixel(ImageFormat format)
    {
        switch (format)
        {
        case ImageFormat::RGBA:    return 4;
        case ImageFormat::RGBA32F: return 16;
        }
        return 0;
    }

	class Image
	{
	public:
		virtual ~Image() = default;

		virtual void SetData(const void* data) = 0;
		virtual void Resize(uint32_t width, uint32_t height) = 0;
		
		virtual uint32_t GetTextureID() const = 0;
		virtual uint32_t GetWidth()     const = 0;
		virtual uint32_t GetHeight()    const = 0;


	};
}