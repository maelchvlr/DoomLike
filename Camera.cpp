#include "Camera.h"

Camera::Camera(int width, int height, glm::vec3 position, GLuint *shaderProgram) 
{
	Camera::rb = new RigidBody(10.0f, 0.0f, true, glm::vec3(0.5, 1, 0.5), position, glm::vec3(0.0f));
	shader = *shaderProgram;
	Camera::width = width;
	Camera::height = height;
}

void Camera::Matrix(float FOVdeg, float nearPlane, float farPlane, const char* uniform, Shader* shaderProgram, float dt)
{
	rb->startSimulate();
	rb->update(dt);

	//std::cout << "Camera position: " << rb->position.x << " " << rb->position.y << " " << rb->position.z << std::endl;

	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);

	//Update the view matrix depending of the orientation and the direction of the camera
	view = glm::lookAt(rb->position, rb->position + Orientation, Up);
	projection = glm::perspective(glm::radians(FOVdeg), (float)(width / height), nearPlane, farPlane);

	//Pass the matrices to the shader
	glUniformMatrix4fv(glGetUniformLocation(shader, uniform), 1, GL_FALSE, glm::value_ptr(projection * view));
}

void Camera::Inputs(GLFWwindow* window) {
	bool anyKeyPressed = false;
	bool isSpacePressed = glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS;

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		rb->velocity.x += speed * Orientation.x;
		rb->velocity.z += speed * Orientation.z;
		anyKeyPressed = true;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		rb->velocity.x += speed * -Orientation.x;
		rb->velocity.z += speed * -Orientation.z;
		anyKeyPressed = true;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		rb->velocity += speed * -glm::normalize(glm::cross(Orientation, Up));
		anyKeyPressed = true;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		rb->velocity += speed * glm::normalize(glm::cross(Orientation, Up));
		anyKeyPressed = true;
	}
		
	// Jump only on the frame the spacebar is pressed
	if (isSpacePressed && rb->velocity.y == 0) {
		rb->velocity += glm::vec3(0, speed * 25.0f, 0); // Modify the y component for the jump
		anyKeyPressed = true;
	}

	if (!anyKeyPressed) {
		rb->velocity.x *= 0.95f;
		rb->velocity.z *= 0.95f;
	}

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