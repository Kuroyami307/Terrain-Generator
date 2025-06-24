#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cstdlib>
#include <stack>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>  // For transformations like translate, rotate, scale
#include <glm/gtc/type_ptr.hpp>

#include "custom/game.h"
#include "custom/shader.h"


#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#include <emscripten/html5.h> 
#endif

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void glfwErrorCallback(int error, const char* description);
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);

unsigned int SCR_WIDTH = 700;
unsigned int SCR_HEIGHT = 700;

bool camRotation = false;
glm::vec3 cameraPos(100.0f, 300.0f, 200.0f), targetPos(0.0f), cameraUp(0.0f, 0.0f, 1.0f);
light lightSource = {glm::vec3(0.0f, 0.0f, 200.0f), glm::vec4(1.0f), 1.0f};
glm::mat4 rotMat(1.0f);

// Globals needed for emscripten main loop
GLFWwindow* window = nullptr;
player* pPtr = nullptr;
shader *shaderPtr = nullptr;


void main_loop()
{

    if (!window)
        return;

    processInput(window);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    pPtr->draw(lightSource, cameraPos);

    glfwSwapBuffers(window);
    glfwPollEvents();
}

int main()
{
    glfwSetErrorCallback(glfwErrorCallback);
    glfwInit();

    #ifdef __EMSCRIPTEN__
        glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    #else
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    #endif
    

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (!window)
    {
        std::cout << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);

    #ifdef __EMSCRIPTEN__
    if (!gladLoadGLLoader((GLADloadproc)emscripten_webgl_get_proc_address))
    {
        std::cout << "Failed to initialize GLAD\n";
        return -1;
    }
    #else
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD\n";
        return -1;
    }
    #endif

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glEnable(GL_DEPTH_TEST);

    shader mainShader;
    shaderPtr = &mainShader;

    #ifdef __EMSCRIPTEN__
        mainShader.loadShaders("docs/vS3dWeb.shader", "docs/fS3DWeb.shader");
    #else
        mainShader.loadShaders("Shaders/vS3D.shader", "Shaders/fS3D.shader");
    #endif


    pPtr = new player(&mainShader);
    pPtr->sheet3D(800, 800, 100);
    // pPtr->setPosition(glm::vec3(-300.0f, 300.0f, 40.0f));

    view  = glm::lookAt(cameraPos, targetPos, cameraUp);
    projection = glm::perspective(glm::radians(85.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1200.0f);

#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(main_loop, 0, 1);
#else
    while (!glfwWindowShouldClose(window))
    {
        main_loop();
    }
#endif

    glDeleteProgram(mainShader.progID);
    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    SCR_WIDTH = width;
    SCR_HEIGHT = height;
}

void glfwErrorCallback(int error, const char* description)
{
    std::cerr << "GLFW Error (" << error << "): " << description << std::endl;
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) 
{
    static double prevMouseX, prevMouseY;
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && !camRotation)
    {
        std::cout << "Mouse button down" <<std::endl;
        glfwGetCursorPos(window, &prevMouseX, &prevMouseY);
        camRotation = true;
    }
        

    if(camRotation)
    {
        double currMouseX = xpos, currMouseY=ypos;

        double dx = currMouseX - prevMouseX;
        double dy = currMouseY - prevMouseY;

        // std::cout << dx << " " << dy << "\n";

        glm::mat4 rot = glm::rotate(glm::mat4(1.0f), glm::radians((float)dx), glm::vec3(0.0f, 0.0f, 1.0f));
        rot = glm::rotate(rot, glm::radians((float)dy), glm::vec3(1.0f, 0.0f, 0.0f));

        glm::vec4 temp(cameraPos, 1.0f);
        temp = rot * temp;
        cameraPos = glm::vec3(temp.x, temp.y, temp.z);

        prevMouseX = currMouseX;
        prevMouseY = currMouseY;
    }

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
    {
        camRotation = false;
    }
}