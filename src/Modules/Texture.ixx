module;

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include "../../inc/stb_image.h"

export module Texture;

import <filesystem>;


export namespace Ace {
    namespace fs = std::filesystem;

    // ��ʱֻ����2D ����
    class Texture {
        struct Texture2DInfo {
            int width;
            int height;
            int colorChannel;
        };
    public:
        // ����һ��2D������󣬲�������󶨵�ǰ���������С�
        Texture() {
            glGenTextures(1, &m_textureId);
            glBindTexture(GL_TEXTURE_2D, m_textureId);
        }
        // ��������2d������������ͼƬ���ڴ���
        explicit Texture(const Ace::fs::path& texturePath) : Texture() {
            setTextureParameter();
            load2DTexture(texturePath);
        }
        ~Texture() {
            glDeleteTextures(1, &m_textureId);
        }
    public:
        // ����������� Ĭ��repeat ���Թ��ˣ���Ե����Ϊָ���˱߽���ɫ
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
        // ����ͼƬ -> ����opengl2D���� ֮���ͷ�ͼƬ��Դ��
        // texturePath ����ͼƬ·��
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
        // ����stbi ����ص�ͼƬ channel ��ת����opengl ͨ������
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


        unsigned int m_textureId;  // �������id
        Texture2DInfo m_textureInfo;  // ����������Ϣ ��� ͨ����
    };
}  // namespace ace;
