Texture 纹理
---
纹理是一个2D图片（也可以是一个1D和3D的纹理）
效果如下

![](https://learnopengl-cn.github.io/img/01/06/textures.png)

为了能够映射到三角形上，需要三角形的每个顶点对应各自的**纹理坐标（**Texture Coordinate）
纹理在xy轴上范围是0-1（2D），使用纹理坐标获取纹理颜色叫**采样** 

如下图
![](https://learnopengl-cn.github.io/img/01/06/tex_coords.png)

纹理环绕方式
---

|环绕方式（Wrapping） | 描述 	|
|---					| --- 	|
|GL_REPEAT			| 重复纹理图像 |
|GL\_MIRRORED_REPEAT |和GL_REPEAT一样，每次图形是镜像放置的.|
|GL\_CLAMP_TO\_EDGE   |纹理坐标会被约束在0-1之前，超出的部份会重复纹理坐标的边缘，产生一种边缘被拉伸的效果|
|GL_CLAMP\_TO\_BORDER | 超出的坐标为用户指定的边缘颜色。|

如图
![](https://learnopengl-cn.github.io/img/01/06/texture_wrapping.png)

每个选项都可以用glTextParameter 函数来对一个单独的坐标轴设置（s,t,r==>x,y,z）
如

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

* 第一个参数指定了纹理坐标，上面是使用的2D纹理
* 第二个参数是指定设置的坐标轴
* 第三个参数是环绕方式
如果用GL_CLAMP_TO_BORDER,还需要指定一个边缘颜色，使用如下

`	float borderColor[] = {1.0f, 1.0f, 0.0f, 1.0f}; 
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);`
	

	
纹理过滤
---
纹理坐标不依赖于分辨率(Resolution)，它可以是任意浮点值，所以OpenGL需要知道怎样将纹理像素(Texture Pixel，也叫Texel，译注1)映射到纹理坐标。当你有一个很大的物体但是纹理的分辨率很低的时候这就变得很重要了。你可能已经猜到了，OpenGL也有对于纹理过滤(Texture Filtering)的选项。纹理过滤有很多个选项，但是现在我们只讨论最重要的两种：GL_NEAREST和GL_LINEAR。

1. GL_NEAREST(邻近过滤) 

	![](https://learnopengl-cn.github.io/img/01/06/filter_nearest.png)
2. GL_LINEAR(线性过滤)

	![](https://learnopengl-cn.github.io/img/01/06/filter_linear.png)
	
![](https://learnopengl-cn.github.io/img/01/06/texture_filtering.png)

当进行放大(Magnify)和缩小(Minify)的时候可以设置纹理过滤选项，可以使用glTextParameter来指定过滤方式

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	   

多线渐远纹理
---
它简单来说就是一系列的纹理图像，后一个纹理图像是前一个的二分之一。多级渐远纹理背后的理念很简单：距观察者的距离超过一定的阈值，OpenGL会使用不同的多级渐远纹理，即最适合物体的距离的那个。由于距离远，解析度不高也不会被用户注意到。同时，多级渐远纹理另一加分之处是它的性能非常好

![](https://learnopengl-cn.github.io/img/01/06/mipmaps.png)

可以用glGenerateMipmaps创建多级渐远文理

|过滤方式	|	描述|
| ---		|  --- |
|GL\_NEAREST\_MIPMAP_NEAREST|	使用最邻近的多级渐远纹理来匹配像素大小，并使用邻近插值进行纹理采样|
|GL\_LINEAR\_MIPMAP_NEAREST|	使用最邻近的多级渐远纹理级别，并使用线性插值进行采样|
|GL\_NEAREST\_MIPMAP_LINEAR|	在两个最匹配像素大小的多级渐远纹理之间进行线性插值，使用邻近插值进行采样|
|GL_LINEAR\_MIPMAP\_LINEAR|	在两个邻近的多级渐远纹理之间使用线性插值，并使用线性插值进行采样|

可以使用glTexParameteri设置过滤方式

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

放大过滤选项用设置为多级渐变无效，多级渐远过滤主要是使用纹理被缩小的情况，否则会产生一个GL_INVALID_ENUM错误代码

加载与创建纹理
---
我们使用SOIL来处理一些图片，对于SOIL的安装在单独介绍
glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
**第四个参数与GL_RG8 很相似。。。**

生成一个纹理的过程看起来像这样

	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	// 为当前绑定的纹理对象设置环绕、过滤方式
	...
	// 加载并生成纹理
	int width, height;
	unsigned char* image = SOIL_load_image("container.jpg", &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0); 
	
纹理单元
---
纹理单元是连续的，在一个着色器中可以使用多个纹理，着先激活对应的纹理单元，然后绑定，
就像这样

	glActiveTexture(GL_TEXTURE0); //在绑定纹理之前先激活纹理单元
	glBindTexture(GL_TEXTURE_2D, texture);
	
两个纹理单元的例子

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


渲染流程大致是，先绑定好两个纹理至对应的纹理单元，然后在着色器中定义采样器对应的哪个纹理单元

绘制小结：
**解绑时，只需解绑定VAO,VBO,EBO与VAO解绑顺充错乱会没有任何效果**





