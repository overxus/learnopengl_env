#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
/* xCamera: 摄像机类
 *		getViewMatrix()  计算摄像机的view矩阵
 *		moveXXX()  控制摄像机的移动
 *		rotateByCurosr  由鼠标控制的旋转
 *		scaleByCurosr  由鼠标滚轮控制的缩放
 */


class xCamera {
public:
	glm::vec3 Position;
	glm::vec3 Front, Up, Right;
	float Yaw, Pitch;
	float Zoom;

	xCamera(glm::vec3 position, float yaw = -90.0f, float pitch = 0.0f) :
		Position{ position }, Front(glm::vec3(0.0f, 0.0f, -1.0f)),
		Yaw(yaw), Pitch(pitch), Zoom(45.0f) {
		updateCameraPos();
	}

	glm::mat4 getViewMatrix();

	void moveForward();
	void moveBackward();
	void moveUp();
	void moveDown();
	void moveLeft();
	void moveRight();

	void rotateByCursor(float xoffset, float yoffset);
	void scaleByCursor(float yoffset);

private:
	const glm::vec3 WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);
	const float MoveSpeed = 2.5f;
	const float MouseSensitivity = 0.1f;
	void updateCameraPos();
};
