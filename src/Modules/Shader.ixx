module;

#include "../../inc/glm/glm/glm.hpp"
#include "../../inc/glm/glm/gtc/matrix_transform.hpp"
#include  "../../inc/glm/glm/gtc/type_ptr.hpp"
#include "../../inc/rapidjson/document.h"
#include <glad/glad.h>

export module Shader;

import <iostream>;
import <string>;
import <filesystem>;
import <map>;
import <initializer_list>;

import utility;
import Error;

export namespace Ace {
    namespace fs = std::filesystem;
    class Shader {
    public:
        // ���ļ��ж�ȡ��ɫ��Դ�� ������ ����ͷ���ɫ����Դ
        // \param vertexShader ������ɫ���ļ�·��
        // \param fragShader   Ƭ����ɫ���ļ�·��
        Shader(const fs::path& vertexShader, const fs::path& fragShader) {
            // ��ȡ�ļ� ���Ż�
            std::string fragShaderCodeStr = Ace::readFile(fragShader);
            std::string vertexShaderCodeStr = Ace::readFile(vertexShader);

            const char* vertexShaderCode = vertexShaderCodeStr.c_str();
            const char* fragShaderCode = fragShaderCodeStr.c_str();

            uint vertexId = glCreateShader(GL_VERTEX_SHADER);
            glShaderSource(vertexId, 1, &vertexShaderCode, nullptr);
            glCompileShader(vertexId);
            Ace::shaderError(vertexId, true);

            uint fragmentId = glCreateShader(GL_FRAGMENT_SHADER);
            glShaderSource(fragmentId, 1, &fragShaderCode, nullptr);
            glCompileShader(fragmentId);
            Ace::shaderError(fragmentId, true);

            m_id = glCreateProgram();
            glAttachShader(m_id, vertexId);
            glAttachShader(m_id, fragmentId);
            glLinkProgram(m_id);
            Ace::shaderError(m_id, false);
            use();
            glDeleteShader(vertexId);
            glDeleteShader(fragmentId);
        }

        ~Shader() {}

    public:
        // ���ֲ��ܲ�ѯ��������ַ������
        // TODO(AceBilly) bug
        // ������ɫ������ֵ
        void setValue(const std::string& var, float value1) {
            try {
                int location = m_variables.at(var);
                glUniform1f(location, value1);
            }
            catch (std::exception& e) {
                std::cout << "set value failed: " << e.what() << '\n';
            }

        }
        void setValue(const std::string& var, int value) {
            try {
                int location = m_variables.at(var);
                glUniform1i(location, value);
            }
            catch (std::exception& e) {
                std::cout << "failed : " << e.what() << '\n';
            }
        }
        // 
        void setValue(const std::string& var, float(&value)[4]) {
            try {
                auto [v1, v2, v3, v4] = value;
                glUniform4f(m_variables.at(var), v1, v2, v3, v4);
            }
            catch (std::exception& e) {
                std::cerr << "failed set variables" << e.what() << '\n';
            }
        }
        // vec4;
        void setValue(const std::string& varName,
            std::initializer_list<float> vec4) {
            if (vec4.size() == 4) {
                auto begin = vec4.begin();
                glUniform4f(m_variables.at(varName), *(begin), *(++begin),
                    *(++begin), *(++begin));
            }
            else
                std::cerr << "please ensure your data is right";
        }
        void setValue(const std::string& varName, const glm::mat4 mat) {
            try {
                int location = m_variables.at(varName);
                glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(mat));
            }
            catch (std::exception& e) {
                std::cerr << "failed : " << e.what() << '\n';
            }
        }

        // ��Ҫָ������λ�õ��ļ�
        void getVariables(const fs::path& shaderPath) {
            try {
                if (fs::exists(shaderPath)) {
                    std::string shaderSrc = readFile(shaderPath);

                    rapidjson::Document document;
                    document.Parse(shaderSrc.c_str());

                    for (auto& entry : document.GetObject()) {
                        // ���string��key��
                        std::string variablesName = entry.name.GetString();
                        int location = entry.value.GetInt();

                        m_variables[variablesName] = location;
                    }
                }
            }
            catch (std::exception& e) {
                std::cout << "map failed: " << e.what() << std::endl;
            }
        }

    public:
        // ʹ����ɫ��
        void use() const {
            glUseProgram(m_id);
        }
        // ������ɫ������ID
        [[nodiscard]] const unsigned int getId() const { return m_id; }

    private:
        int m_id;  // shader program id;:
        std::map<std::string, int> m_variables;  // uniform ������ location ֮���ӳ��
    };  // class shader;
}  // namespace ace;
