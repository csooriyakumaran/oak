#include "oakpch.h"
#include "OpenGLImage.h"

#include "backends/imgui_impl_opengl3.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace Oak {


    static unsigned int OakImageFormatToOpenGLFormat(ImageFormat format)
    {
        switch (format)
        {
        case ImageFormat::RGBA:    return GL_RGBA;
        case ImageFormat::RGBA32F: return GL_RGBA32F;
        }
        return 0;
    }


    OpenGLImage::OpenGLImage(std::string_view path)
        : m_Filepath(path)
    {
        int width, height, channels;
        uint8_t* data = nullptr;

        if (stbi_is_hdr(m_Filepath.c_str()))
        {
            data = (uint8_t*)stbi_loadf(m_Filepath.c_str(), &width, &height, &channels, 4);
            m_Format = ImageFormat::RGBA32F;
        }
        else {
            data = stbi_load(m_Filepath.c_str(), &width, &height, &channels, 4);
            m_Format = ImageFormat::RGBA;
        }

        m_Width = width;
        m_Height = height;

        AllocateMemory(m_Width * m_Height * BytesPerPixel(m_Format));
        SetData(data);
        stbi_image_free(data);
        
    }

    OpenGLImage::~OpenGLImage()
    {
        Release();
    }


    void OpenGLImage::SetData(const void* data)
    {
        unsigned int open_gl_format = OakImageFormatToOpenGLFormat(m_Format);

        // create an OpenGL texture identifier
        glGenTextures(1, &m_TextureID);
        glBindTexture(GL_TEXTURE_2D, m_TextureID);

        // set up filterning for displace
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // this is required on WebGL for non power-of-two textures
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // same

    #if defined(GL_UNPACK_ROW_LENGTH) && !defined(__EMSCRIPTEN__)
        glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
    #endif

        glTexImage2D(GL_TEXTURE_2D, 0, open_gl_format, m_Width, m_Height, 0, open_gl_format, GL_UNSIGNED_BYTE, data);
    }

    void OpenGLImage::Resize(uint32_t width, uint32_t height)
    {
        m_Width = width;
        m_Height = height;

        Release();
        AllocateMemory(m_Width * m_Height * BytesPerPixel(m_Format));
    }

    void OpenGLImage::Release()
    {

    }
    void OpenGLImage::AllocateMemory(uint64_t size)
    {
    }
}