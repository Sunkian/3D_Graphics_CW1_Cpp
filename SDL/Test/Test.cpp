#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>


int main()
{
    glfwInit();
    
    const int WINDOW_WIDTH = 640;
    const int WINDOW_HEIGHT = 480;
    int framebufferWidth = 0;
    int framebufferHeight = 0;
    
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    
    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, 'HEYYY', NULL, NULL);
    
    glfwGetFramebufferSize(window, &framebufferWidth, &framebufferHeight);
    glViewport(0, 0, framebufferWidth, framebufferHeight);
    
    glfwMakeContextCurrent(window);
    
    //INIT GLEW
    glewExperimental = GL_TRUE;
    if(glewInit() != GLEW_OK)
        {
            std::cout << "ERROR::MAIN.CPP::GLEW_INIT_FAILED" << "\n";
            glfwTerminate();
        }
    //MAIN LOOP
    while(!glfwWindowShouldClose(window))
        {
            
            
            
        }
    
    glfwTerminate();
    
    return 0;
}
