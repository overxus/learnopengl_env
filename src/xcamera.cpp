#include <glad/glad.h>
#include <xcamera.h>
#include <xtime.h>


static float clamp(float v, float vmin, float vmax) {
	if (v < vmin) {
		return vmin;
	}
	else if (v > vmax) {
		return vmax;
	}
	else {
		return v;
	}
}


glm::mat4 xCamera::getViewMatrix() {
	return glm::lookAt(Position, Position + Front, Up);
}


void xCamera::moveForward() {
	float offset = MoveSpeed * xTime::deltaTime;
	Position += Front * offset;
}


void xCamera::moveBackward() {
	float offset = MoveSpeed * xTime::deltaTime;
	Position -= Front * offset;
}


void xCamera::moveUp() {
	float offset = MoveSpeed * xTime::deltaTime;
	Position += Up * offset;
}


void xCamera::moveDown() {
	float offset = MoveSpeed * xTime::deltaTime;
	Position -= Up * offset;
}


void xCamera::moveLeft() {
	float offset = MoveSpeed * xTime::deltaTime;
	Position -= Right * offset;
}


void xCamera::moveRight() {
	float offset = MoveSpeed * xTime::deltaTime;
	Position += Right * offset;
}


void xCamera::rotateByCursor(float xoffset, float yoffset) {
	Yaw += xoffset * MouseSensitivity;
	Pitch = clamp(Pitch + yoffset * MouseSensitivity, -89.0f, 89.0f);

	updateCameraPos();
}


void xCamera::scaleByCursor(float yoffset) {
	Zoom = clamp(Zoom - yoffset, 1.0f, 45.0f);
}


void xCamera::updateCameraPos() {
	glm::vec3 front;
	front.x = cos(glm::radians(Pitch)) * cos(glm::radians(Yaw));
	front.y = sin(glm::radians(Pitch));
	front.z = cos(glm::radians(Pitch)) * sin(glm::radians(Yaw));
	Front = glm::normalize(front);
	Right = glm::normalize(glm::cross(Front, WorldUp));
	Up = glm::normalize(glm::cross(Right, Front));
}