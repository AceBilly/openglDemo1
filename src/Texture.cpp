//
// Created by yhtse on 7/22/2021.
//

#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION

#include "../inc/stb_image.h"

Ace::Texture::Texture() {
    glGenTextures(1, &m_textureId);
    glBindTexture(GL_TEXTURE_2D, m_textureId);
}

void Ace::Texture::setTextureParameter() {
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    float borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
}

void Ace::Texture::load2DTexture(const Ace::fs::path &texturePath) {
    stbi_set_flip_vertically_on_load(true);
    unsigned char *imageData = stbi_load(texturePath.c_str(), &m_textureInfo.width, &m_textureInfo.height,
                                         &m_textureInfo.colorChannel, 0);
    if (!imageData)
        throw std::runtime_error("failed load texture");
    glTexImage2D(GL_TEXTURE_2D, 0, checkImageChannel(), m_textureInfo.width, m_textureInfo.height, 0,
                 checkImageChannel(), GL_UNSIGNED_BYTE,
                 imageData);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(imageData);
}

Ace::Texture::Texture(const Ace::fs::path &texturePath) : Texture() {
    setTextureParameter();
    load2DTexture(texturePath);
}

GLint Ace::Texture::checkImageChannel() const {
    switch (m_textureInfo.colorChannel) {
        case 3:
            return GL_RGB;
        case 4:
            return GL_RGBA;
        default:
            return GL_RGB;
    }
}
