#include<glad\glad.h>
#include<GLFW\glfw3.h>

#include<iostream>

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// 暂时将shader的源代码硬编码在C风格字符串中。在运行时动态编译
// 这个vertex shader接收一个vec3向量aPos作为输入(in)，设置输入变量的位置为0。并直接将其转换成vec4输出
const char* vertexShaderSource = "#version 330 core\n"
	"layout (location = 0) in vec3 aPos;\n"
	"void main()\n"
	"{\n"
	"	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
	"}\0";

// 这个fragment shader输出一个不透明的橘黄色vec4
const char* fragmentShaderSource = "#version 330 core\n"
	"out vec4 FragColor;\n"
	"void main()\n"
	"{\n"
	"	FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
	"}\0";

int main()
{
	// glfw initialize and configure。GLFW：创建OpenGL context和窗口的抽象
	// -----------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	

	// 创建并编译shader program
	// ------------------------
	// Vertex shader
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);	// 创建着色器对象，依然用ID来引用因此用unsigned int来存储
	// 把着色器源码附加到着色器对象上，然后进行编译
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	// 检查编译是否成功
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	
	// Fragment shader，片段着色器用于计算像素最后的颜色输出
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	// 检查是否成功，沿用前面的success和Log变量
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// 将编译的两个shader链接成一个着色器程序对象Shader Program Object
	unsigned int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);	// 附加到shader program object
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);	// 链接
	// 检查链接是否成功
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINK_FAILED\n" << infoLog << std::endl;
	}
	// 激活程序对象，删除shader
	// glUseProgram(shaderProgram);	// 每个着色器调用、渲染调用都会使用这个程序对象了，也就是之前写的shader会生效。放在渲染循环中使用
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	// 此时，已经把顶点数据发送给GPU，并且指示GPU如何在vertex shader和fragment shader中处理数据
	// OpenGL还不知道该如何解释内存中的顶点数据，以及如何将顶点数据连接到顶点着色器属性上


	// set up vertex data and buffers and configure vertex attributes
	// --------------------------------------------------------------
	// 创建顶点数据、存储在GPU内存中，用顶点缓冲对象VBO管理
	float vertices[] = {
		0.5f, 0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f,
		-0.5f, 0.5f, 0.0f
	};
	// 创建顶点索引，用索引缓冲对象EBO管理
	unsigned int indices[] = {
		0, 1, 3,
		1, 2, 3
	};

	// 通过顶点缓冲对象VBO管理显卡上的内存，存储顶点数据
	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	// 1. 绑定VAO
	glBindVertexArray(VAO);	
	// 2. 把顶点数组复制到缓冲中，供OpenGL使用
	glBindBuffer(GL_ARRAY_BUFFER, VBO);	// 把新创建的缓冲VBO绑定到GL_ARRAY_BUFFER，使用的任何（在GL_ARRAY_BUFFER目标上的）缓冲调用都会用来配置当前绑定的缓冲(VBO)
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);	// 把定义的顶点数据复制到缓冲的内存中
	// 3. 复制索引数组到索引缓冲，供OpenGL使用
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);																		
	// 4. 设置顶点属性指针。现在顶点数组和vertex shader都有了，需要指定如何匹配整个数组和shader的每次输入
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);	// 1:顶点属性的位置值，layout(location = 0); 2:顶点属性的大小，vec3; 3:数据类型
																					// 4:是否标准化到[0,1]，vertices不是无符号类型; 5:步长; 6:数据在buffer中的偏移量	
	glEnableVertexAttribArray(0);	// 以顶点属性位置值为参数，启用顶点属性，绑定到GL_ARRAY_BUFFER的VBO
									// 也就是每个顶点属性从一个VBO管理的内存中获得它的数据
	
	// 可选：解绑定
	glBindBuffer(GL_ARRAY_BUFFER, 0);	// 调用glVertexAttribPointer将VBO跟顶点属性绑定，因此之后可以将VBO跟GL_ARRAY_BUFFER解绑了
	glBindVertexArray(0);	// 之后也可以解绑VAO，这样其他VAO的调用就不会修改这个VAO
							// 但很少发生这种情况，因为修改其他VAO一定要调用glBindVertexArray，通常如果不必要，就不解绑VAO和VBO

	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);	// 用线框模式绘制三角形
	// glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);	// 改回默认模式（填充）

	// 在render loop中进行绘制
	// -----------------------
	while (!glfwWindowShouldClose(window))
	{
		// input
		// -----
		processInput(window);

		// render
		// ------
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// 绘制三角形
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);	// 因为只有一个VAO，实际上没有必要每次绑定它。这样写更加有组织
		// glDrawArrays(GL_TRIANGLES, 0, 3);	// 用顶点绘制
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);	// 用索引绘制。(绘制模式，绘制顶点个数，索引类型，偏移量)
		// glBindVertexArray(0);	// 没必要每次解绑

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	// 可选：释放分配过的对象
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteProgram(shaderProgram);

	// glfw: terminate, clearing all previously allocated GLFW resources
	// -----------------------------------------------------------------
	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}
