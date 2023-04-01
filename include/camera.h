#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum CameraMove {
	UP, DOWN,
	LEFT, RIGHT,
	FORWARD, BACKWARD
};

static const float YAW = -90.0f;
static const float PITCH = 0.0f;
static const float SPEED = 2.5f;
static const float SENSITIVITY = 0.1f;
static const float ZOOM = 45.0f;


class Camera {
public:
	glm::vec3 Position;  // 相机所处的坐标
	glm::vec3 Front, Up, Right;  // 相机的前/上/右方方向向量
	glm::vec3 WorldUp;  // 指向世界的上方
	float Yaw, Pitch;  // 欧拉角
	float MoveSpeed, MouseSensitivity, Zoom;

	Camera(glm::vec3 position, glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
		float yaw = YAW, float pitch = PITCH)
		: Front(glm::vec3(0.0f, 0.0f, -1.0f)), MoveSpeed(SPEED),
		MouseSensitivity(SENSITIVITY), Zoom(ZOOM) {
		Position = position;
		WorldUp = up;
		Yaw = yaw;
		Pitch = pitch;
		updateCameraPos();
	}
	
	glm::mat4 getViewMatrix() {
		return glm::lookAt(Position, Position + Front, Up);
	}

	void processKeyboard(CameraMove direction, float deltaTime) {
		float velocity = MoveSpeed * deltaTime;
		switch (direction) {
		case UP: Position += WorldUp * velocity; break;
		case DOWN: Position -= WorldUp * velocity; break;
		case LEFT: Position -= Right * velocity; break;
		case RIGHT: Position += Right * velocity; break;
		case FORWARD: Position += Front * velocity; break;
		case BACKWARD: Position -= Front * velocity; break;
		default: break;
		}
	}

	void processMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true) {
		xoffset *= MouseSensitivity;
		yoffset *= MouseSensitivity;
		Yaw += xoffset;
		Pitch += yoffset;

		if (constrainPitch) {
			if (Pitch > 89.0f) Pitch = 89.0f;
			if (Pitch < -89.0f) Pitch = -89.0f;
		}

		updateCameraPos();
	}

	void processMouseScroll(float yoffset) {
		Zoom -= yoffset;
		if (Zoom < 1.0f) Zoom = 1.0f;
		if (Zoom > 45.0f) Zoom = 45.0f;
	}

private:
	void updateCameraPos() {
		glm::vec3 front;
		front.x = cos(glm::radians(Pitch)) * cos(glm::radians(Yaw));
		front.y = sin(glm::radians(Pitch));
		front.z = cos(glm::radians(Pitch)) * sin(glm::radians(Yaw));
		Front = glm::normalize(front);
		Right = glm::normalize(glm::cross(Front, WorldUp));
		Up = glm::normalize(glm::cross(Right, Front));
	}
};

#endif