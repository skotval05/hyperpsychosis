#include "graphics.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow * window);

const unsigned int SCREENWIDTH = 800;
const unsigned int SCREENHEIGHT = 600;

const char* vertexShaderSource = 
        "#version 330 core \n"
        "layout (location =0) in vec3 aPos;\n"
        "void main() {\n"
        "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
        "}\0";

const char* fragmentShaderSource = 
        "#version 330 core\n"
        "out vec4 FragColor;\n"
        "void main() {\n"
        "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
        "}\0";

int doGraphicsSetup() {
    // replacement for main function, visible from header file for implementation.
    // if you get lost, and it's okay if you do! I love you regardless, whatever you're messing with is probably
    // a buffer of some sort misbehaving. I wish I could guide you, but I'm in the past, and you're not. :P
    
    // let us initiate the window.
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // window creation

    GLFWwindow* window = glfwCreateWindow(SCREENWIDTH, SCREENHEIGHT, "Test Window", NULL, NULL);

    if (window == NULL) {
        printf("Failed to create GLFW window.\n");
        glfwTerminate();
        return -1;
    }

    // if we're here, we good! yeeey
    
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        printf("Failed to initialize GLAD.\n");
        return -1;
    }
    
    // test triangle
    float vertices[] = {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f
    };

    // generate vertex buffer object (stores vertices)
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // maybe something goes here, but now its telling me to buffer data.
    
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // vertex shader. is programmable. uses GLSL, no idea if that'll be foreshadowing of a painful future.

    // SEE IMPLEMENTATION ABOVE THIS FUNCTION.

    // great. have the shader src, now we need an object and to use it.

    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);

    // attach source  and compile shader?

    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // check if shader compilation was successful, probably a good idea.
    
    int success;
    char infoLog[512];

    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        printf("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n%s\n", infoLog);
        return 1;
    }

    // begin fragment shader ! not really sure what this part does.

    // SEE IMPLEMENTATION ABOVE THIS FUNCTION

    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    
    // should also check if this one compiles, might be important.
    
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        printf("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n%s\n", infoLog);
        return 1;
    }

    // make da shader program !?

    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();

    // jesse we need to attach the shaders

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // now we make sure the program successfully compiles. i guess. whatever. not my problem if it doesnt (lie)

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        printf("ERROR::PROGRAM::SHADERS::LINKING_FAILED\n%s\n", infoLog);
        return 1;
    }

    glUseProgram(shaderProgram);

    // theoretically, at this point, we dont need the raw shaders anymore.

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // now comes linking vertex attributes. oh no.
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // make da vertex array object :3

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);

    // initialization code done once unless the object frequently changes...

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // DA RENDER LOOP

    while (!glfwWindowShouldClose(window)) {

        // do input
        processInput(window);

        // render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // draw triangle number 1
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // swap buffers and poll IO events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // cleanup!
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    // terminate.
    glfwTerminate();
    return 0;
}

// PROCESS GLFW INPUT

void processInput(GLFWwindow * window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

// WINDOW SIZE CHANGE FUNCTION

void framebuffer_size_callback(GLFWwindow * window, int width, int height) {
    // make sure viewport matches new window dimensions.
    glViewport(0, 0, width, height);
}
