//TEXTURE PLUS LIGHT
#include <iostream>
#include <cmath>

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
#include "Camera.h"


/* Window */
const GLuint Width = 800, Height = 600;
int windowWidth, windowHeight;
//int Width, Height;

void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode);
//void ScrollCallback(GLFWwindow *window, double xOffset, double yOffset);
void MouseCallback(GLFWwindow *window, double xPos, double yPos);
void Movement(); //Called every single frame

/* Initial values of the camera */
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
GLfloat lastXPosition = Width / 2.0;
GLfloat lastYPosition = Height / 2.0;
bool keys[1024]; //Different types of keys
bool firstMouse = true;
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

glm::vec3 LightPosition(1.2f, 1.0f, 2.0f);


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
    

    glfwGetFramebufferSize( window, &windowWidth, &windowHeight );
    
    if ( nullptr == window )
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate( );
        
        return -1;
    }
    
    glfwMakeContextCurrent( window );
    glfwGetFramebufferSize( window, &windowWidth, &windowHeight);

    
    /* Set up the camera tools (key, cursor, scroll )*/
    glfwSetKeyCallback(window, KeyCallback);
    glfwSetCursorPosCallback (window, MouseCallback);
    //glfwSetScrollCallback(window, ScrollCallback);
    
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    
    glewExperimental = GL_TRUE; // Why --> https://stackoverflow.com/questions/46376020/opengl-glgenvertexarrays-thread-1-exc-bad-access-code-1-address-0x0
    /* Initialize GLEW to setup the OpenGL Function pointers */
    if ( GLEW_OK != glewInit( ) )
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return -1;
    }
    
    /* Viewport dimensions */
    glViewport( 0, 0, windowWidth, windowHeight );
    
    glEnable(GL_DEPTH_TEST);

    
    //glEnable( GL_BLEND );
    //glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    
    /* Build and compile the shader program */
    Shader lightingShader( "resources/shaders/lighting.vs", "resources/shaders/lighting.frag" );
    Shader lampShader( "resources/shaders/lamp.vs", "resources/shaders/lamp.frag" );
 // Set up vertex data (and buffer(s)) and attribute pointers
 // Set up vertex data (and buffer(s)) and attribute pointers
 GLfloat vertices[] =
 {
     // Positions            // Normals              // Texture Coords
     -0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,     0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,     0.0f,  0.0f, -1.0f,     1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,     0.0f,  0.0f, -1.0f,     1.0f,  1.0f,
     0.5f,  0.5f, -0.5f,     0.0f,  0.0f, -1.0f,     1.0f,  1.0f,
     -0.5f,  0.5f, -0.5f,    0.0f,  0.0f, -1.0f,     0.0f,  1.0f,
     -0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,     0.0f,  0.0f,
     
     -0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  1.0f,     0.0f,  0.0f,
     0.5f, -0.5f,  0.5f,     0.0f,  0.0f,  1.0f,     1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,     0.0f,  0.0f,  1.0f,     1.0f,  1.0f,
     0.5f,  0.5f,  0.5f,     0.0f,  0.0f,  1.0f,      1.0f,  1.0f,
     -0.5f,  0.5f,  0.5f,    0.0f,  0.0f,  1.0f,     0.0f,  1.0f,
     -0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  1.0f,     0.0f,  0.0f,
     
     -0.5f,  0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,    1.0f,  0.0f,
     -0.5f,  0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,    1.0f,  1.0f,
     -0.5f, -0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,    0.0f,  1.0f,
     -0.5f, -0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,    0.0f,  1.0f,
     -0.5f, -0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,    0.0f,  0.0f,
     -0.5f,  0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,    1.0f,  0.0f,
     
     0.5f,  0.5f,  0.5f,     1.0f,  0.0f,  0.0f,     1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,     1.0f,  0.0f,  0.0f,     1.0f,  1.0f,
     0.5f, -0.5f, -0.5f,     1.0f,  0.0f,  0.0f,     0.0f,  1.0f,
     0.5f, -0.5f, -0.5f,     1.0f,  0.0f,  0.0f,     0.0f,  1.0f,
     0.5f, -0.5f,  0.5f,     1.0f,  0.0f,  0.0f,     0.0f,  0.0f,
     0.5f,  0.5f,  0.5f,     1.0f,  0.0f,  0.0f,     1.0f,  0.0f,
     
     -0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,     0.0f,  1.0f,
     0.5f, -0.5f, -0.5f,     0.0f, -1.0f,  0.0f,     1.0f,  1.0f,
     0.5f, -0.5f,  0.5f,     0.0f, -1.0f,  0.0f,     1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,     0.0f, -1.0f,  0.0f,     1.0f,  0.0f,
     -0.5f, -0.5f,  0.5f,    0.0f, -1.0f,  0.0f,     0.0f,  0.0f,
     -0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,     0.0f,  1.0f,
     
     -0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,     0.0f,  1.0f,
     0.5f,  0.5f, -0.5f,     0.0f,  1.0f,  0.0f,     1.0f,  1.0f,
     0.5f,  0.5f,  0.5f,     0.0f,  1.0f,  0.0f,     1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,     0.0f,  1.0f,  0.0f,     1.0f,  0.0f,
     -0.5f,  0.5f,  0.5f,    0.0f,  1.0f,  0.0f,     0.0f,  0.0f,
     -0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,     0.0f,  1.0f
 };
 
 // Positions all containers
 glm::vec3 cubePositions[] = {
     glm::vec3(  0.0f,   0.0f,   0.0f),
     glm::vec3(  2.0f,   5.0f,   -15.0f),
     glm::vec3(  -1.5f,  -2.2f,  -2.5f),
     glm::vec3(  -3.8f,  -2.0f,  -12.3f),
     glm::vec3(  2.4f,   -0.4f,  -3.5f),
     glm::vec3(  -1.7f,  3.0f,   -7.5f),
     glm::vec3(  1.3f,   -2.0f,  -2.5f),
     glm::vec3(  1.5f,   2.0f,   -2.5f),
     glm::vec3(  1.5f,   0.2f,   -1.5f),
     glm::vec3(  -1.3f,  1.0f,   -1.5f)
 };
    /* Initialization of the Vertex Buffer Object, Vertex Array Object and EBO*/
    GLuint vbo, boxVao; //,ebo
    glGenVertexArrays( 1, &boxVao );
    glGenBuffers( 1, &vbo );
   // glGenBuffers( 1, &ebo );
    
   
    
    glBindBuffer( GL_ARRAY_BUFFER, vbo );
    glBufferData( GL_ARRAY_BUFFER, sizeof( vertices ), vertices, GL_STATIC_DRAW );
    
    //glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, ebo );
    //glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( indices ), indices, GL_STATIC_DRAW );
     glBindVertexArray( boxVao );
    
    /* Position attribute */
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof( GLfloat ), ( GLvoid * ) 0 ); //6 differents pieces of values
    glEnableVertexAttribArray(0);
    
    /* Normal attribute */
    glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof( GLfloat ), ( GLvoid * )(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    
    /* Color attribute */
    //glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof( GLfloat ), ( GLvoid * )( 3 * sizeof( GLfloat ) ) );
    //glEnableVertexAttribArray( 1 );
    
    /* Texture Coordinate attribute */
    glVertexAttribPointer( 2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof( GLfloat ), ( GLvoid * )( 6 * sizeof( GLfloat ) ) );
    glEnableVertexAttribArray( 2 );
    
    glBindVertexArray( 0 ); // Unbind vao
    
    GLuint lightVao;
    glGenVertexArrays(1, &lightVao);
    glBindVertexArray(lightVao);
    glBindBuffer( GL_ARRAY_BUFFER, vbo);
    // Set the vertex attributes (only position data for the lamp))
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof( GLfloat ), ( GLvoid * )0 ); // Note that we skip over the other data in our buffer object (we don't need the normals/textures, only positions).
    glEnableVertexAttribArray( 0 );
    glBindVertexArray( 0 );
    
    // Load textures
    GLuint diffuseMap, specularMap, emissionMap;
    glGenTextures( 1, &diffuseMap );
    glGenTextures( 1, &specularMap );
    glGenTextures( 1, &emissionMap );
    int width, height;
    unsigned char *image;
    
    // Diffuse map
    image = SOIL_load_image( "images/container2.png", &width, &height, 0, SOIL_LOAD_RGB );
    glBindTexture( GL_TEXTURE_2D, diffuseMap );
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image );
    glGenerateMipmap( GL_TEXTURE_2D );
    SOIL_free_image_data( image );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST );
    //glBindTexture( GL_TEXTURE_2D, 0 );
    
    // Specular map
    image = SOIL_load_image( "images/container2_specular.png", &width, &height, 0, SOIL_LOAD_RGB );
    glBindTexture( GL_TEXTURE_2D, specularMap );
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image );
    glGenerateMipmap( GL_TEXTURE_2D );
    SOIL_free_image_data( image );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST );
    glBindTexture( GL_TEXTURE_2D, 0 );

    
    // Set texture units
    lightingShader.Use( );
    glUniform1i( glGetUniformLocation(lightingShader.Program, "material.diffuse" ),  0 );
    glUniform1i( glGetUniformLocation(lightingShader.Program, "material.specular" ), 1 );
    
    
    glm::mat4 projection(1);
    projection = glm::perspective( camera.GetZoom( ), ( GLfloat )windowWidth / ( GLfloat )windowHeight, 0.1f, 100.0f );
    
    // Game loop
    while (  glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
              glfwWindowShouldClose(window) == 0 ) //Quits if you press Esc
    {
        //LightPosition.x -= 0.03f;
        //LightPosition.z -= 0.03f;
        /* Set the frame time*/
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        
        glfwPollEvents( );
        Movement();
        
        // Render
        // Clear the colorbuffer
        glClearColor( 0.2f, 0.2f, 0.4f, 0.0f ); //Background color
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
       
        lightingShader.Use();
        GLint lightPosLoc = glGetUniformLocation( lightingShader.Program, "light.position" );
        GLint viewPosLocation = glGetUniformLocation(lightingShader.Program, "viewPos");
        
        glUniform3f( lightPosLoc, LightPosition.x, LightPosition.y, LightPosition.z );
        glUniform3f(viewPosLocation, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
        
        // Set lights properties
        glUniform3f( glGetUniformLocation( lightingShader.Program, "light.ambient" ),  0.2f, 0.2f, 0.2f );
        glUniform3f( glGetUniformLocation( lightingShader.Program, "light.diffuse" ),  0.5f, 0.5f, 0.5f );
        glUniform3f( glGetUniformLocation( lightingShader.Program, "light.specular" ), 1.0f, 1.0f, 1.0f );
        glUniform1f( glGetUniformLocation( lightingShader.Program, "light.constant" ), 1.0f );
        glUniform1f( glGetUniformLocation( lightingShader.Program, "light.linear" ), 0.09 );
        glUniform1f( glGetUniformLocation( lightingShader.Program, "light.quadratic" ), 0.032 );
        // Set material properties
        glUniform1f( glGetUniformLocation( lightingShader.Program, "material.shininess" ), 32.0f );

        glm::mat4 view(1);
        view = camera.GetViewMatrix();
        
        GLint modelLocation = glGetUniformLocation(lightingShader.Program, "model");
        GLint viewLocation = glGetUniformLocation(lightingShader.Program, "view");
        GLint projectionLocation = glGetUniformLocation(lightingShader.Program, "projection");
        
        /* Pass those matrices to the shader */
        glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));
        
        // Bind diffuse map
        glActiveTexture( GL_TEXTURE0 );
        glBindTexture( GL_TEXTURE_2D, diffuseMap );
        // Bind specular map
        glActiveTexture( GL_TEXTURE1 );
        glBindTexture( GL_TEXTURE_2D, specularMap );
    
        glm::mat4 model(1);
        glBindVertexArray( boxVao );
        for ( GLuint i = 0; i < 10; i++)
        {
            //model = glm::mat4( );
            model = glm::translate( model, cubePositions[i] );
            GLfloat angle = 20.0f * i;
            model = glm::rotate( model, angle, glm::vec3( 1.0f, 0.3f, 0.5f ) );
            glUniformMatrix4fv( modelLocation, 1, GL_FALSE, glm::value_ptr( model ) );
            
            glDrawArrays( GL_TRIANGLES, 0, 36 );
        }
        
        // Also draw the lamp object, again binding the appropriate shader
        lampShader.Use( );
        // Get location objects for the matrices on the lamp shader (these could be different on a different shader)
        modelLocation = glGetUniformLocation( lampShader.Program, "model" );
        viewLocation = glGetUniformLocation( lampShader.Program, "view" );
        projectionLocation = glGetUniformLocation( lampShader.Program, "projection" );
        // Set matrices
        glUniformMatrix4fv( viewLocation, 1, GL_FALSE, glm::value_ptr( view ) );
        glUniformMatrix4fv( projectionLocation, 1, GL_FALSE, glm::value_ptr( projection ) );
        model = glm::mat4( );
        model = glm::translate( model, LightPosition );
        model = glm::scale( model, glm::vec3( 0.2f ) ); // Make it a smaller cube
        glUniformMatrix4fv( modelLocation, 1, GL_FALSE, glm::value_ptr( model ) );
        // Draw the light object (using light's vertex attributes)
        glBindVertexArray( lightVao);
        glDrawArrays( GL_TRIANGLES, 0, 36 );
        glBindVertexArray( 0 );
        
        // Swap the screen buffers
        glfwSwapBuffers( window );
    }
    
    // Properly de-allocate all resources once they've outlived their purpose
    glDeleteVertexArrays( 1, &boxVao );
    glDeleteVertexArrays( 1, &lightVao );
    glDeleteBuffers( 1, &vbo );
    //glDeleteBuffers(1, &ebo);
    
    // Terminate GLFW, clearing any resources allocated by GLFW.
    glfwTerminate( );
    
    return 0;
}

/* Handling the movement of the camera */
void Movement()
{
    /* Moving forward */
    if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
    {
        camera.ProcessKeyboard((FORWARD), deltaTime);
    }
    /* Moving down */
    if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
    {
        camera.ProcessKeyboard((BACKWARD), deltaTime);
    }
    /* Moving to the left */
    if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
    {
        camera.ProcessKeyboard((LEFT), deltaTime);
    }
    /* Moving to the right */
    if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
    {
        camera.ProcessKeyboard((RIGHT), deltaTime);
    }
}

void KeyCallback( GLFWwindow *window, int key, int scancode, int action, int mode )
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS )
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
    
    if(key >= 0 && key < 1024)
    {
        if (GLFW_PRESS == action)
        {
            keys[key] = true;
        }
        else if (GLFW_RELEASE == action)
        {
            keys[key] = false;
        }
    }
}

void MouseCallback( GLFWwindow *window, double xPos, double yPos)
{
    if (firstMouse)
    {
        lastXPosition= xPos;
        lastYPosition = yPos;
        firstMouse = false;
    }
    
    GLfloat xOffset = xPos - lastXPosition;
    GLfloat yOffset = lastYPosition - yPos; // Reversed bc it the positions start from the bottom left
    
    lastXPosition = xPos;
    lastYPosition = yPos;
    
    camera.ProcessMouseMovement(xOffset, yOffset);
}

