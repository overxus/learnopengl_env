#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <shader.h>
#include <camera.h>
#include <model.h>

#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


void framebuffersize_callback(GLFWwindow* window, int width, int height);
void cursorpos_callback(GLFWwindow* window, double xposIn, double yposIn);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);


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

Camera g_camera(glm::vec3(0.0f, 0.0f, 3.0f));

struct {
	glm::vec3 position = glm::vec3(1.2f, 1.0f, 2.0f);
} g_light;

bool init_glfw_and_glad();

int main() {
	if (! init_glfw_and_glad()) {
		return -1;
	}

	// tell stb_image.h to flip loaded texture's on the y-axis (before loading model).
    stbi_set_flip_vertically_on_load(true);

    Shader shader("shaders//model.vs", "shaders//model.fs");
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

	g_window.window = glfwCreateWindow(g_window.width, g_window.height, "LearnOpenGL", NULL, NULL);
	if (g_window.window == NULL) {
		std::cout << "ERROR failed to create GLFW window\n";
		glfwTerminate();
		return false;
	}
	glfwMakeContextCurrent(g_window.window);
	glfwSetFramebufferSizeCallback(g_window.window, framebuffersize_callback);
	glfwSetCursorPosCallback(g_window.window, cursorpos_callback);
	glfwSetScrollCallback(g_window.window, scroll_callback);

	// glfwSetInputMode(g_window.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// init GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "ERROR failed to initialize GLAD\n";
		return false;
	}

	glEnable(GL_DEPTH_TEST);

	return true;
}


void framebuffersize_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}


void cursorpos_callback(GLFWwindow* window, double xposIn, double yposIn) {
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

	g_camera.processMouseMovement(xoffset, yoffset);
}


void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	g_camera.processMouseScroll(static_cast<float>(yoffset));
}


void processInput(GLFWwindow *window) {
	float deltaTime = 1 / g_window.FPS;
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		g_camera.processKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		g_camera.processKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		g_camera.processKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		g_camera.processKeyboard(RIGHT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		g_camera.processKeyboard(UP, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		g_camera.processKeyboard(DOWN, deltaTime);
}
