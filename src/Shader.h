//
// Created by yhtse on 7/16/2021.
//
#ifndef SHADER_H_
#define SHADER_H_

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <map>
//#include <boost/filesystem.hpp>
#include <glad/glad.h>
#include <initializer_list>
#include "Error.h"

#include "../inc/glm/glm/glm.hpp"
#include "../inc/glm/glm/gtc/matrix_transform.hpp"
#include "../inc/glm/glm/gtc/type_ptr.hpp"


namespace Ace {
    namespace fs = std::filesystem;
    class Shader {
    public:
        Shader(const fs::path &vertexShader, const fs::path &fragShader);

        ~Shader() {}

    public:
        // 出现不能查询到变量地址的问题
        // TODO(AceBilly) bug
        void setValue(const std::string &var, float value1);
        void setValue(const std::string &var, int value);

        void setValue(const std::string& var, float (&value)[4]);

        void setValue(const std::string &varName, std::initializer_list<float> vec4);
        void setValue(const std::string &varName, const glm::mat4 mat);

        // 需要指定变量位置的文件
        void getVariables(const fs::path &shaderPath);

    public:
        void use() const {
            glUseProgram(m_id);
        }

        [[nodiscard]] const unsigned int getId() const { return m_id; }

    private:
        int m_id;  // shader program id;:
        std::map<std::string, int> m_variables;
    };
}

#endif  // shader_h_