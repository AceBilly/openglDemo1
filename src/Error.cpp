#include "Error.h"


void Ace::Error::shaderError(unsigned int id, bool flag, const std::source_location location) {
    int success;
    char infoLog[512];
    if (flag) {
        glGetShaderiv(id, GL_COMPILE_STATUS, &success);
    } else {
        glGetProgramiv(id, GL_LINK_STATUS, &success);
    }
    if (!success) {
        std::cout << "file_name: " << location.file_name() << ":\n"
                  << "line: " << location.line() << "\n \t"
                  << "function: " << location.function_name() << '\n';
        if (flag) {
            glGetShaderInfoLog(id, 512, nullptr, infoLog);
            std::cout << "Error:Shader vertex compilation_failed " << infoLog << std::endl;
        } else {
            glGetProgramInfoLog(id, 512, nullptr, infoLog);
            std::cout << "Error::program link failed " << infoLog << std::endl;
        }
    }
}

void Ace::Error::getOpenglError() {
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
