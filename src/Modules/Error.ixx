// 这个项目的错误处理模块
module;

#include <glad/glad.h>

export module Error;

import <iostream>;
import <source_location>;

export namespace Ace {
    std::source_location current(const uint_least32_t _Line_ = __builtin_LINE(),
        const uint_least32_t _Column_ = __builtin_COLUMN(), const char* const _File_ = __builtin_FILE(),
        const char* const _Function_ = __builtin_FUNCTION()) noexcept {
        return std::source_location(_Line_, _Column_, _File_, _Function_);
    }
            // 只处理编译Shader 和 链接程序额错误：暂时
    void shaderError(unsigned int id, bool flag = true, const std::source_location location = current()) {
            int success;
            char infoLog[512];
            if (flag) {
                glGetShaderiv(id, GL_COMPILE_STATUS, &success);
            }
            else {
                glGetProgramiv(id, GL_LINK_STATUS, &success);
            }
            if (!success) {
                std::cout << "file_name: " << location.file_name() << ":\n"
                    << "line: " << location.line() << "\n \t"
                    << "function: " << location.function_name() << '\n';
                if (flag) {
                    glGetShaderInfoLog(id, 512, nullptr, infoLog);
                    std::cout << "Error:Shader vertex compilation_failed " << infoLog << std::endl;
                }
                else {
                    glGetProgramInfoLog(id, 512, nullptr, infoLog);
                    std::cout << "Error::program link failed " << infoLog << std::endl;
                }
            }
        }
        void getOpenglError() {
            auto error_code = glGetError();
            switch (error_code) {
            case GL_NO_ERROR:
                std::cout << "no error" << std::endl;
                break;
            case GL_INVALID_ENUM:
                std::cerr << "error code not invalid" << '\n';
                break;
            case GL_INVALID_VALUE:
                std::cerr << "invalid value" << std::endl;
                break;
            case GL_INVALID_OPERATION:
                std::cerr << "invalid operation" << '\n';
                break;
            case GL_INVALID_FRAMEBUFFER_OPERATION:
                std::cerr << "invalid framebuffer operation" << '\n';
                break;
            case GL_OUT_OF_MEMORY:
                std::cerr << "out of memory" << '\n';
                break;
            case GL_STACK_UNDERFLOW:
                std::cerr << "stack underlow" << '\n';
                break;
            case GL_STACK_OVERFLOW:
                std::cerr << "stack overlow" << '\n';
                break;
            default:
                std::cerr << "umatch emum value" << std::endl;
                break;
            }
        }
}  // namespace ace;
