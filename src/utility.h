//
// Created by yhtse on 7/22/2021.
//

#ifndef VULKAN_DEMO_UTILITY_H
#define VULKAN_DEMO_UTILITY_H
#include <filesystem>
#include <concepts>


namespace Ace {
    namespace fs = std::filesystem;
    template<typename T>
    concept PositiveInterge =std::integral<T> && requires(T x) {
        requires x >= 0;
    };
}  // namespace ace;

#endif //VULKAN_DEMO_UTILITY_H
