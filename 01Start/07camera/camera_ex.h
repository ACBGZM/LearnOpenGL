#pragma once
#ifndef CAMERA_EX_H
#define CAMERA_EX_H

#include<glad\glad.h>
#include<glm\glm.hpp>
#include<glm\gtc\matrix_transform.hpp>

#include<vector>


// Defines several possible options for camera movement.
// Used as abstraction to stay away from window-system specific input method.

enum Camera_Movement
{
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	UP,
	DOWN
};

// Default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;


bool FPSMode = false;	// 练习1，FPS相机一直保持在地面上，也就是 Position.y = 0.0f


// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL.
class Camera
{
public:
	// camera attributes
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;
	// euler angles
	float Yaw;
	float Pitch;
	// camera options
	float MovementSpeed;
	float MouseSensitivity;
	float Zoom;

	// constructor with vectors
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) :Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
	{
		Position = position;
		WorldUp = up;
		Yaw = yaw;
		Pitch = pitch;
		updateCameraVectors();
	}

	// constructor with scalar values
	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) :Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
	{
		Position = glm::vec3(posX, posY, posZ);
		WorldUp = glm::vec3(upX, upY, upZ);
		Yaw = yaw;
		Pitch = pitch;
		updateCameraVectors();
	}

	// returns the view matrix calculated using Euler Angles and the LookAt Matrix
	glm::mat4 GetViewMatrix()
	{
		return MyLookAt(Position, Position + Front, Up);
	}

	// processes input received from any keyboard-like input system.
	// Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
	void ProcessKeyboard(Camera_Movement direction, float deltaTime)
	{
		float velocity = deltaTime * MovementSpeed;
		if (direction == FORWARD) Position += Front * velocity;
		if (direction == BACKWARD) Position -= Front * velocity;
		if (direction == LEFT) Position -= Right * velocity;
		if (direction == RIGHT) Position += Right * velocity;
		if (direction == UP) Position += Up * velocity;
		if (direction == DOWN) Position -= Up * velocity;
		
		if(FPSMode) Position.y = 0.0f;
	}

	// processes input received from a mouse input system.
	// Expects the offset value in both the x and y direction.
	void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
	{
		xoffset *= MouseSensitivity;
		yoffset *= MouseSensitivity;

		Yaw += xoffset;
		Pitch += yoffset;

		// make sure that when pitch is out of bounds, screen doesn't get flipped
		if (constrainPitch)
		{
			if (Pitch > 89.0f) Pitch = 89.0f;
			if (Pitch < -89.0f) Pitch = -89.0f;
		}

		// update Front, Right and Up vectors using the updated Euler angles
		updateCameraVectors();
	}

	// processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
	void ProcessMouseScroll(float yoffset)
	{
		Zoom -= (float)yoffset;
		if (Zoom < 1.0f)
			Zoom = 1.0f;
		if (Zoom > 45.0f)
			Zoom = 45.0f;
	}

	// LookAt(position, position + front, up)，position + front 就是 target，up 是相机的上方向
	glm::mat4 MyLookAt(glm::vec3 position, glm::vec3 target, glm::vec3 up)
	{
		// 定义camera坐标系的三个轴
		glm::vec3 direction = glm::normalize(position - target);	// 跟指向方向相反
		
		glm::vec3 worldup = glm::vec3(0.0f, 1.0f, 0.0f);	// 先定义一个上向量，来计算right。直接用WorldUp也可
		glm::vec3 right = glm::normalize(glm::cross(worldup, direction));

		// 参数up就是相机的上方向，在updateCameraVectors()中计算。也可以通过下面的语句手动计算
		// glm::vec3 up = glm::normalize(glm::cross(direction, right));
		

		// glm::mat4 是按列排列的，因为opengl在显存中是按列存储的。mat4[i][j]表示第i列第j行
		glm::mat4 translation;
		translation[3][0] = -position.x;
		translation[3][1] = -position.y;
		translation[3][2] = -position.z;

		glm::mat4 rotation;
		for (unsigned int i = 0;i < 3;i++) rotation[i][0] = right[i];
		for (unsigned int i = 0;i < 3;i++) rotation[i][1] = up[i];
		for (unsigned int i = 0;i < 3;i++) rotation[i][2] = direction[i];

		return rotation * translation;
	}

private:

	void updateCameraVectors()
	{
		// calculate the new Front vector
		glm::vec3 front;
		front.x = cos(glm::radians(Pitch)) * cos(glm::radians(Yaw));
		front.y = sin(glm::radians(Pitch));
		front.z = cos(glm::radians(Pitch)) * sin(glm::radians(Yaw));
		Front = glm::normalize(front);
		// also re-calculate the Right and Up vector
		Right = glm::normalize(glm::cross(Front, WorldUp));
		Up = glm::normalize(glm::cross(Right, Front));
	}
};
#endif // !CAMERA_H
