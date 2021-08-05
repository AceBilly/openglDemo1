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

    // ��ʼ��glfw ָ��oepngl �汾 4.6 core ģʽ
    void initialize_glfw() {
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    }
    // ��ʼ�� glad
    void initialize_glad() {
        // ��ʼ��glad����ָ��
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
            throw std::runtime_error("failed initialize glad");
    }

    export class GLWindow {
    public:
        using window_t = GLFWwindow;
    public:
        // �������ڲ���ʼ��opengl ���� �󶨵�ǰ���ڵ���ǰ�߳�
        GLWindow(uint width, uint height, const std::string& winTitle) {
            initialize_glfw();
            createWindow(width, height, winTitle);
            initialize_glad();
            // �����ӿ� ��3dģʽ
            glViewport(0, 0, width, height);
            glEnable(GL_DEPTH_TEST);
            // ���ڱ仯�ص�����
            glfwSetFramebufferSizeCallback(m_window, &frameBuffer_size_callback);
            glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }

        // �ͷ� ���������Դ ������Դ
        virtual ~GLWindow() {
            glDeleteBuffers(1, &m_EBO);
            glDeleteBuffers(1, &m_VBO);
            glfwTerminate();
        }
        // ��Ⱦ���� �� ����
        virtual void render() {
            // �����ʮ��������
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
                    // TODO �����￪һ��������̣߳����������̻߳���̸�����ֵ������ �����ܴ��۴�
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
        // ���´��ڴ�С
        void reflushWindowsSize() {
            glfwGetWindowSize(m_window, &m_windowWidth, &m_windowHeight);
        }
    public:
        // ���ɶ����������
        void generateVao() {
            glGenVertexArrays(1, &m_VAO);
            glBindVertexArray(m_VAO);
        }
        // ���ñ�����ɫ
        // @param r g b a -> redColor green blue alpha ����
        // ����ɫֵ���浽����ʵ���У� ������֮���ˢ��
        void setBackgroundColor(float r, float g, float b, float a) {
            m_background.r = r;
            m_background.g = g;
            m_background.b = b;
            m_background.a = a;

            glClearColor(r, g, b, a);
            // ��������� ��ֹ��Ⱦ3d ͼ�γ��ֲ������Ľ��
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        }

        
        // ���� һ�� VBO ���󣬲����������ݰ󶨵��������У�
        template<typename T, size_t N>
        void generateVBO(T(&data)[N], std::initializer_list<AssistStruct> locationAndSize) {
            // ������ һ�����������ܲ���
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
            // �Զ��������е�����
            for (auto [location, size] : locationAndSize) {
                // TODO(AceBilly) Type not dect
                glVertexAttribPointer(location, size, GL_FLOAT, GL_FALSE, len * sizeof(T), (void*)(offset * sizeof(T)));
                //                Ace::Error::getOpenglError();
                offset += size;
                glEnableVertexAttribArray(location);
            }
        }
        // �������������������
        // @param indices ���㻺������
        // TODO ������Լ�����ƣ� Ӧ��Ҫ��Ϊ��Ȼ��
        template<typename T, size_t N>
        requires std::is_signed<T>::value || std::is_unsigned<T>::value
            void generateEBO(T(&indices)[N]) {
            m_vertexCount = N;
            glGenBuffers(1, &m_EBO);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
        }

        // ����2D ����
        void load2DTexture(const Ace::fs::path& texturePath) {
            m_textures.push_back(std::make_shared<Ace::Texture>(texturePath));
        }
        // \params texturePath ����·��λ�ã� ��shader�б��������֣�
        void load2DTexture(const Ace::fs::path& texturePath, const std::string& textureUniformName) {
            thread_local int textureCount = 0;  //  ���� ��������
            load2DTexture(texturePath);
            mp_shader->setValue(textureUniformName, textureCount);
            ++textureCount;
        }

        // ����׿��ɫ����uniform ����λ�õ���Ϣ��json�ļ�
        void loadShader(const Ace::fs::path& vertexShaderPath, const Ace::fs::path& fragShaderPath,
            const Ace::fs::path& variablesLocationConfig) {
            mp_shader = std::make_unique<Ace::Shader>(vertexShaderPath, fragShaderPath);
            mp_shader->getVariables(variablesLocationConfig);
        }

    private:
        // �����ⲿ�����¼�
        void processInput()const;
        // ��������
        // @param width ���ڿ�� ��λpx
        // @param height ���ڸ߶�
        // @param winTitle ���ڱ���
        void createWindow(uint width, uint height, const std::string& winTitle) {
            m_window = glfwCreateWindow(width, height, winTitle.c_str(), nullptr, nullptr);
            if (!m_window)
                throw std::runtime_error("failed create window!");
            glfwMakeContextCurrent(m_window);
        }
        // ���ڸ��Ĵ��ڴ�С��ʱ��Ļص�����
        // TODO ��������ʵ���ı���ͬ��
        static void frameBuffer_size_callback(window_t* window, int width, int height) {
            glViewport(0, 0, width, height);
        }
        
        // ˢ�»���
        void reflushBuffer() {
            auto [r, g, b, a] = m_background;
            setBackgroundColor(r, g, b, a);
        }
    private:
        BackgroundColor m_background;  // ���ڱ�����ɫ
        window_t* m_window = nullptr;  // ����ָ��
        std::unique_ptr<Ace::Shader> mp_shader = nullptr;  //��ɫ������ָ��
        using texture_pt = std::shared_ptr<Ace::Texture>;  // �����������
        unsigned int m_EBO, m_VAO, m_VBO;  // �������� ���㻺�壬 ����
        std::vector<texture_pt> m_textures;  // ��������
        unsigned int m_vertexCount = 0;  // ���ڼ���һ����Ҫ���ƶ��ٸ�����
        int m_windowHeight, m_windowWidth;  // ��ǰ���ڸ߶� ���

        // TODO �����
    };

}  // namespace Ace;
