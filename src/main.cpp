
import GLWindow;
import utility;

#include <iostream>

#include <exception>



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

constinit float attributeArray[] = { 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,// ����
                                    0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,// ����
                                    -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,// ����
                                    -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f// ����
};
//constinit float attributeArray[] = {0.0f, 0.0f, 1.0f, 0.0f, 0.0f,// ����
//                                    0.5f, 0.5f, 0.0f, 1.0f, 0.0f,// ����
//                                    0.0f, 0.5f, 0.0f, 0.0f, 1.0f,// ����
//                                    0.5f, 0.0f, 1.0f, 1.0f, 1.0f// ����
//};
constinit unsigned int EBOArray[] = {
        0, 1, 3,
        1, 2, 3
};


constinit int windowWidth = 800;
constinit int windowHeight = 800;

int main(const int argc, const char* argv[]) {
    try {
        Ace::GLWindow window(windowWidth, windowHeight, "computer graphics");
        // TODO VBO EBO VAO ���ϵ�һ��RenderContext��
        window.generateVao();
        //        window.generateEBO(EBOArray);
        window.generateVBO(vertices, { {0, 3},
                                            {1, 2} });

        window.loadShader("shader/vertexShader.glsl", "shader/fragmentShader.glsl", "config/variablesLocation.json");
        window.load2DTexture("Images/brick.png", "cusTexture");
        window.load2DTexture("Images/awesomeface.png", "cusTexture1");
        window.setBackgroundColor(0.0f, 1.0f, 1.0f, 0.0f);
        window.render();
    }
    catch (std::exception& e) {
        std::cout << "failed : " << e.what() << '\n';
        return 1;
    }
    return 0;
}