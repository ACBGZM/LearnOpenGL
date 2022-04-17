#include<glad\glad.h>
#include<GLFW\glfw3.h>

#include<iostream>

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}

// �Դ���ע��һ���ص�����(Callback Function)����ÿ�δ��ڴ�С��������ʱ�򱻵���
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);	// �����ӿڣ�ǰ���������Ǵ������½ǵ�λ�ã������������ش�С
}

int main()
{
	// ��ʼ��GLFW��GLFW������OpenGL context�ʹ��ڵĳ���
	glfwInit();

	// ����glfwWindowHint����������GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);	// ����GLFW��ʹ��OpenGL�İ汾��3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);	// ʹ�ú���ģʽ core-profile
	// glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);	// APPLE OS

	// ����һ�����ڶ��󣬴�����и�������ص����ݣ����һᱻGLFW����������Ƶ���õ�
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);	// �����괰�ں�֪ͨGLFW��window��context����Ϊ��ǰ�̵߳���context
	
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);	// �Դ���ע��ص�����������


	// �ڵ����κ�OpenGL����ǰ�����ȳ�ʼ��GLAD��GLAD������OS����OpenGL�ĺ���ָ�롣
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// glViewport(0, 0, 800, 600);	


	// ���GLFW�Ƿ�Ҫ���˳�
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		glfwSwapBuffers(window);	// ������ɫ���塣ǰ���屣�����������ͼ����Ⱦָ���ں󻺳���ƣ���Ⱦָ��ִ����Ϻ���н�����������ʱ��Ⱦ��ͼ����˸����
		glfwPollEvents();	// ������޴����¼�����������롢����ƶ��������´��ڶ�̬�������ö�Ӧ�Ļص�����
	}

	glfwTerminate();
	return 0;
}