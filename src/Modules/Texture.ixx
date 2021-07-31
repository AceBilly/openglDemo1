module;

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include "../../inc/stb_image.h"

export module Texture;

import <filesystem>;


export namespace Ace {
    namespace fs = std::filesystem;

    // 暂时只处理2D 纹理
    class Texture {
        struct Texture2DInfo {
            int width;
            int height;
            int colorChannel;
        };
    public:
        // 创建一个2D纹理对象，并将将其绑定当前的上下文中。
        Texture() {
            glGenTextures(1, &m_textureId);
            glBindTexture(GL_TEXTURE_2D, m_textureId);
        }
        // 创建并绑定2d纹理并加载纹理图片到内存中
        explicit Texture(const Ace::fs::path& texturePath) : Texture() {
            setTextureParameter();
            load2DTexture(texturePath);
        }
        ~Texture() {
            glDeleteTextures(1, &m_textureId);
        }
    public:
        // 设置纹理参数 默认repeat 线性过滤，边缘处理为指定了边界颜色
        void setTextureParameter() {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            float borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };
            glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
        }
        void bindTextures(int count) const {
            glActiveTexture(GL_TEXTURE0 + count);
            glBindTexture(GL_TEXTURE_2D, m_textureId);
        }
    private:
        // 加载图片 -> 生成opengl2D纹理 之后释放图片资源；
        // texturePath 纹理图片路径
        void load2DTexture(const Ace::fs::path& texturePath) {
            stbi_set_flip_vertically_on_load(true);
            unsigned char* imageData = stbi_load(texturePath.string().c_str(), &m_textureInfo.width, &m_textureInfo.height,
                &m_textureInfo.colorChannel, 0);
            if (!imageData)
                throw std::runtime_error("failed load texture");
            glTexImage2D(GL_TEXTURE_2D, 0, checkImageChannel(), m_textureInfo.width, m_textureInfo.height, 0,
                checkImageChannel(), GL_UNSIGNED_BYTE,
                imageData);
            glGenerateMipmap(GL_TEXTURE_2D);
            stbi_image_free(imageData);
        }
        // 根据stbi 库加载的图片 channel 来转换成opengl 通道参数
        GLint checkImageChannel() const {
            switch (m_textureInfo.colorChannel) {
            case 3:
                return GL_RGB;
            case 4:
                return GL_RGBA;
            default:
                return GL_RGB;
            }
        }

    private:


        unsigned int m_textureId;  // 纹理对象id
        Texture2DInfo m_textureInfo;  // 储存纹理信息 宽高 通道数
    };
}  // namespace ace;
