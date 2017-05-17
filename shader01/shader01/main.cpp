//
//  main.cpp
//  shader01
//
//  Created by xufan on 2017/5/17.
//  Copyright © 2017年 xufan. All rights reserved.
//

#include <iostream>
#include <math.h>

#define GLEW_STATIC
#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include "Shader.hpp"

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void display();


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

    GLint nrAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;
    
    int width,height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
    
    
    Shader myShader("/Users/xufan/learnOpenGL/shader01/shader01.vs", "/Users/xufan/learnOpenGL/shader01/shader01.frag");
    
    
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
        
        myShader.Use();
        
                GLfloat timeValue = glfwGetTime();
                GLfloat offx = (sin(timeValue) / 2); // 0-0.5
        //        GLint vertextColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
        //        glUniform4f(vertextColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
        //
        GLint offyLocation = glGetUniformLocation(myShader.Program, "offx");
        glUniform1f(offyLocation, offx);
        //绘制这个三角形
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices)/ (6 * sizeof(vertices[0])));
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
