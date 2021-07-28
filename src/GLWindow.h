//
// Created by yhtse on 7/21/2021.
//

#ifndef VULKAN_DEMO_GLWINDOW_H
#define VULKAN_DEMO_GLWINDOW_H


#include <memory>
#include <initializer_list>
#include <functional>
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include "Shader.h"
#include "Texture.h"
#include "Camer.h"

#include "../inc/glm/glm/glm.hpp"
#include "../inc/glm/glm/gtc/matrix_transform.hpp"
#include "../inc/glm/glm/gtc/type_ptr.hpp"



namespace Ace {
    class GLWindow {
    public:
        using window_t = GLFWwindow;
    public:
        GLWindow(uint width, uint height, const std::string &winTitle);
        ~GLWindow(){
            glDeleteBuffers(1, &m_EBO);
            glDeleteBuffers(1, &m_VBO);
            glfwTerminate();
        }
        void render();
    public:
        void getWindowSize();
    public:
        void generateVao();

        void setBackgroundColor(float r, float g, float b, float a);

        // 生成并将属性值绑定到VBO
        struct AssistStruct {
            int location;  // 属性值在的位置
            int size;  // 属性值在数组中的步长
        };

        template<typename T, size_t N>
        void generateVBO(T (&data)[N], std::initializer_list<AssistStruct> locationAndSize) {
            auto calcLen = [&locationAndSize]()->int {
                int res = 0;
                for (const auto[locatoin, size] : locationAndSize) {
                    res += size;
                }
                return res;
            };
            int len  = calcLen();
            glGenBuffers(1, &m_VBO);
            glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);
            unsigned int offset = 0;
            for (auto[location, size] : locationAndSize) {
                // TODO(AceBilly) Type not dect
                glVertexAttribPointer(location, size, GL_FLOAT, GL_FALSE, len * sizeof(T), (void *) (offset * sizeof(T)));
//                Ace::Error::getOpenglError();
                offset += size;
                glEnableVertexAttribArray(location);
            }
        }

        template<typename T, size_t N>
        requires std::is_signed<T>::value || std::is_unsigned<T>::value
        void generateEBO(T (&indices)[N]) {
            m_vertexCount = N;
            glGenBuffers(1, &m_EBO);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
        }

        // 加载2D 纹理
        void load2DTexture(const Ace::fs::path &texturePath);
        // \params texturePath 纹理路径位置， 在shader中变量的名字；
        void load2DTexture(const Ace::fs::path &texturePath, const std::string& textureUniformName);

       // 编译卓着色器和uniform 变量位置的信息的json文件
        void loadShader(const Ace::fs::path &vertexShaderPath, const Ace::fs::path &fragShader,
                        const Ace::fs::path &variablesLocationConfig);

    private:
        // 处理外部输入事件
        void processInput()const;
        // 创建窗口
        void createWindow(uint width, uint height, const std::string &winTitle);
        // 用于更改窗口大小的时候的回调函数
        // TODO 不能与类实例的变量同步
        static void frameBuffer_size_callback(window_t *window, int width, int height);
        // 储存颜色信息
        struct BackgroundColor{
            float r;
            float g;
            float b;
            float a;
        };
        void reFlashBuffer();
    private:
        BackgroundColor m_background;
        window_t *m_window = nullptr;
        std::unique_ptr<Ace::Shader> mp_shader = nullptr;
        using texture_pt = std::shared_ptr<Ace::Texture>;
        unsigned int m_EBO, m_VAO, m_VBO;
        std::vector<texture_pt> m_textures;
        unsigned int m_vertexCount = 0;
        int m_windowHeight, m_windowWidth;

        // TODO 摄像机类refactor
    private:
        static float lastX;
        static float lastY;
        static bool firstMouse;

        // TODO 设计非良构
    };

}  // namespace Ace;



#endif //VULKAN_DEMO_GLWINDOW_H
