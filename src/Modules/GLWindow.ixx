module;
#include <iostream>
#include <initializer_list>
#include <memory>
#include <vector>
#include <filesystem>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "../../inc/glm/glm/glm.hpp"
#include "../../inc/glm/glm/gtc/matrix_transform.hpp"
#include "../../inc/glm/glm/gtc/type_ptr.hpp"

export module GLWindow;
export import :AssisantType;  


import Texture;
import utility.type;
import Shader;
import Error;

namespace Ace {
    namespace fs = std::filesystem;

    // 初始化glfw 指定oepngl 版本 4.6 core 模式
    void initialize_glfw() {
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    }
    // 初始化 glad
    void initialize_glad() {
        // 初始化glad函数指针
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
            throw std::runtime_error("failed initialize glad");
    }

    export class GLWindow {
    public:
        using window_t = GLFWwindow;
    public:
        // 创建窗口并初始化opengl 函数 绑定当前窗口到当前线程
        GLWindow(uint width, uint height, const std::string& winTitle) {
            initialize_glfw();
            createWindow(width, height, winTitle);
            initialize_glad();
            // 设置视口 打开3d模式
            glViewport(0, 0, width, height);
            glEnable(GL_DEPTH_TEST);
            // 窗口变化回调函数
            glfwSetFramebufferSizeCallback(m_window, &frameBuffer_size_callback);
            glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }

        // 释放 缓冲对象资源 窗口资源
        virtual ~GLWindow() {
            glDeleteBuffers(1, &m_EBO);
            glDeleteBuffers(1, &m_VBO);
            glfwTerminate();
        }
        // 渲染进程 非 良构
        virtual void render() {
            // 另外的十个正方体
            glm::vec3 cubePositions[] = {
                    glm::vec3(0.0f, 0.0f, 0.0f),
                    glm::vec3(2.0f, 5.0f, -15.0f),
                    glm::vec3(-1.5f, -2.2f, -2.5f),
                    glm::vec3(-3.8f, -2.0f, -12.3f),
                    glm::vec3(2.4f, -0.4f, -3.5f),
                    glm::vec3(-1.7f, 3.0f, -7.5f),
                    glm::vec3(1.3f, -2.0f, -2.5f),
                    glm::vec3(1.5f, 2.0f, -2.5f),
                    glm::vec3(1.5f, 0.2f, -1.5f),
                    glm::vec3(-1.3f, 1.0f, -1.5f)
            };

            while (!glfwWindowShouldClose(m_window)) {
                reflushBuffer();
                if (m_textures.size()) {
                    int count = 0;
                    for (auto& texture : m_textures) {
                        texture->bindTextures(count);
                        ++count;
                    }
                }
                if (mp_shader) {
                    mp_shader->use();
                    reflushWindowsSize();
                    // TODO 在这里开一个服务端线程，接受其他线程或进程更改数值的请求 （性能代价大）
                    glm::mat4 view = glm::mat4(1.0f);
                    glm::mat4 projection = glm::mat4(1.0f);
                    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
                    projection = glm::perspective(glm::radians(45.0f), (float)m_windowWidth / (float)m_windowHeight, 0.1f, 100.0f);;
                    mp_shader->setValue("view", view);
                    mp_shader->setValue("projection", projection);
                }

                glBindVertexArray(m_VAO);
                for (auto& cubePosition : cubePositions) {
                    glm::mat4 model = glm::mat4(1.0f);
                    model = glm::translate(model, cubePosition);
                    model = glm::rotate(model, (float)(glfwGetTime()), glm::vec3(1.0f, 0.3f, 0.5f));
                    mp_shader->setValue("model", model);

                    glDrawArrays(GL_TRIANGLES, 0, 36);
                }
                //        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
                //        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                //        glDrawElements(GL_TRIANGLES, m_vertexCount, GL_UNSIGNED_INT, 0);

                glfwSwapBuffers(m_window);
                glfwPollEvents();
                //        Ace::Error::getOpenglError();  // debug

            }
        }
    public:
        // 更新窗口大小
        void reflushWindowsSize() {
            glfwGetWindowSize(m_window, &m_windowWidth, &m_windowHeight);
        }
    public:
        // 生成顶点数组对象
        void generateVao() {
            glGenVertexArrays(1, &m_VAO);
            glBindVertexArray(m_VAO);
        }
        // 设置背景颜色
        // @param r g b a -> redColor green blue alpha 分量
        // 将颜色值储存到对象实例中， 用于在之后的刷新
        void setBackgroundColor(float r, float g, float b, float a) {
            m_background.r = r;
            m_background.g = g;
            m_background.b = b;
            m_background.a = a;

            glClearColor(r, g, b, a);
            // 启用了深度 防止渲染3d 图形出现不正常的结果
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        }

        
        // 创建 一个 VBO 对象，并将属性数据绑定到缓冲区中；
        template<typename T, size_t N>
        void generateVBO(T(&data)[N], std::initializer_list<AssistStruct> locationAndSize) {
            // 计算其 一条属性数据总步长
            auto calcLen = [&locationAndSize]()->int {
                int res = 0;
                for (const auto [locatoin, size] : locationAndSize) {
                    res += size;
                }
                return res;
            };
            int len = calcLen();
            glGenBuffers(1, &m_VBO);
            glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);
            unsigned int offset = 0;
            // 自动启动其中的数据
            for (auto [location, size] : locationAndSize) {
                // TODO(AceBilly) Type not dect
                glVertexAttribPointer(location, size, GL_FLOAT, GL_FALSE, len * sizeof(T), (void*)(offset * sizeof(T)));
                //                Ace::Error::getOpenglError();
                offset += size;
                glEnableVertexAttribArray(location);
            }
        }
        // 创建并绑定索引缓冲对象
        // @param indices 顶点缓冲数据
        // TODO 类型制约不完善， 应该要求为自然数
        template<typename T, size_t N>
        requires std::is_signed<T>::value || std::is_unsigned<T>::value
            void generateEBO(T(&indices)[N]) {
            m_vertexCount = N;
            glGenBuffers(1, &m_EBO);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
        }

        // 加载2D 纹理
        void load2DTexture(const Ace::fs::path& texturePath) {
            m_textures.push_back(std::make_shared<Ace::Texture>(texturePath));
        }
        // \params texturePath 纹理路径位置， 在shader中变量的名字；
        void load2DTexture(const Ace::fs::path& texturePath, const std::string& textureUniformName) {
            thread_local int textureCount = 0;  //  计数 纹理数量
            load2DTexture(texturePath);
            mp_shader->setValue(textureUniformName, textureCount);
            ++textureCount;
        }

        // 编译卓着色器和uniform 变量位置的信息的json文件
        void loadShader(const Ace::fs::path& vertexShaderPath, const Ace::fs::path& fragShaderPath,
            const Ace::fs::path& variablesLocationConfig) {
            mp_shader = std::make_unique<Ace::Shader>(vertexShaderPath, fragShaderPath);
            mp_shader->getVariables(variablesLocationConfig);
        }

    private:
        // 处理外部输入事件
        void processInput()const;
        // 创建窗口
        // @param width 窗口宽度 单位px
        // @param height 窗口高度
        // @param winTitle 窗口标题
        void createWindow(uint width, uint height, const std::string& winTitle) {
            m_window = glfwCreateWindow(width, height, winTitle.c_str(), nullptr, nullptr);
            if (!m_window)
                throw std::runtime_error("failed create window!");
            glfwMakeContextCurrent(m_window);
        }
        // 用于更改窗口大小的时候的回调函数
        // TODO 不能与类实例的变量同步
        static void frameBuffer_size_callback(window_t* window, int width, int height) {
            glViewport(0, 0, width, height);
        }
        
        // 刷新缓存
        void reflushBuffer() {
            auto [r, g, b, a] = m_background;
            setBackgroundColor(r, g, b, a);
        }
    private:
        BackgroundColor m_background;  // 窗口背景颜色
        window_t* m_window = nullptr;  // 窗口指针
        std::unique_ptr<Ace::Shader> mp_shader = nullptr;  //着色器对象指针
        using texture_pt = std::shared_ptr<Ace::Texture>;  // 纹理对象类型
        unsigned int m_EBO, m_VAO, m_VBO;  // 索引缓冲 顶点缓冲， 顶点
        std::vector<texture_pt> m_textures;  // 储存纹理
        unsigned int m_vertexCount = 0;  // 用于计数一共需要绘制多少个顶点
        int m_windowHeight, m_windowWidth;  // 当前窗口高度 宽度

        // TODO 摄像机
    };

}  // namespace Ace;
