
#include <exception>
#include "GLWindow.h"

// TODO 因为GLFW 库的设置回调函数的函数，导致整体代码非良构
constinit int windowWidth = 800;
constinit int windowHeight = 800;

 constinit float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};

constinit float attributeArray[] = {0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,// 右上
                                    0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,// 右下
                                    -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,// 左下
                                    -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f// 左上
};
//constinit float attributeArray[] = {0.0f, 0.0f, 1.0f, 0.0f, 0.0f,// 左下
//                                    0.5f, 0.5f, 0.0f, 1.0f, 0.0f,// 右上
//                                    0.0f, 0.5f, 0.0f, 0.0f, 1.0f,// 左上
//                                    0.5f, 0.0f, 1.0f, 1.0f, 1.0f// 右下
//};
constinit unsigned int EBOArray[] = {
        0, 1, 3,
        1, 2, 3
};

import utility;

int main(void) {
  Ace::fun(1);
  return 1;}

int main2(const int argc, const char *argv[]) {
    try {
        Ace::GLWindow window(windowWidth, windowHeight, "computer graphics");
        // TODO VBO EBO VAO 整合到一个RenderContext中
        window.generateVao();
//        window.generateEBO(EBOArray);
        window.generateVBO(vertices, {{0, 3},
                                            {1, 2}});

        window.loadShader("shader/vertexShader.glsl", "shader/fragmentShader.glsl", "config/variablesLocation.json");
        window.load2DTexture("Images/brick.png", "cusTexture");
        window.load2DTexture("Images/awesomeface.png", "cusTexture1");
        window.setBackgroundColor(0.0f, 1.0f, 1.0f, 0.0f);
        window.render();
    } catch (std::exception &e) {
        std::cout << "failed : " << e.what() << '\n';
        return 1;
    }
    return 0;
}