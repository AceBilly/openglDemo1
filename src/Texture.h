//
// Created by yhtse on 7/22/2021.
//

#ifndef VULKAN_DEMO_TEXTURE_H
#define VULKAN_DEMO_TEXTURE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "utility.h"

namespace Ace {
    struct Texture2DInfo {
        int width;
        int height;
        int colorChannel;
    };
    // 暂时只处理2D 纹理
    class Texture {
    public:
        Texture();
        explicit Texture(const Ace::fs::path &texturePath);
        ~Texture(){
            glDeleteTextures(1, &m_textureId);
        }
    public:
        // 设置纹理参数，如过滤模式，渐进模式
        void setTextureParameter();
        void bindTextures(int count) const {
            glActiveTexture(GL_TEXTURE0 + count);
            glBindTexture(GL_TEXTURE_2D, m_textureId);
        }
    private:
        // 加载图片 -> 生成opengl纹理
        void load2DTexture(const Ace::fs::path& texturePath);
        GLint checkImageChannel() const;

    private:
        unsigned int m_textureId;
        Texture2DInfo m_textureInfo;
    };
}


#endif //VULKAN_DEMO_TEXTURE_H
