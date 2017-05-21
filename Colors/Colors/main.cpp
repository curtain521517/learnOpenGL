//
//  main.cpp
//  Colors
//
//  Created by xufan on 2017/5/20.
//  Copyright © 2017年 xufan. All rights reserved.
//

#include <iostream>

#define GLEW_STATIC
#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <shader/Shader.hpp>
#include <camera/Camera.hpp>

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


Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
glm::vec3 lightPos(0.0f, 0.0f, 1.0f);




GLfloat vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    
    0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
    0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
    0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
    0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
    0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
    0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
    
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
    0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
    0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
    
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
    0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
    0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
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
    //启动深度测试，z轴处理
    glEnable(GL_DEPTH_TEST);

    
    glewExperimental = GL_TRUE;
    glewInit();
    
    int screenWidth, screenHeight;
    glfwGetFramebufferSize(window, &screenWidth, &screenHeight);
    glViewport(0, 0, screenWidth, screenHeight);
    
    //初始化shader
    Shader lampShader("/Users/xufan/learnOpenGL/Colors/lamp.vs","/Users/xufan/learnOpenGL/Colors/lamp.frag");
    Shader lightingShader("/Users/xufan/learnOpenGL/Colors/lighting.vs","/Users/xufan/learnOpenGL/Colors/lighting.frag");
    
    
    GLuint VBO, containerVAO;
    glGenVertexArrays(1, &containerVAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(containerVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    
    glBindVertexArray(0);
    
    GLuint lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
    
    while (!glfwWindowShouldClose(window)) {
        
        glfwPollEvents();
        do_movement();
        
        GLfloat currentTime = glfwGetTime();
        deltaTime = currentTime - lastFrame;
        lastFrame = currentTime;
        
        //缓冲颜色
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        //清空颜色缓冲
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        lightPos = glm::vec3(0.0f, 0.0f, 1.0f);

        glm::mat4 m = glm::mat4();
        m = glm::rotate(m, currentTime * 100 , glm::vec3(2.0, 2.0, -2.0));
        glm::vec4 nPos = m * glm::vec4(lightPos, 1.0);
        lightPos.x = nPos.x;
        lightPos.y = nPos.y;
        lightPos.z = nPos.z;
        
        //
        lightingShader.Use();
        GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
        
        GLint matAmbientLoc = glGetUniformLocation(lightingShader.Program, "material.ambient");
        GLint matDiffuseLoc = glGetUniformLocation(lightingShader.Program, "material.diffuse");
        GLint matSpecularLoc = glGetUniformLocation(lightingShader.Program, "material.specular");
        GLint matShineLoc = glGetUniformLocation(lightingShader.Program, "material.shininess");
        
        GLint lightPositionLoc = glGetUniformLocation(lightingShader.Program, "light.position");
        GLint lightAmbientLoc = glGetUniformLocation(lightingShader.Program, "light.ambient");
        GLint lightDiffuseLoc = glGetUniformLocation(lightingShader.Program, "light.diffuse");
        GLint lightSpecularLoc = glGetUniformLocation(lightingShader.Program, "light.specular");
        
        glUniform3f(viewPosLoc, camera.Position.x, camera.Position.y, camera.Position.z);

        glUniform3f(matAmbientLoc, 1.0f, 0.5f, 0.31f);
        glUniform3f(matDiffuseLoc, 1.0f, 0.5f, 0.31f);
        glUniform3f(matSpecularLoc, 0.5f, 0.5f, 0.5f);
        glUniform1f(matShineLoc, 32.0f);
        
        glUniform3f(lightPositionLoc,lightPos.x, lightPos.y, lightPos.z);
        
        glm::vec3 lightColor;
        lightColor.x = sin(glfwGetTime() * 2.0f);
        lightColor.y = sin(glfwGetTime() * 0.7f);
        lightColor.z = sin(glfwGetTime() * 1.3f);
        
        glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f);
        glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f);
        
        glUniform3f(lightAmbientLoc, ambientColor.x, ambientColor.y, ambientColor.z);
        glUniform3f(lightDiffuseLoc, diffuseColor.x, diffuseColor.y, diffuseColor.z);
        glUniform3f(lightSpecularLoc, 1.0f, 1.0f, 1.0f);




        glm::mat4 view;
        view = camera.GetViewMatrix();
        
        glm::mat4 projection;
        projection = glm::perspective(camera.Zoom, screenWidth / screenHeight * 1.0f, 0.1f, 100.0f);
        
        GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
        GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
        GLint projectionLoc = glGetUniformLocation(lightingShader.Program, "projection");
        
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
        
        //绘制被照的物体
        glBindVertexArray(containerVAO);
        glm::mat4 model;
        //model =  glm::rotate(model, 55.0f, glm::vec3(1.0f,0.0f,0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        
        //
        lampShader.Use();
        modelLoc = glGetUniformLocation(lampShader.Program, "model");
        viewLoc = glGetUniformLocation(lampShader.Program, "view");
        projectionLoc = glGetUniformLocation(lampShader.Program, "projection");

        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

        model = glm::mat4();
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.2f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        
        //绘制灯泡
        glBindVertexArray(lightVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        
        
        glfwSwapBuffers(window);
    }
    
    //glDeleteVertexArrays(1,&containerVAO);
    //glDeleteVertexArrays(1,&lightVAO);
    //glDeleteBuffers(1, &VBO);
    
    glfwTerminate();
    
    return 0;
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
    
    else if(key == GLFW_KEY_W) {
        camera.ProcessKeyboard(FORWARD, deltaTime);
        
    }
    else if(key == GLFW_KEY_S) {
        camera.ProcessKeyboard(BACKWARD, deltaTime);

    }
    else if(key == GLFW_KEY_A) {
        camera.ProcessKeyboard(LEFT, deltaTime);

    }
    else if(key == GLFW_KEY_D) {
        camera.ProcessKeyboard(RIGHT, deltaTime);
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
    
    camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}

void do_movement()
{
    GLfloat cameraSpeed = 5.0f * deltaTime;
    
    if(keys[GLFW_KEY_W]) {
        camera.ProcessKeyboard(FORWARD, deltaTime);
        
    }
    else if(keys[GLFW_KEY_S]) {
        camera.ProcessKeyboard(BACKWARD, deltaTime);
        
    }
    else if(keys[GLFW_KEY_A]) {
        camera.ProcessKeyboard(LEFT, deltaTime);
        
    }
    else if(keys[GLFW_KEY_D]) {
        camera.ProcessKeyboard(RIGHT, deltaTime);
    }
}
