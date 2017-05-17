
#version 330 core

in vec3 ourColor;
in vec3 outPosition;  //输入顶点位置

out vec4 color;

void main()
{
    
	color = vec4(ourColor, 1.0f);
    
    //顶点颜色与顶点位置一样的，测试
    color = vec4(outPosition, 1.0f);
}
