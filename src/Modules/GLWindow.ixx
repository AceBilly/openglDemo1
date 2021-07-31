module;

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "../../inc/glm/glm/glm.hpp"
#include "../../inc/glm/glm/gtc/matrix_transform.hpp"
#include "../../inc/glm/glm/gtc/type_ptr.hpp"

export module GLWindow;

import<memory>;
import<initializer_list>;
import<iostream>;
import<vector>;
import<filesystem>;

import Texture;
import utility.type;
import Shader;
import Error;

export namespace Ace {
    namespace fs = std::filesystem;
    class GLWindow {
    public:
        using window_t = GLFWwindow;
    public:
        GLWindow(uint width, uint height, const std::string& winTitle);
        ~GLWindow() {
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

        // ���ɲ�������ֵ�󶨵�VBO
        struct AssistStruct {
            int location;  // ����ֵ�ڵ�λ��
            int size;  // ����ֵ�������еĲ���
        };

        template<typename T, size_t N>
        void generateVBO(T(&data)[N], std::initializer_list<AssistStruct> locationAndSize) {
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
            for (auto [location, size] : locationAndSize) {
                // TODO(AceBilly) Type not dect
                glVertexAttribPointer(location, size, GL_FLOAT, GL_FALSE, len * sizeof(T), (void*)(offset * sizeof(T)));
                //                Ace::Error::getOpenglError();
                offset += size;
                glEnableVertexAttribArray(location);
            }
        }

        template<typename T, size_t N>
        requires std::is_signed<T>::value || std::is_unsigned<T>::value
            void generateEBO(T(&indices)[N]) {
            m_vertexCount = N;
            glGenBuffers(1, &m_EBO);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
        }

        // ����2D ����
        void load2DTexture(const Ace::fs::path& texturePath);
        // \params texturePath ����·��λ�ã� ��shader�б��������֣�
        void load2DTexture(const Ace::fs::path& texturePath, const std::string& textureUniformName);

        // ����׿��ɫ����uniform ����λ�õ���Ϣ��json�ļ�
        void loadShader(const Ace::fs::path& vertexShaderPath, const Ace::fs::path& fragShader,
            const Ace::fs::path& variablesLocationConfig);

    private:
        // �����ⲿ�����¼�
        void processInput()const;
        // ��������
        void createWindow(uint width, uint height, const std::string& winTitle);
        // ���ڸ��Ĵ��ڴ�С��ʱ��Ļص�����
        // TODO ��������ʵ���ı���ͬ��
        static void frameBuffer_size_callback(window_t* window, int width, int height);
        // ������ɫ��Ϣ
        struct BackgroundColor {
            float r;
            float g;
            float b;
            float a;
        };
        void reFlashBuffer();
    private:
        BackgroundColor m_background;
        window_t* m_window = nullptr;
        std::unique_ptr<Ace::Shader> mp_shader = nullptr;
        using texture_pt = std::shared_ptr<Ace::Texture>;
        unsigned int m_EBO, m_VAO, m_VBO;
        std::vector<texture_pt> m_textures;
        unsigned int m_vertexCount = 0;
        int m_windowHeight, m_windowWidth;

        // TODO �������refactor
    private:
        static float lastX;
        static float lastY;
        static bool firstMouse;

        // TODO ��Ʒ�����
    };

    // initializer static variables of class GLWindows;
    float Ace::GLWindow::lastX = 800 / 2.0f;
    float Ace::GLWindow::lastY = 800 / 2.0f;
    bool Ace::GLWindow::firstMouse = true;

}  // namespace Ace;
