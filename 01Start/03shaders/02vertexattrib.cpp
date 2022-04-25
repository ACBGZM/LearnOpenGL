// 着色器：一种把输入转化为输出的的小程序，运行在GPU上,为图形渲染管线的某个特定部分而运行
// 着色器是一种非常独立的程序，它们之间不能相互通信，只有通过输入和输出进行沟通

// 一个典型的GLSL着色器结构：声明版本，输入输出变量，uniform，main函数
/*
#version version_number
in type in_variable_name;
out type out_variable_name;
uniform type uniform_name;
int main()
{
	// 处理输入，进行一些图形操作
	// 输出处理过的结果到输出变量
	out_variable_name = weird_stuff_we_processed;
}
*/

#include<glad\glad.h>
#include<GLFW\glfw3.h>

#include<iostream>

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// 顶点着色器，从顶点数据中直接接收输入，用location定义顶点数据如何管理
const char* vertexShaderSource =
"#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"		// 位置变量的属性位置为0
"layout (location = 1) in vec3 aColor;\n"	// 颜色变量的属性位置为1
"out vec3 vertexColor;\n"		// 为片段着色器指定一个颜色输出。两个着色器之间发送数据
"void main()\n"
"{\n"
"	gl_Position = vec4(aPos, 1.0f);\n"	// 可以把vec3直接作为vec4构造器的参数
"	vertexColor = aColor;\n"	// 设置输出变量
"}\0";

/*
// 片段着色器，需要一个vec4颜色输出变量
const char* fragmentShaderSource =
"#version 330 core\n"
"out vec4 FragColor;\n"
"in vec4 vertexColor;\n"		// 接收顶点着色器传来的输入变量，要求名称、类型都相同
"void main()\n"
"{\n"
"	FragColor = vertexColor;\n"	// 使用vertex shader传入的变量vertexColor
"}\0";
*/

const char* fragmentShaderSource =
"#version 330 core\n"
"out vec4 FragColor;\n"
"in vec3 vertexColor;\n"	// 接收顶点着色器从VBO解析的颜色的顶点属性
"void main()\n"
"{\n"
"	FragColor = vec4(vertexColor, 1.0f);\n"
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


	// build and compile our shader program
	// ------------------------------------
	// Vertex shader
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	// check for shader compile errors
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// Fragment shaders
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	// check for shader compile errors
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// link the program
	unsigned int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);	// 附加到shader program object
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);	// 链接
	// check for linking errors
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// set up vertex data and buffers and configure vertex attributes
	// --------------------------------------------------------------
	float vertices[] = {
		// position			//color
		0.5f, -0.5f, 0.0f,	1.0f, 0.0f, 0.0f,	// bottom right
		-0.5f, -0.5f, 0.0f,	0.0f, 1.0f, 0.0f,	// bottom left
		0.0f,  0.5f, 0.0f,	0.0f, 0.0f, 1.0f	// top 
	};

	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// 顶点的位置属性，layout(location = 0)
	// glVertexAttribPointer(index, size, type, normalized, stride, pointer偏移量)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// 顶点的颜色属性，layout(location = 1)
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	// glBindVertexArray(0);


	// bind the VAO (it was already bound, but just to demonstrate): seeing as we only have a single VAO we can 
	// just bind it beforehand before rendering the respective triangle; this is another approach.
	glBindVertexArray(VAO);

	// uncomment this call to draw in wireframe polygons.
	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);	// 用线框模式绘制三角形
	// glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);	// 改回默认模式（填充）

	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		// input
		// -----
		processInput(window);

		// render
		// ------
		glClearColor(0.8f, 0.8f, 0.0f, 0.3f);
		glClear(GL_COLOR_BUFFER_BIT);

		// be sure to activate the shader before any calls to glUniform
		glUseProgram(shaderProgram);

		// render the triangle
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
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



