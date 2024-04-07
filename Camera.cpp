#include "Camera.h"

Camera::Camera(int width, int height, glm::vec3 position, GLuint *shaderProgram) 
{
	Camera::rb = new RigidBody(0.0f, 0.0f, false, glm::vec3(0.5, 1, 0.5), position, glm::vec3(0.0f));
	shader = *shaderProgram;
	Camera::width = width;
	Camera::height = height;
}

void Camera::Matrix(float FOVdeg, float nearPlane, float farPlane, const char* uniform)
{
	//Initialize the spawn position and orientation of the camera
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);

	//Update the view matrix depending of the orientation and the direction of the camera
	view = glm::lookAt(rb->position, rb->position + Orientation, Up);
	projection = glm::perspective(glm::radians(FOVdeg), (float)(width / height), nearPlane, farPlane);

	//Pass the matrices to the shader
	glUniformMatrix4fv(glGetUniformLocation(shader, uniform), 1, GL_FALSE, glm::value_ptr(projection * view));
}

void Camera::Inputs(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		rb->position += speed * Orientation;

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		rb->position += speed * -Orientation;

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		rb->position += speed * -glm::normalize(glm::cross(Orientation, Up));

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		rb->position += speed * glm::normalize(glm::cross(Orientation, Up));

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		rb->position += speed * Up;

	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
		rb->position += speed * -Up;

	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		speed = 0.4f;
	else if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
		speed = 0.1f;

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

		if (firstClick) {
			glfwSetCursorPos(window, (width / 2), (height / 2));
			firstClick = false;
		}

		double mouseX;
		double mouseY;
		glfwGetCursorPos(window, &mouseX, &mouseY);

		//Normalize the coordinates to start at the center of the screen
		float rotX = sensitivity * (float)(mouseY - height / 2) / height;
		float rotY = sensitivity * (float)(mouseX - height / 2) / height;

		//To prevent barrel roll
		glm::vec3 newOrientation = glm::rotate(Orientation, glm::radians(-rotX), glm::normalize(glm::cross(Orientation, Up)));

		if (!((glm::angle(newOrientation, Up) <= glm::radians(5.0f)) or (glm::angle(newOrientation, -Up) <= glm::radians(5.0f)))) {
			Orientation = newOrientation;
		}

		Orientation = glm::rotate(Orientation, glm::radians(-rotY), Up);

		glfwSetCursorPos(window, (width / 2), (height / 2));
	}
	else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		firstClick = true;
	}
}