//
//  Image_Turns.cpp
//  SDL
//
//  Created by Alice Pagnoux on 05/12/2019.
//  Copyright © 2019 Alice Pagnoux. All rights reserved.
//


/* Glew */
#define GLEW_STATIC
#include <GL/glew.h>

/* Glfw */
#include <GLFW/glfw3.h>

/* Soil allows to handle .png, .jpg, etc and .obj */
#include "SOIL2/SOIL2.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
// Other includes, we use the glm (maths)
#include "Shader.h"


/* Window */
const GLuint Width = 800, Height = 600;

/* Main */
int main( )
{
    /* Glew initialization */
    glfwInit( );
    
    /* Required options for GLFW*/
    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 ); // <-- latest version of OpenGL 3.3
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
    glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
    glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );
    
    glfwWindowHint( GLFW_RESIZABLE, GL_FALSE );
    
    /* Characteristics of the window */
    GLFWwindow *window = glfwCreateWindow( Width, Height, "Coursework", nullptr, nullptr );
    
    int windowWidth, windowHeight;
    glfwGetFramebufferSize( window, &windowWidth, &windowHeight );
    
    if ( nullptr == window )
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate( );
        
        return EXIT_FAILURE;
    }
    
    glfwMakeContextCurrent( window );
    
    
    glewExperimental = GL_TRUE; // Why --> https://stackoverflow.com/questions/46376020/opengl-glgenvertexarrays-thread-1-exc-bad-access-code-1-address-0x0
    /* Initialize GLEW to setup the OpenGL Function pointers */
    if ( GLEW_OK != glewInit( ) )
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return EXIT_FAILURE;
    }
    
    /* Viewport dimensions */
    glViewport( 0, 0, windowWidth, windowHeight );
    
    //glEnable(GL_DEPTH_TEST);
    
    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    
    /* Build and compile the shader program */
    Shader shader( "resources/shaders/core.vs", "resources/shaders/core.frag" );
    
        GLfloat vertices[] =
    {
        // Positions          // Colors           // Texture Coords
        0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // Top Right
        0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // Bottom Right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // Bottom Left
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // Top Left
    };
    GLuint indices[] =
    {  // Note that we start from 0!
        0, 1, 3, // First Triangle
        1, 2, 3  // Second Triangle
    };
    
    /* Initialization of the Vertex Buffer Object, Vertex Array Object and EBO*/
    GLuint vbo, vao, ebo;
    glGenVertexArrays( 1, &vao );
    glGenBuffers( 1, &vbo );
    glGenBuffers( 1, &ebo );
    
    glBindVertexArray( vao );
    
    glBindBuffer( GL_ARRAY_BUFFER, vbo );
    glBufferData( GL_ARRAY_BUFFER, sizeof( vertices ), vertices, GL_STATIC_DRAW );
    
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, ebo );
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( indices ), indices, GL_STATIC_DRAW );
    
    
    /* Position attribute */
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof( GLfloat ), ( GLvoid * ) 0 );
    glEnableVertexAttribArray(0);
    
    /* Color attribute */
    glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof( GLfloat ), ( GLvoid * )( 3 * sizeof( GLfloat ) ) );
    glEnableVertexAttribArray( 1 );
    
    /* Texture Coordinate attribute */
    glVertexAttribPointer( 2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof( GLfloat ), ( GLvoid * )( 6 * sizeof( GLfloat ) ) );
    glEnableVertexAttribArray( 2 );
    
    glBindVertexArray( 0 ); // Unbind vao
    
    // Load and create a texture
    GLuint texture;
    
    int Width, Height;
    
    
    // ===================
    // Texture
    // ===================
    glGenTextures( 1, &texture );
    glBindTexture( GL_TEXTURE_2D, texture );
    // Set our texture parameters
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    // Set texture filtering
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    /* Load, create texture and generate mipmaps */
    unsigned char *image = SOIL_load_image( "images/1.png", &Width, &Height, 0, SOIL_LOAD_RGBA );
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, Width, Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image );
    glGenerateMipmap( GL_TEXTURE_2D );
    SOIL_free_image_data( image );
    glBindTexture( GL_TEXTURE_2D, 0 );
    
    glm::mat4 projection;
    projection = glm::perspective(45.0f, (GLfloat)windowWidth / (GLfloat)windowHeight, 0.1f, 1000.0f); //45.0 degrees
    
    
    // Game loop
    while (  glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
              glfwWindowShouldClose(window) == 0 ) //Quits if you press Esc
    {
        // Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
        glfwPollEvents( );
        
        // Render
        // Clear the colorbuffer
        glClearColor( 0.2f, 0.2f, 0.4f, 0.0f ); //Background color
        glClear( GL_COLOR_BUFFER_BIT);
        
        shader.Use( );
        
        //Create tranformations such as translation/rotation
        glm::mat4 transform(1);
        
        transform = glm::translate(transform, glm::vec3(0.5f, -0.5f, 0.0f));
        transform = glm::rotate(transform, (GLfloat)glfwGetTime() * -5.0f, glm::vec3(0.0f, 0.0f, 1.0f));
        
        GLint transformationLocation = glGetUniformLocation(shader.Program, "transform");
        glUniformMatrix4fv(transformationLocation, 1, GL_FALSE, glm::value_ptr(transform));
        
        /* Activation of the texture */
        glActiveTexture( GL_TEXTURE0 );
        glBindTexture( GL_TEXTURE_2D, texture );
        glUniform1i( glGetUniformLocation( shader.Program, "texture" ), 0 );
        
        
        
        /* Create model and view matrices */
        /*glm::mat4 model;
        glm::mat4 view;
        model = glm::rotate(model, (GLfloat) glfwGetTime() * 1.0f, glm::vec3(0.5f, 1.0f, 0.0f) ); //First parameter is the speed, 2nd parameter is the position XYZ, 4x4 matrix
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
        
        GLint modelLocation = glGetUniformLocation(shader.Program, "model");
        GLint viewLocation = glGetUniformLocation(shader.Program, "view");
        GLint projectionLocation = glGetUniformLocation(shader.Program, "projection");*/
        /* Pass it to the shader */
        /*glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection)); */
        /* Draw our object */
        /*glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 36); //36 because we have 2 triangles and each has 6 coordinates
        glBindVertexArray(0);*/
        
        // Draw container
        glBindVertexArray( vao );
        glDrawElements( GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0 );
        glBindVertexArray( 0 );
        
        // Swap the screen buffers
        glfwSwapBuffers( window );
    }
    
    // Properly de-allocate all resources once they've outlived their purpose
    glDeleteVertexArrays( 1, &vao );
    glDeleteBuffers( 1, &vbo );
    
    // Terminate GLFW, clearing any resources allocated by GLFW.
    glfwTerminate( );
    
    return EXIT_SUCCESS;
}





