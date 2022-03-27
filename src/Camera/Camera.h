#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

using glm::vec3;

/* Define several possible options for camera movement.
   Used as abstraction to stay away form window-system specific input methods
*/
enum Camera_Movement
{
	FORWARD,
	BACKWARD,
	LEFT, 
	RIGHT
};

// Default Camera values.
#define YAW -90.0f
#define PITCH 0.0f
#define MOVEMENT_SPEED 2.5f
#define MOUSE_SENSITIVITY 0.1f
#define DEFAULT_FOV 45.0f

// Default Camera position (world center).
#define DEFAULT_CAM_POSITION vec3(0.0f, 0.0f, 0.0f)

// Default World Up Vector.
#define DEFAULT_WORLD_UP vec3(0.0f, 1.0f, 0.0f)

class Camera
{
private:

	// Camera attributes.
	vec3 Position;
	vec3 Front;
	vec3 Up;
	vec3 Right;
	vec3 WorldUp;

	// Euler angles.
	float Yaw;
	float Pitch;

	// Camera Settings.
	float MovementSpeed;
	float MouseSensitivity;
	float MouseZoomFOV;

public:

	/* Constructor with vectors. */
	Camera(vec3 position = DEFAULT_CAM_POSITION, vec3 up = DEFAULT_WORLD_UP, float yaw = YAW, float pitch = PITCH);

	/* Constructor with scalar values */
	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);

	/* Constructor with default values. */
	Camera();

	// Get View Matrix
	glm::mat4 GetViewMatrix() const;

	// Get Current FOV
	float GetCurrentFOV() const { return MouseZoomFOV; }

	// Process Keyboard Input.
	void ProcessKeyboard(Camera_Movement direction, float deltaTime);

	// Handle Mouse Input
	void ProcessMouseInput(float xOffset, float yOffset, GLboolean constrainPitch = true);

	// Process FOV
	void ProcessMouseScroll(const float yOffset);

private:
	
	// Calculate camera direction and location based on input values.
	void UpdateCameraVectors();

};