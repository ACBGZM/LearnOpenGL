#include<glad\glad.h>
#include<GLFW\glfw3.h>

#include<iostream>

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// ��ʱ��shader��Դ����Ӳ������C����ַ����С�������ʱ��̬����
// ���vertex shader����һ��vec3����aPos��Ϊ����(in)���������������λ��Ϊ0����ֱ�ӽ���ת����vec4���
const char* vertexShaderSource = "#version 330 core\n"
	"layout (location = 0) in vec3 aPos;\n"
	"void main()\n"
	"{\n"
	"	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
	"}\0";

// ���fragment shader���һ����͸�����ٻ�ɫvec4
const char* fragmentShaderSource = "#version 330 core\n"
	"out vec4 FragColor;\n"
	"void main()\n"
	"{\n"
	"	FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
	"}\0";

int main()
{
	// glfw initialize and configure��GLFW������OpenGL context�ʹ��ڵĳ���
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
	

	// ����������shader program
	// ------------------------
	// Vertex shader
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);	// ������ɫ��������Ȼ��ID�����������unsigned int���洢
	// ����ɫ��Դ�븽�ӵ���ɫ�������ϣ�Ȼ����б���
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	// �������Ƿ�ɹ�
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	
	// Fragment shader��Ƭ����ɫ�����ڼ�������������ɫ���
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	// ����Ƿ�ɹ�������ǰ���success��Log����
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// �����������shader���ӳ�һ����ɫ���������Shader Program Object
	unsigned int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);	// ���ӵ�shader program object
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);	// ����
	// ��������Ƿ�ɹ�
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINK_FAILED\n" << infoLog << std::endl;
	}
	// ����������ɾ��shader
	// glUseProgram(shaderProgram);	// ÿ����ɫ�����á���Ⱦ���ö���ʹ�������������ˣ�Ҳ����֮ǰд��shader����Ч��������Ⱦѭ����ʹ��
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	// ��ʱ���Ѿ��Ѷ������ݷ��͸�GPU������ָʾGPU�����vertex shader��fragment shader�д�������
	// OpenGL����֪������ν����ڴ��еĶ������ݣ��Լ���ν������������ӵ�������ɫ��������


	// set up vertex data and buffers and configure vertex attributes
	// --------------------------------------------------------------
	// �����������ݡ��洢��GPU�ڴ��У��ö��㻺�����VBO����
	float vertices[] = {
		0.5f, 0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f,
		-0.5f, 0.5f, 0.0f
	};
	// ���������������������������EBO����
	unsigned int indices[] = {
		0, 1, 3,
		1, 2, 3
	};

	// ͨ�����㻺�����VBO�����Կ��ϵ��ڴ棬�洢��������
	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	// 1. ��VAO
	glBindVertexArray(VAO);	
	// 2. �Ѷ������鸴�Ƶ������У���OpenGLʹ��
	glBindBuffer(GL_ARRAY_BUFFER, VBO);	// ���´����Ļ���VBO�󶨵�GL_ARRAY_BUFFER��ʹ�õ��κΣ���GL_ARRAY_BUFFERĿ���ϵģ�������ö����������õ�ǰ�󶨵Ļ���(VBO)
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);	// �Ѷ���Ķ������ݸ��Ƶ�������ڴ���
	// 3. �����������鵽�������壬��OpenGLʹ��
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);																		
	// 4. ���ö�������ָ�롣���ڶ��������vertex shader�����ˣ���Ҫָ�����ƥ�����������shader��ÿ������
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);	// 1:�������Ե�λ��ֵ��layout(location = 0); 2:�������ԵĴ�С��vec3; 3:��������
																					// 4:�Ƿ��׼����[0,1]��vertices�����޷�������; 5:����; 6:������buffer�е�ƫ����	
	glEnableVertexAttribArray(0);	// �Զ�������λ��ֵΪ���������ö������ԣ��󶨵�GL_ARRAY_BUFFER��VBO
									// Ҳ����ÿ���������Դ�һ��VBO������ڴ��л����������
	
	// ��ѡ�����
	glBindBuffer(GL_ARRAY_BUFFER, 0);	// ����glVertexAttribPointer��VBO���������԰󶨣����֮����Խ�VBO��GL_ARRAY_BUFFER�����
	glBindVertexArray(0);	// ֮��Ҳ���Խ��VAO����������VAO�ĵ��þͲ����޸����VAO
							// �����ٷ��������������Ϊ�޸�����VAOһ��Ҫ����glBindVertexArray��ͨ���������Ҫ���Ͳ����VAO��VBO

	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);	// ���߿�ģʽ����������
	// glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);	// �Ļ�Ĭ��ģʽ����䣩

	// ��render loop�н��л���
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

		// ����������
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);	// ��Ϊֻ��һ��VAO��ʵ����û�б�Ҫÿ�ΰ���������д��������֯
		// glDrawArrays(GL_TRIANGLES, 0, 3);	// �ö������
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);	// ���������ơ�(����ģʽ�����ƶ���������������ͣ�ƫ����)
		// glBindVertexArray(0);	// û��Ҫÿ�ν��

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	// ��ѡ���ͷŷ�����Ķ���
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
