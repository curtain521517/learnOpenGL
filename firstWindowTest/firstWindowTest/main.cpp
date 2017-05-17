//
//  main.cpp
//  firstWindowTest
//
//  Created by xufan on 2017/5/15.
//  Copyright © 2017年 xufan. All rights reserved.
//

#include <iostream>

#define GLEW_STATIC
#include <GL/glew.h>

#include <GLFW/glfw3.h>

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void display();


int main(int argc, const char * argv[]) {
    
        //初始化GLFW
    glfwInit();
    
    //初始化OpenGL的主版本3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    //初始化OpenGL的次版本3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    //使用核心模式
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //窗口不可调大小
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    //适于os x系统
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    
    GLFWwindow *window = glfwCreateWindow(800, 600, "firstWindowTest", nullptr, nullptr);
    if (window == nullptr) {
        std::cout<<"failed to create GLFW window";
        glfwTerminate();
    }
    
    //将window设置为当前线程的主上下文
    glfwMakeContextCurrent(window);
    
    //这个变量值可以使GLEW在管理OpenGL的函数指针时更多的使用现代化技术
    glewExperimental = GL_TRUE;
    
    //初始化glew
    if (glewInit() != GLEW_OK) {
        
        std::cout<<"failed to initialize GLEW" << std::endl;
        return -1;
        
    }
    
    int width = 0, height = 0;

    //拿到窗口的尺寸(像素)
    glfwGetFramebufferSize(window, &width, &height);
    //设置视口，前两个参数控制窗口左下角的位置，后面两个是控制渲染窗口的宽度和高度（像素）
    glViewport(0, 0, width, height);
    
    glfwSetKeyCallback(window, key_callback);
    
    while (!glfwWindowShouldClose(window)) {
        
        //事件检测，如果有会调用其回调函数
        glfwPollEvents();
        
        //渲染
        display();
        
        //交换缓冲
        glfwSwapBuffers(window);
    }
    
    //释放GLFW分配的内存
    glfwTerminate();
    
    std::cout << "Hello, World!\n";
    
    
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
