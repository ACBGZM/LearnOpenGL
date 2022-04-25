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

// 对窗口定义一个回调函数(Callback Function)，在每次窗口大小被调整的时候被调用
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);	// 定义视口，前两个参数是窗口左下角的位置，后两个是像素大小
}

int main()
{
	// 初始化GLFW。GLFW：创建OpenGL context和窗口的抽象
	glfwInit();

	// 调用glfwWindowHint函数来配置GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);	// 告诉GLFW，使用OpenGL的版本是3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);	// 使用核心模式 core-profile

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);	// APPLE OS
#endif // __APPLE__

	// 创建一个窗口对象，存放所有跟窗口相关的数据，而且会被GLFW的其他函数频繁用到
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);	// 创建完窗口后，通知GLFW将window的context设置为当前线程的主context
	
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);	// 对窗口注册回调函数。这个函数在每次窗口尺寸修改时被调用，修改glviewport的尺寸

	// 在调用任何OpenGL函数前必须先初始化GLAD。GLAD：根据不同的OS，管理OpenGL的函数指针。
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}


	// 渲染循环。循环条件是检查GLFW是否被要求退出
	// 所有的渲染操作都放到渲染循环中
	while (!glfwWindowShouldClose(window))
	{
		// 输入
		processInput(window);


		// 渲染指令
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);	//	设置清空屏幕所用的颜色（状态设置函数）
		glClear(GL_COLOR_BUFFER_BIT);	// 清空屏幕的颜色缓冲（状态使用函数）
		/*
		glClear函数来清空屏幕的颜色缓冲，它接受一个缓冲位(Buffer Bit)来指定要清空的缓冲，
		可能的缓冲位有GL_COLOR_BUFFER_BIT，GL_DEPTH_BUFFER_BIT和GL_STENCIL_BUFFER_BIT
		*/


		// 检查并调用事件，交换缓冲
		glfwSwapBuffers(window);	// 交换颜色缓冲。前缓冲保存最终输出的图像，渲染指令在后缓冲绘制，渲染指令执行完毕后进行交换，避免临时渲染的图像闪烁问题
		glfwPollEvents();	// 检查有无触发事件（如键盘输入、鼠标移动），更新窗口动态，并调用对应的回调函数
	}

	glfwTerminate();	// 渲染循环结束后，正确释放之前分配的所有资源
	return 0;
}