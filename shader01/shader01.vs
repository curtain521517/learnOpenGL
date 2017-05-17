
#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;

out vec3 ourColor;
out vec3 outPosition;
//改变水平偏移
uniform float offx;


void main()
{
    //-position.y 可以上下颠倒
	//1.2小题
    //gl_Position = vec4(position.x + offx, - position.y, position.z, 1.0);
    
    //3
    gl_Position = vec4(position, 1.0);
    outPosition = position;
    
	ourColor = color;
    
    
}

