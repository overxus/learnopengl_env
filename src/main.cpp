#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <xshader.h>
#include <xcamera.h>
#include <xlog.h>
#include <model.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


void framebuffersizeCallback(GLFWwindow* window, int width, int height);
void cursorposCallback(GLFWwindow* window, double xposIn, double yposIn);
void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);


struct {
	unsigned int width = 800;
	unsigned int height = 600;
	GLFWwindow* window = NULL;
	float FPS = 30.0f;
} g_window;

struct {
	float lastX = 0.0f;
	float lastY = 0.0f;
	bool hasTriggeredCursor = false;
} g_cursor;

xCamera g_camera(glm::vec3(0.0f, 0.0f, 3.0f));

struct {
	glm::vec3 position = glm::vec3(1.2f, 1.0f, 2.0f);
} g_light;

bool init_glfw_and_glad();

int main() {
	if (!init_glfw_and_glad()) {
		return -1;
	}

	// tell stb_image.h to flip loaded texture's on the y-axis (before loading model).
	stbi_set_flip_vertically_on_load(true);

	xShader shader("shaders//model.vs", "shaders//model.fs");
	Model robot("nanosuit//nanosuit.obj");


	float lastTime = 0.0f;
	while (!glfwWindowShouldClose(g_window.window)) {
		float currentTime = static_cast<float>(glfwGetTime());
		if (currentTime - lastTime < 1 / g_window.FPS) continue;
		lastTime = currentTime;

		processInput(g_window.window);

		glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader.use();
		glm::mat4 projection = glm::perspective(glm::radians(g_camera.Zoom),
			(float)g_window.width / (float)g_window.height, 0.1f, 100.0f);
		glm::mat4 view = g_camera.getViewMatrix();
		shader.setMat4("projection", projection);
		shader.setMat4("view", view);

		// render the loaded model
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
		shader.setMat4("model", model);
		robot.Draw(shader);

		glfwSwapBuffers(g_window.window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

bool init_glfw_and_glad() {
	// init GLFW
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	g_window.window = glfwCreateWindow(g_window.width, g_window.height, "Learn OpenGL", NULL, NULL);
	if (g_window.window == NULL) {
		glfwTerminate();
		xLog::error("failed to create GLFW window");
	}
	glfwMakeContextCurrent(g_window.window);
	glfwSetFramebufferSizeCallback(g_window.window, framebuffersizeCallback);
	glfwSetCursorPosCallback(g_window.window, cursorposCallback);
	glfwSetScrollCallback(g_window.window, scrollCallback);

	// glfwSetInputMode(g_window.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// init GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		xLog::error("failed to initialize GLAD");
	}

	glEnable(GL_DEPTH_TEST);

	return true;
}


void framebuffersizeCallback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}


void cursorposCallback(GLFWwindow* window, double xposIn, double yposIn) {
	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);

	if (!g_cursor.hasTriggeredCursor) {
		g_cursor.hasTriggeredCursor = true;
		g_cursor.lastX = xpos;
		g_cursor.lastY = ypos;
	}

	float xoffset = xpos - g_cursor.lastX;
	float yoffset = g_cursor.lastY - ypos;
	g_cursor.lastX = xpos;
	g_cursor.lastY = ypos;

	g_camera.rotateByCursor(xoffset, yoffset);
}


void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
	g_camera.scaleByCursor(static_cast<float>(yoffset));
}


void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		g_camera.moveForward();
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		g_camera.moveBackward();
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		g_camera.moveLeft();
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		g_camera.moveRight();
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		g_camera.moveUp();
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		g_camera.moveDown();
}
