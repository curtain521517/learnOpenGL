Hello, 三角形
===
>
* 顶点数组对象：Vertex Array Object, VAO
* 顶点缓冲对象：Vertex Buffer Object,VBO
* 索引缓冲对象：Element Buffer Object, EBO 或Index Buffer Object

下图是一个图形渲染管线的每个阶段的抽象展示。要注意蓝色部分代表的是我们可以注入的自定义的着色器的部分。
![](https://learnopengl-cn.github.io/img/01/04/pipeline.png)


>
**标准化设备坐标(Normalized Device Coordinates,NDC)**
一旦你的顶点坐标已经在顶点着色器中处理过，它们就应该是标准化设备坐标了，标准化设备坐标是一个x、y和z值在-1.0到1.0的一小段空间。任何落在范围外的坐标都会被丢弃/裁剪，不会显示在你的屏幕上。下面你会看到我们定义的在标准化设备坐标中的三角形(忽略z轴)：
![](https://learnopengl-cn.github.io/img/01/04/ndc.png)
与通常的屏幕坐标不同，y轴正方向为向上，(0, 0)坐标是这个图像的中心，而不是左上角。最终你希望所有(变换过的)坐标都在这个坐标空间中，否则它们就不可见了。
你的标准化设备坐标接着会变换为屏幕空间坐标(Screen-space Coordinates)，这是使用你通过glViewport函数提供的数据，进行视口变换(Viewport Transform)完成的。所得的屏幕空间坐标又会被变换为片段输入到片段着色器中。

**1.顶点数组对象 vertex array object, VAO**

1. 使用glGenVertexArrays生成相应的对象
2. 绑定VAO

示例代码如下

	 GLuint VAO = glGenVertexArrays(1, &VAO);
	    
**2.顶点缓冲对象 vertex buffer object, VBO**

1. 使用glGenBuffers函数可以生成相应的对象
2. 使用glBufferData函数复制数据到相应的目标中

示例代码如下

    GLfloat vertices[] = {
        0.5f, 0.5f, 0.0f,   // 右上角
        0.5f, -0.5f, 0.0f,  // 右下角
        -0.5f, -0.5f, 0.0f, // 左下角
        -0.5f, 0.5f, 0.0f   // 左上角
    };
    
    GLuint VBO = glGenBuffers(1,&VBO);
    //绑定VBO到 GL_ARRAY_BUFFER中
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //复制数据
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices,GL_STATIC_DRAW);
 
**3.索引缓冲对象 Index/Element buffer object, EBO**

1. 使用glBuffers生成相应的对象
2. 使用glBufferData复制数据（同VBO）

示例代码如下

    GLuint indices[] = { // 注意索引从0开始!
        0, 1, 3, // 第一个三角形
        1, 2, 3  // 第二个三角形
    };
    
    GLuint EBO = glGenBuffers(1, &EBO);
    //绑定
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //复制
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices),indices,GL_STATIC_DRAW);

**4.配置顶点指针属性**

1. glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid\*)0);

	* 第一个参数指定需要配置的顶点属性
	* 第二个参数指定顶点属性的大小
	* 第三个参数指定数据类型
	* 下个参数定义我们是否希望数据被标准化(Normalize)。如果我们设置为GL_TRUE，所有数据都会被映射到0（对于有符号型signed数据是-1）到1之间。我们把它设置为GL_FALSE。
	* 第五个参数叫做步长(Stride)，它告诉我们在连续的顶点属性组之间的间隔。由于下个组位置数据在3个GLfloat之后，我们把步长设置为3 * sizeof(GLfloat)。
	* 最后一个是偏移量
2. glEnableVertexAttribArray(0);
	启用位置为0的顶点属性，默认是关闭的

**5.解绑VAO**

1. glBindVertexArray(0);



最后一连串的代码就是这样

    GLfloat vertices[] = {
        0.5f, 0.5f, 0.0f,   // 右上角
        0.5f, -0.5f, 0.0f,  // 右下角
        -0.5f, -0.5f, 0.0f, // 左下角
        -0.5f, 0.5f, 0.0f   // 左上角
    };
    
    GLuint indices[] = { // 注意索引从0开始!
        0, 1, 3, // 第一个三角形
        1, 2, 3  // 第二个三角形
    };
    
    GLuint VBO,EBO,VAO;
    
    //生成相应的对象，第一个参数是数量，第二个参数是接收地址，可以一次性生成多个对象，如glGenVertexArrays(2, VAOs);VAOs是类型为GLuint的数组名

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1,&VBO);
    glGenBuffers(1,&EBO);
    
    //绑定VAO
    glBindVertexArray(VAO);

    //绑定VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //复制VBO数据
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices,GL_STATIC_DRAW);
    //绑定EBO数据
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //复制EBO数据
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices),indices,GL_STATIC_DRAW);
    
    //设置顶点属性指针
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE, 3 * sizeof(GL_FLOAT),(GLvoid *)0);
    
    //启动顶点属性0位置的，对应shader里面的location
    glEnableVertexAttribArray(0);
    
    //解绑
    glBindVertexArray(0);
    
    //线框模式，
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    
    while (!glfwWindowShouldClose(window)) {
        
        glfwPollEvents();
        
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        glUseProgram(shaderProgram);
        
        //绑定
        glBindVertexArray(VAO);
        //绘制
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        //解绑
        glBindVertexArray(0);
        
        glfwSwapBuffers(window);
    }








 
 


    
    
    
    
    

 





