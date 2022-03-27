#include "Camera.h"

Camera::Camera(vec3 position, vec3 up, float yaw, float pitch)
{
	Position = position;
	WorldUp = up;
	Yaw = yaw;
	Pitch = pitch;

	Front = vec3(0.0f, 0.0f, -1.0f); // Z axis
	MovementSpeed = MOVEMENT_SPEED;
	MouseSensitivity = MOUSE_SENSITIVITY;
	MouseZoomFOV = DEFAULT_FOV;

	UpdateCameraVectors();
}


Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch)
{
	Position = vec3(posX, posY, posZ);
	WorldUp = vec3(upX, upY, upZ);
	Yaw = yaw;
	Pitch = pitch;

	Front = vec3(0.0f, 0.0f, -1.0f); // Z axis
	MovementSpeed = MOVEMENT_SPEED;
	MouseSensitivity = MOUSE_SENSITIVITY;
	MouseZoomFOV = DEFAULT_FOV;

	UpdateCameraVectors();
}

Camera::Camera()
{
	Position = vec3(0.0f, 0.0f, 3.0f);
	WorldUp = DEFAULT_WORLD_UP;
	Yaw = YAW;
	Pitch = PITCH;

	Front = vec3(0.0f, 0.0f, -1.0f); // Z axis
	MovementSpeed = MOVEMENT_SPEED;
	MouseSensitivity = MOUSE_SENSITIVITY;
	MouseZoomFOV = DEFAULT_FOV;

	UpdateCameraVectors();
}


glm::mat4 Camera::GetViewMatrix() const
{
	return glm::lookAt(Position, Position + Front, Up);
}


void Camera::ProcessKeyboard(Camera_Movement direction, float deltaTime)
{
	 
	float movementSpeed = MovementSpeed * deltaTime;

	if (direction == FORWARD)
		Position += Front * movementSpeed;

	if (direction == BACKWARD)
		Position -= Front * movementSpeed;

	if (direction == LEFT)
		Position -= Right * movementSpeed;

	if (direction == RIGHT)
		Position += Right * movementSpeed;
}


void Camera::ProcessMouseInput(float xOffset, float yOffset, GLboolean constrainPitch)
{
	xOffset *= MouseSensitivity;
	yOffset *= MouseSensitivity;

	Yaw += xOffset;
	Pitch += yOffset;

	if (constrainPitch)
	{
		if (Pitch > 89.0f)
			Pitch = 89.0f;

		if (Pitch < -89.0f)
			Pitch = -89.0f;
	}

	// Update new rotations.
	UpdateCameraVectors();
}


void Camera::ProcessMouseScroll(const float yOffset)
{
	MouseZoomFOV -= yOffset;

	if (MouseZoomFOV < 1.0f)
		MouseZoomFOV = 1.0f;

	if (MouseZoomFOV > 45.0f)
		MouseZoomFOV = 45.0f;
}


void Camera::UpdateCameraVectors()
{
	vec3 newFront;
	newFront.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	newFront.y = sin(glm::radians(Pitch));
	newFront.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));

	Front = glm::normalize(newFront);

	// Recalculate the right and up vector of the camera from camera origin 
	Right = glm::normalize(glm::cross(Front, WorldUp));
	Up = glm::normalize(glm::cross(Right, Front));
}


