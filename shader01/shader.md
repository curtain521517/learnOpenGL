shader 着色器
====
着色器是运行在GPU上面的小程序，这些小程序为图形渲染的某个特定部份而运行，从基本意义上来说，着色器只一种把输入转换为输出的程序。

着色器的开头声明版本，接着输入和输出变量，uniform和main函数。
一个典型的着色器有下面的结构

	#version version_number
	
	in type in_variable_name;
	in type in_variable_name;
	
	out type out_variable_name;
	
	uniform type uniform_name;
	
	int main()
	{
	  // 处理输入并进行一些图形操作
	  ...
	  // 输出处理过的结果到输出变量
	  out_variable_name = weird_stuff_we_processed;
	}
	

顶点着色器的输入变量也叫__顶点属性（Vertex Attribute）__,它由硬件来决定，OpenGL确保至少有16个包含4分量的顶点属性可以用，可以用GL_MAX_VERTEX_ATTRIBS来查询

	GLint nrAttributes;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;
	
__数据类型__
------
GLSL中包含C等其它语言大部份的默认的数据类型，如`int`,`float`,`double`,`uint`,`bool`.GLSL也有两种容器类型，矩阵（**Matrix**）,向量（**Vector**）

####向量
| 类型 		| 说明 		|
| ---		| ---		|
| vecn    |包含n个float分量的的默认向量|
| bvecn	|包含n个bool分量的向量|
| ivecn	| 包含n个int分量的向量 |
| uvecn	| 包含n个unsigned int 分量的向量|
| dvecn   | 包含n个double分量的向量|

输入与输出
----
顶点着色器接收的是一种特殊行式的输入，顶点着色器的输入特殊在，它从顶点数据中直接接收输入。为了定义顶点数据该如何管理，我们使用location这一元数据指定输入变量，这样我们才可以在CPU上配置顶点属性。我们已经在前面的教程看过这个了，layout (location = 0)。顶点着色器需要为它的输入提供一个额外的layout标识，这样我们才能把它链接到顶点数据。
另一个例外是片段着色器，它需要一个vec4颜色输出变量，因为片段着色器需要生成一个最终输出的颜色。如果你在片段着色器没有定义输出颜色，OpenGL会把你的物体渲染为黑色（或白色）。

所以，如果我们打算从一个着色器向另一个着色器发送数据，我们必须在发送方着色器中声明一个输出，在接收方着色器中声明一个类似的输入。当类型和名字都一样的时候，OpenGL就会把两个变量链接到一起，它们之间就能发送数据了（这是在链接程序对象时完成的）。
