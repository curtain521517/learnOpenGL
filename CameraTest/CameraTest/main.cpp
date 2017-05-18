//
//  main.cpp
//  CameraTest
//
//  Created by xufan on 2017/5/18.
//  Copyright © 2017年 xufan. All rights reserved.
//

#include <iostream>

#define GLEW_STATIC
#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <shaderClass/Shader.hpp>

#include <SOIL/SOIL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

void do_movement();
void display();
Shader *gShader;
glm::mat4 *gModel;

bool keys[1024];

GLfloat deltaTime = 0.0f;   // 当前帧与上一帧的时间差
GLfloat lastFrame = 0.0f;   // 上一帧的时间

GLfloat lastX = 400, lastY = 300;
GLfloat yaw= -90.0f, pitch;
GLfloat fov = 45.0f;//视野
bool firstMouse = true;




GLfloat vertices[] = {
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

glm::vec3 cubePositions[] = {
    glm::vec3( 0.0f,  0.0f,  0.0f),
    glm::vec3( 2.0f,  5.0f, -15.0f),
    glm::vec3(-1.5f, -2.2f, -2.5f),
    glm::vec3(-3.8f, -2.0f, -12.3f),
    glm::vec3( 2.4f, -0.4f, -3.5f),
    glm::vec3(-1.7f,  3.0f, -7.5f),
    glm::vec3( 1.3f, -2.0f, -2.5f),
    glm::vec3( 1.5f,  2.0f, -2.5f),
    glm::vec3( 1.5f,  0.2f, -1.5f),
    glm::vec3(-1.3f,  1.0f, -1.5f)
};


glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f,  3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);





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
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    

    
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    
    glewExperimental = GL_TRUE;
    glewInit();
    
    int screenWidth, screenHeight;
    glfwGetFramebufferSize(window, &screenWidth, &screenHeight);
    glViewport(0, 0, screenWidth, screenHeight);
    
    //初始化shader
    Shader myShader("/Users/xufan/learnOpenGL/CameraTest/shader01.vs", "/Users/xufan/learnOpenGL/CameraTest/shader01.frag");
    gShader = &myShader;
    
    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    
    glBindVertexArray(VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    //    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    
    //    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    //    glEnableVertexAttribArray(1);
    
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);
    
    //这里只需要解绑定VAO一个就可以了，VAO要在前面才行，
    glBindVertexArray(0);
    
    
    //纹理1
    GLuint texture1;
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    //加载图片1
    int width, height;
    unsigned char* image = SOIL_load_image("/Users/xufan/learnOpenGL/Textures/box.jpg", &width, &height, 0, SOIL_LOAD_RGB);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    
    SOIL_free_image_data(image);
    glBindTexture(GL_TEXTURE_2D, 0);
    
    //纹理2
    GLuint texture2;
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    //加载图片2
    image = SOIL_load_image("/Users/xufan/learnOpenGL/Textures/awesomeface.png", &width, &height, 0, SOIL_LOAD_RGB);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(image);
    glBindTexture(GL_TEXTURE_2D, 0);
    
    //启动深度测试，z轴处理
    glEnable(GL_DEPTH_TEST);
    
    
    while (!glfwWindowShouldClose(window)) {
        
        glfwPollEvents();
        do_movement();
        
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        
        //缓冲颜色
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        //清空颜色缓冲
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glUniform1i(glGetUniformLocation(myShader.Program,"ourTexture1"), 0);
        
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);
        glUniform1i(glGetUniformLocation(myShader.Program,"ourTexture2"), 1);
        
        myShader.Use();
        
        // view matrix
        //glm::mat4 view;
        //view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
        GLfloat radius = 10.0f;
        GLfloat camX = sin(glfwGetTime()) * radius;
        GLfloat camZ = cos(glfwGetTime()) * radius;
        glm::mat4 view;
        //view = glm::lookAt(glm::vec3(camX, 0.0, camZ), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
        
        view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

        
        
        glm::mat4 projection;
        projection = glm::perspective(fov, screenWidth / screenHeight * 1.0f, 0.1f, 100.0f);
        
        // 将矩阵传入着色器
        GLint modelLoc = glGetUniformLocation(myShader.Program, "model");
        //model = glm::rotate(model, (GLfloat)glfwGetTime() * 50.0f, glm::vec3(0.5f, 1.0f, 0.0f));
        
        //glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        
        GLint viewLoc = glGetUniformLocation(myShader.Program, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        
        GLint projectionLoc = glGetUniformLocation(myShader.Program, "projection");
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
        
        //绑定VAO
        glBindVertexArray(VAO);
        //glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        // glDrawArrays(GL_TRIANGLES, 0, 36);
        
        for (GLuint i = 0; i < 10; i++) {
            
            glm::mat4 model;
            model = glm::translate(model, cubePositions[i]);
            
            GLfloat angle = 20.0f * i;
            int key = (i%3==0 || i==0) ? 1 : 0;
            key = 0;
            model = glm::rotate(model, angle+ key * (GLfloat)glfwGetTime() * 50, glm::vec3(1.0f, 0.3f, 0.5f));
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
            glDrawArrays(GL_TRIANGLES, 0, 36);
            
            
        }
        
        glBindVertexArray(0);
        
        
        glfwSwapBuffers(window);
    }
    
    glDeleteVertexArrays(1,&VAO);
    glDeleteBuffers(1, &VBO);
    //glDeleteBuffers(1, &EBO);
    
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
    static float mix = 0.2f;
    static float angle = 0.0f;
    GLfloat cameraSpeed = 0.05f;
    
    if(action == GLFW_PRESS)
    {
        keys[key] = true;
    }
    else if(action == GLFW_RELEASE)
    {
        keys[key] = false;
    }
    //
    
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
    else if(key == GLFW_KEY_UP && action == GLFW_PRESS) {
        
        mix+=0.05;
        mix  = mix >= 1 ? 1:mix;
        glUniform1f( glGetUniformLocation(gShader->Program, "mixf"), mix);
        
    }
    else if(key == GLFW_KEY_DOWN && action == GLFW_PRESS) {
        mix-=0.05;
        mix  = mix <= 0 ? 0:mix;
        // glUniform1f( glGetUniformLocation(gShader->Program, "mixf"), mix);
        
        //glm::mat4 model;
        // 沿x轴旋转-55度
        *gModel = glm::rotate(*gModel, 5.0f, glm::vec3(0.5f, 1.0f, 0.0f));
        
        // 将矩阵传入着色器
        GLint modelLoc = glGetUniformLocation(gShader->Program, "model");
        // *gModel = glm::rotate(*gModel, angle, glm::vec3(0.0f, 0.0f, 1.0f));
        
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(*gModel));
    }
    else if(key == GLFW_KEY_W) {
        cameraPos += cameraSpeed * cameraFront;
        
    }
    else if(key == GLFW_KEY_S) {
        cameraPos -=cameraSpeed * cameraFront;
    }
    else if(key == GLFW_KEY_A) {
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    }
    else if(key == GLFW_KEY_D) {
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    }
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if(firstMouse) // 这个bool变量初始时是设定为true的
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    
    GLfloat xoffset = xpos - lastX;
    GLfloat yoffset = lastY - ypos; // 注意这里是相反的，因为y坐标是从底部往顶部依次增大的
    lastX = xpos;
    lastY = ypos;
    
    GLfloat sensitivity = 0.02f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;
    
    yaw   += xoffset;
    pitch += yoffset;
    
    if(pitch > 89.0f)
        pitch =  89.0f;
    if(pitch < -89.0f)
        pitch = -89.0f;
    
    glm::vec3 front;
    front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
    front.y = sin(glm::radians(pitch));
    front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
    
    std::cout<<front.x<<" : "<<front.y<<" : "<<front.z<<std::endl;
    cameraFront = glm::normalize(front);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    if(fov >= 1.0f && fov <= 45.0f)
        fov -= yoffset;
    if(fov <= 1.0f)
        fov = 1.0f;
    if(fov >= 45.0f)
        fov = 45.0f;
}

void do_movement()
{
    GLfloat cameraSpeed = 5.0f * deltaTime;
    
    if(keys[GLFW_KEY_W]) {
        cameraPos += cameraSpeed * cameraFront;
        
    }
    else if(keys[GLFW_KEY_S]) {
        cameraPos -=cameraSpeed * cameraFront;
    }
    else if(keys[GLFW_KEY_A]) {
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    }
    else if(keys[GLFW_KEY_D]) {
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    }
    
}
