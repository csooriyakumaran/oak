#pragma once

#include "oakpch.h"
#include "oak/core/Image.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"


namespace Oak {

	class OpenGLImage : public Image
	{
	public:
		OpenGLImage(std::string_view path);
		virtual ~OpenGLImage();

		void SetData(const void* data) override;
		void Resize(uint32_t width, uint32_t height) override;

		uint32_t GetTextureID() const override{ return m_TextureID; }
		uint32_t GetWidth() const override { return m_Width; }
		uint32_t GetHeight() const override { return m_Height; }

	private:
		void AllocateMemory(uint64_t size);
		void Release();

	private:

		uint32_t m_Width = 0, m_Height = 0;
		uint32_t m_TextureID;

		ImageFormat m_Format = ImageFormat::None;
		std::string m_Filepath;

	};

}