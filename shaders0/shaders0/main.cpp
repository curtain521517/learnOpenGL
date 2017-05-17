//
//  main.cpp
//  shaders0
//
//  Created by xufan on 2017/5/16.
//  Copyright © 2017年 xufan. All rights reserved.
//

#include <iostream>
#include <math.h>

#define GLEW_STATIC
#include <GL/glew.h>

#include <GLFW/glfw3.h>

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void display();

// Shaders


const GLchar *vertexShaderSource = " \n \
    #version 330 core \n \
    layout (location = 0) in vec3 position; // position变量的属性位置值为0 \n \
    layout (location = 1) in vec3 color;    // 颜色变量的属性位置值为1 \n \
\n \
    out vec3 ourColor; // 为片段着色器输出一个颜色输出\n \
\n \
    void main() \n \
    { \n \
        gl_Position = vec4(position, 1.0); // 注意我们如何把一个vec3作为vec4的构造器的参数\n \
        ourColor = color; //将ourColor设置为我们从顶点数据那里得到的输入颜色\n \
    }\n";

const GLchar* fragmentShaderSource = "\n \
    #version 330 core \n \
    in vec3 ourColor; // 从顶点着色器传来的输入变量（名称相同、类型相同）\n \
\n \
    out vec4 color; // 片段着色器输出的变量名可以任意命名，类型必须是vec4\n \
    //uniform vec4 ourColor; //在OpenGL程序代码中设定这个变量\n \
\n \
    void main() \n \
    { \n \
        color = vec4(ourColor, 1.0f); \n \
        //color = ourColor; \n \
    }\n";

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    
    GLFWwindow *window = glfwCreateWindow(800, 600, "learnOpenGL", nullptr, nullptr);
    glfwMakeContextCurrent(window);
    
    glfwSetKeyCallback(window, key_callback);
    
    glewExperimental = GL_TRUE;
    glewInit();
    
    printf("%s\n",vertexShaderSource);
    printf("%s\n",fragmentShaderSource);
    GLint nrAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;

    int width,height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
    
    //vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    
    //check
    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout<<"vertex shader compilation failed"<<std::endl;
    }
    
    //fragmeng shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource,NULL);
    glCompileShader(fragmentShader);
    
    glGetShaderiv(fragmentShader,GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout<<"fragment shader compilation failed"<<std::endl;
    }
    
    //create a program
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    
    //check
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout<<"shaderPorgram failed"<<std::endl;
    }
    
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    
//    GLfloat vertices[] = {
//        -0.5f, -0.5f, 0.0f, // Left
//        0.5f, -0.5f, 0.0f, // Right
//        0.0f,  0.5f, 0.0f  // Top
//    };
//    GLfloat vertices[] = {
//        
//
//        
//        // 第一个三角形
//        0.5f, 0.5f, 0.0f,   // 右上角
//        0.5f, -0.5f, 0.0f,  // 右下角
//        -0.5f, 0.5f, 0.0f,  // 左上角
//        // 第二个三角形
//        0.5f, -0.5f, 0.0f,  // 右下角
//        -0.5f, -0.5f, 0.0f, // 左下角
//        -0.5f, 0.5f, 0.0f   // 左上角
//        
//    };
    
    GLfloat vertices[] = {
        // 位置              // 颜色
        0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // 右下
        -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  // 左下
        0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // 顶部

    };
    
    GLuint VBO; // vertex buffer object,顶点缓冲对象
    GLuint VAO; // vertex array object ,顶点数组对象
    
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    
    //绑定
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    
    //复制
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    //设置顶点属性指针,位置属性
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 6, (GLvoid*)(0));
    //启动顶点属性，位置是0
    glEnableVertexAttribArray(0);
    
    //设置颜色属性，启动
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, (GLvoid *)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    
    //解绑
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
    //线框
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    //GL_FILL 填充
    
    while (!glfwWindowShouldClose(window)) {
        
        glfwPollEvents();
        
        display();
        
        //激活着色器
        glUseProgram(shaderProgram);
        
//        GLfloat timeValue = glfwGetTime();
//        GLfloat greenValue = (sin(timeValue) / 2) + 0.5;
//        GLint vertextColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
//        glUniform4f(vertextColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
//        
        //绘制这个三角形
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices)/ (3 * sizeof(vertices[0])));
        glBindVertexArray(0);
        
        glfwSwapBuffers(window);
    }
    
    glDeleteVertexArrays(1,&VAO);
    glDeleteBuffers(1, &VBO);
    
    glfwTerminate();
    
    return 0;
}

void display()
{
    //缓冲颜色
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    //清空颜色缓冲
    glClear(GL_COLOR_BUFFER_BIT);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        
        std::cout<<"enter escape";
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}
