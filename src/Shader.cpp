//
// Created by yhtse on 7/16/2021.
//

#include "Shader.h"


#include <vector>
#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>

using namespace std::string_literals;

// readfile
// \param filePath 文件路径
// \returns {std::string} 返回包含文件内容的string
std::string readFile(const Ace::fs::path &filePath) {
    try {
        if (!Ace::fs::exists(filePath)) {
            throw std::runtime_error("file path not exists!");
        }
        std::string res;
        std::ifstream file;
        file.open(filePath);
        std::stringstream fileStream;
        fileStream << file.rdbuf();
        res = fileStream.str();
        return res;
    } catch (std::exception &e) {
        std::cout << "failed read file: " << e.what() << std::endl;
    }
    return ""s;
}

// 从文件中读取着色器源码 并编译 最后释放着色器资源
// \param vertexShader 顶点着色器文件路径
// \param fragShader   片段着色器文件路径
Ace::Shader::Shader(const fs::path &vertexShader, const fs::path &fragShader) {
    // 读取文件 待优化
    std::string fragShaderCodeStr = readFile(fragShader);
    std::string vertexShaderCodeStr = readFile(vertexShader);

    const char *vertexShaderCode = vertexShaderCodeStr.c_str();
    const char *fragShaderCode = fragShaderCodeStr.c_str();

    uint vertexId = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexId, 1, &vertexShaderCode, nullptr);
    glCompileShader(vertexId);
    Ace::Error::shaderError(vertexId, true);

    uint fragmentId = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentId, 1, &fragShaderCode, nullptr);
    glCompileShader(fragmentId);
    Ace::Error::shaderError(fragmentId, true);

    m_id = glCreateProgram();
    glAttachShader(m_id, vertexId);
    glAttachShader(m_id, fragmentId);
    glLinkProgram(m_id);
    Ace::Error::shaderError(m_id, false);
    use();
    glDeleteShader(vertexId);
    glDeleteShader(fragmentId);
}

void Ace::Shader::getVariables(const fs::path &shaderPath) {
    try {

        if (fs::exists(shaderPath)) {
            std::string shaderSrc = readFile(shaderPath);

            rapidjson::Document document;
            document.Parse(shaderSrc.c_str());

            for (auto &entry : document.GetObject()) {

                // 这个string是key，
                std::string variablesName = entry.name.GetString();
                int location = entry.value.GetInt();
//            m_variables.insert(std::make_pair(variablesName, location));

//                m_variables.insert({variablesName, location});
                m_variables[variablesName] = location;
            }
        }
    } catch (std::exception &e) {
        std::cout << "map failed: " << e.what() << std::endl;
    }
}

void Ace::Shader::setValue(const std::string &varName, std::initializer_list<float> vec4) {
    if (vec4.size() == 4) {
        auto begin = vec4.begin();
        glUniform4f(m_variables.at(varName), *(begin), *(++begin), *(++begin), *(++begin));
    } else
        std::cerr << "please ensure your data is right";

}

void Ace::Shader::setValue(const std::string &var, float value1) {
//    glUniform1f(glGetUniformLocation(m_id, var.c_str()), value1);
//    int location = glGetUniformLocation(m_id, var.c_str());
    int location = m_variables.at(var);
    glUniform1f(location, value1);
}

// TODO(AceBilly) 无法查询变量位置
void Ace::Shader::setValue(const std::string &var, float (&values)[4]) {
//    int location = glGetUniformLocation(m_id, var.c_str());
    try {
        auto[v1, v2, v3, v4] = values;
        int location = m_variables.at(var);
        glUniform4f(location, v1, v2, v3, v4);
    } catch (std::exception &e) {
        std::cout << "failed 121: " << e.what() << std::endl;
    }
}

void Ace::Shader::setValue(const std::string &var, int value) {
    try {
        int location = m_variables.at(var);
        glUniform1i(location, value);
    } catch(std::exception& e) {
        std::cout << "failed : " << e.what() << '\n';
    }
}

void Ace::Shader::setValue(const std::string &varName, const glm::mat4 mat) {
    try {
        int location = m_variables.at(varName);
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(mat));
    } catch(std::exception& e) {
        std::cerr << "failed : " << e.what() << '\n';
    }
}
