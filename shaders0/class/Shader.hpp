//
//  Shader.hpp
//  shaders0
//
//  Created by xufan on 2017/5/17.
//  Copyright © 2017年 xufan. All rights reserved.
//

#ifndef Shader_hpp
#define Shader_hpp

#include <stdio.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <GL/glew.h>

class Shader
{
public:
    
    //程序ID
    GLuint Program;
    
    //构造着色器
    Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
    
    //使用程序
    void Use();
    
};

#endif /* Shader_hpp */
