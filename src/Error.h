//
// Created by yhtse on 7/17/2021.
//

#ifndef VULKAN_DEMO_ERROR_H
#define VULKAN_DEMO_ERROR_H
// 这个宏是因为目前编译器中出现 未定义这个宏 而导致 Source Locatio 类而没有被定义
#define __has_builtin(__builtin_source_location) true
#include <source_location>

#include <glad/glad.h>
#include <iostream>

namespace Ace {
    class Error {
    public:
        // 只处理编译Shader 和 链接程序额错误：暂时
        static void shaderError(unsigned int id, bool flag = true, const std::source_location location = std::source_location::current());
        static void getOpenglError();
    };
}


#endif //VULKAN_DEMO_ERROR_H
