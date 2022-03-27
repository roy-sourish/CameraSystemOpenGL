#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/type_ptr.hpp>

#include "Shader/Shader.h"
#include "Camera/Camera.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <iostream>	


#pragma region CallbackFunctions

// Window resize and input callback functions.
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window, Shader& shaderProgram);

// Camera / Mouse movement callback functions.
void mouse_callback(GLFWwindow* window, double xPosIn, double yPosIn);
void scroll_callback(GLFWwindow* window, double xOffset, double yOffset);

#pragma endregion


// Window Context Settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
float textureInterpVal = 0.2f;

// Projection Matrix Settings.
float aspect = (float)SCR_WIDTH / (float)SCR_HEIGHT;
float zNear = 0.1f;
float zFar = 100.0f;

// Delta Time.
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// Initial mouse position.
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;


// --------- CAMERA ---------- //
#pragma region Camera

// Create Camera Object.
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

#pragma endregion


int main()
{

#pragma region InitWindow
	
	// Initialize and configure OpenGL vesion and profile.
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create window context
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "OpenGL Camera System", nullptr, nullptr);
	if (!window)
	{
		std::cout << "Failed to create GLFW window." << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	glfwSetCursorPosCallback(window, mouse_callback);

	glfwSetScrollCallback(window, scroll_callback);

	// Set Mouse Capture.
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Load all OpenGL function pointers.
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD\n";
		return -1;
	}

#pragma endregion


#pragma region Build_and_Compile_Shaders

	// Build and compile our shader programs.
	//---------------------------------------
	Shader shaderProgram("src/Shader/Vertex.shader", "src/Shader/Fragment.shader");

#pragma endregion


#pragma region VertexData

	// Configure global OpenGL state. Enable Depth Testing.
	glEnable(GL_DEPTH_TEST);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {

		-0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 0.0f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 0.0f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 0.0f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f,  0.0f, 1.0f
	};

	// world space coordinates for our cubes.
	//---------------------------------------
	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	// Vertex Array Object.
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);

	// bind vertex array obj first then bind and set vertex buffers.
	// and the configure vertex attributes.
	glBindVertexArray(VAO);

	// Vertex Buffer Object.
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Position Attrib.
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Color Attrib.
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Texture Attrib.
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

#pragma endregion


#pragma region LoadTextures

	// Base Texture / texture 1
	//-------------------------

	unsigned int texture1;
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);

	// Texture Wrapping.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Texture Filtering.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Load texture images.
	//---------------------

	// flip the y-axis during image loading.
	stbi_set_flip_vertically_on_load(true);

	int width, height, nrChannels;
	
	unsigned char* imageData = stbi_load("texture_1.jpg", &width, &height, &nrChannels, 0);
	if (imageData)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		cout << "FAILED TO LOAD TEXTURE-1\n";
	}
	stbi_image_free(imageData);

	// Texture 2 / Overlay texture.
	//-----------------------------

	unsigned int texture2;
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);

	// Texture Wrapping.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Texture Filtering.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Load Image.
	imageData = stbi_load("texture_2.png", &width, &height, &nrChannels, 0);
	if (imageData)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		cout << "FAILED TO LOAD TEXTURE-2\n";
	}
	stbi_image_free(imageData);

	// Tell OpenGL for each sampler to which texture unit it belongs to 
	shaderProgram.useShaderProgram();
	glUniform1i(glGetUniformLocation(shaderProgram.getShaderID(), "texture1"), 0);
	glUniform1i(glGetUniformLocation(shaderProgram.getShaderID(), "texture2"), 1);
	glUniform1f(glGetUniformLocation(shaderProgram.getShaderID(), "textureInterp"), textureInterpVal);


#pragma endregion


#pragma region RenderLoop

	while (!glfwWindowShouldClose(window))
	{
		// Calculate Delta Time.
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Bind Textures.
		//---------------
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		// Handle Keyboard Inputs.
		processInput(window, shaderProgram);

		// Projection Matrix.
		//-------------------
		glm::mat4 projectionMatrix = glm::perspective(glm::radians(camera.GetCurrentFOV()), aspect, zNear, zFar);
		unsigned int pMatLocation = glGetUniformLocation(shaderProgram.getShaderID(), "projectionMatrix");
		glUniformMatrix4fv(pMatLocation, 1, GL_FALSE, &projectionMatrix[0][0]);

		// View Matrix.
		//-------------
		glm::mat4 viewMatrix = camera.GetViewMatrix();
		unsigned int vMatLocation = glGetUniformLocation(shaderProgram.getShaderID(), "viewMatrix");
		glUniformMatrix4fv(vMatLocation, 1, GL_FALSE, &viewMatrix[0][0]);


		// Bind Vertex Array Object before any draw calls.
		glBindVertexArray(VAO);

		// Draw 10 cubes in different location from the world origin.
		//-----------------------------------------------------------

		for (int i = 0; i < 10; i++)
		{
			// Model Matrix.
			//--------------
			glm::mat4 modelMatrix = glm::mat4(1.0f);	// Identity Mat.

			// translate each cube to a new position.
			modelMatrix = glm::translate(modelMatrix, cubePositions[i]);

			unsigned int mMatLocation = glGetUniformLocation(shaderProgram.getShaderID(), "modelMatrix");
			glUniformMatrix4fv(mMatLocation, 1, GL_FALSE, &modelMatrix[0][0]);

			// Draw Vertices.
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		// Swap buffers and poll IO events.
		//---------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// De-allocate all resources once they've outlived their purpose.
	//---------------------------------------------------------------
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram.getShaderID());

	// Clear all previously allocated resources.
	//------------------------------------------
	glfwTerminate();

#pragma endregion

	return 0;
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}


void processInput(GLFWwindow* window, Shader& shaderProgram)
{
	// Terminate on press Escape.
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window ,true);


#pragma region TextureInterp

	// Interpolate the Second Texture's blending.

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		textureInterpVal = ((textureInterpVal + 0.0001f) < 1.0f) ? (textureInterpVal + 0.0001f) : 1.0f;
		glUniform1f(glGetUniformLocation(shaderProgram.getShaderID(), "textureInterp"), textureInterpVal);
	}

	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		textureInterpVal = ((textureInterpVal - 0.0001f) > 0.0f) ? (textureInterpVal - 0.0001f) : 0.0f;
		glUniform1f(glGetUniformLocation(shaderProgram.getShaderID(), "textureInterp"), textureInterpVal);
	}

#pragma endregion


#pragma region CameraMovement

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);

#pragma endregion

}


void mouse_callback(GLFWwindow* window, double xPosIn, double yPosIn)
{
	float xpos = static_cast<float>(xPosIn);
	float ypos = static_cast<float>(yPosIn);

	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xOffset = xpos - lastX;
	float yOffset = lastY - ypos;

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseInput(xOffset, yOffset);
}


void scroll_callback(GLFWwindow* window, double xOffset, double yOffset)
{
	camera.ProcessMouseScroll(static_cast<float>(yOffset));
}
