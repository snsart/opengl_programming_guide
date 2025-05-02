#include <iostream>

#define GLEW_STATIC
#include <GL/glew.h>//opengl 跨平台接口
#include <GLFW/glfw3.h>//窗口管理类
#include "Shader.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Camera.h"
#include "Material.h"
#include "DirectionLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Mesh.h"
#include "Model.h"
#include <map>
#include "Skybox.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
float lastX = 800 / 2.0f;
float lastY = 600 / 2.0f;
bool firstMouse = true;
bool mousePress = false;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

#pragma  region model data
float vertices[] = {
	// positions          // normals           // texture coords
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
};

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

float transparentVertices[] = {
	// positions         // texture Coords (swapped y coordinates because texture is flipped upside down)
	0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
	0.0f, -0.5f,  0.0f,  0.0f,  1.0f,
	1.0f, -0.5f,  0.0f,  1.0f,  1.0f,

	0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
	1.0f, -0.5f,  0.0f,  1.0f,  1.0f,
	1.0f,  0.5f,  0.0f,  1.0f,  0.0f
};

float rectangleVertices[] = {
	// positions         // texture Coords (swapped y coordinates because texture is flipped upside down)
	-1.0f,  1.0f,  0.0f,  0.0f,  0.0f,
	-1.0f, 0.0f,  0.0f,  0.0f,  1.0f,
	-0.0f, 0.0f,  0.0f,  1.0f,  1.0f,

	-1.0f,  1.0f,  0.0f,  0.0f,  0.0f,
	-0.0f, 0.0f,  0.0f,  1.0f,  1.0f,
	-0.0f,  1.0f,  0.0f,  1.0f,  0.0f
};

std::vector<glm::vec3> vegetation;

#pragma endregion

unsigned int loadImageToGPU(const char* filename, GLint internalFormat, GLenum format, unsigned int textureSlot ) {
	unsigned int texture;
	glGenTextures(1, &texture);
	glActiveTexture(GL_TEXTURE0+textureSlot);
	glBindTexture(GL_TEXTURE_2D, texture);

	int width, height, nrchannels;
	stbi_set_flip_vertically_on_load(false);
	unsigned char* data = stbi_load(filename, &width, &height, &nrchannels, 0);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		printf("load image failed!");
	}

	stbi_image_free(data);
	return texture;
}

Camera* camera = new Camera();

int main(int argc,char* argv[]) {
	std::string exePath = argv[0];

	vegetation.push_back(glm::vec3(-1.5f, 0.0f, -0.48f));
	vegetation.push_back(glm::vec3(1.5f, 0.0f, 0.51f));
	vegetation.push_back(glm::vec3(0.0f, 0.0f, 0.7f));
	vegetation.push_back(glm::vec3(-0.3f, 0.0f, -2.3f));
	vegetation.push_back(glm::vec3(0.5f, 0.0f, -0.6f));

	#pragma region  open a window
	glfwInit();
	//设置glfw版本号为3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//设置opengl版本
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//创建opengl 窗口
	GLFWwindow* window = glfwCreateWindow(800, 600, "my opengl game", NULL, NULL);
	if (window == NULL) {
		printf("open window failed");
		glfwTerminate();
		return -1;
	}
	//设置程序上下文为当前窗口
	glfwMakeContextCurrent(window);

	//Init GLEW
	glewExperimental = true;
	if (glewInit() != GLEW_OK) {
		printf("Init glew failed");
		glfwTerminate();
		return -1;
	}
	glViewport(0, 0, 800, 600);


#pragma endregion

	#pragma region input callback
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
#pragma endregion

	#pragma region init shader program
	Shader* myShader = new Shader("vertexSource.vert", "fragmentSource.frag");
	Shader* singleColorShader = new Shader("scaleVertexSource.vert", "singleColor.frag");
	Shader* frameBufferShader = new Shader("frameBufferVertex.vert", "frameBuffer.frag");
	Shader* skyboxShader = new Shader("skyboxVert.vert", "skybox.frag");
	#pragma endregion
	
	#pragma region init and load model to VBO,VAO
	//Mesh Cube(vertices);
	Model backpack(exePath.substr(0, exePath.find_last_of('\\')) + "\\model\\backpack\\backpack.obj");
	Skybox skybox(exePath.substr(0, exePath.find_last_of('\\')) + "\\model\\skybox");


	unsigned int VAO,VBO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(transparentVertices), transparentVertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(2);

	unsigned int rectangleVAO,rectangleVBO;
	glGenVertexArrays(1, &rectangleVAO);
	glBindVertexArray(rectangleVAO);

	glGenBuffers(1, &rectangleVBO);
	glBindBuffer(GL_ARRAY_BUFFER, rectangleVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rectangleVertices), rectangleVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	
	#pragma endregion

	#pragma region 创建帧缓冲
	unsigned int frameBufferID;
	glGenFramebuffers(1, &frameBufferID);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBufferID);

	unsigned int texColorBuffer;
	glGenTextures(1, &texColorBuffer);
	glBindTexture(GL_TEXTURE_2D, texColorBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texColorBuffer, 0);

	unsigned int renderBufferObject;
	glGenRenderbuffers(1, &renderBufferObject);
	glBindRenderbuffer(GL_RENDERBUFFER, renderBufferObject);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 800, 600);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderBufferObject);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	#pragma endregion

	unsigned int grassID;
	grassID = loadImageToGPU("blending_transparent_window.png", GL_RGBA, GL_RGBA, 0);

	#pragma region create material

	Material* myMaterial = new Material(
		glm::vec3(0.5f, 0.5f, 0.5f), 
		loadImageToGPU("container2.png", GL_RGBA, GL_RGBA, 0), 
		loadImageToGPU("container2_specular.png", GL_RGBA, GL_RGBA, 1),
		32);
	#pragma endregion

	#pragma region prepare MVP matrices
	
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 projection;
	projection = glm::perspective(glm::radians(45.0f), (float)800 / 600, 0.1f, 100.0f);

	#pragma endregion
	
	DirectionLight* directionLight = new DirectionLight();
	PointLight* pointLight = new PointLight(glm::vec3(0.0f,0.0f,0.0f),glm::vec3(1.0f,1.0f,1.0f));
	SpotLight* spotLight = new SpotLight();

	while (!glfwWindowShouldClose(window)) {

		std::map<float, glm::vec3> sorted;
		for (unsigned int i = 0; i < vegetation.size(); i++) {
			float distance = glm::length(camera->Position - vegetation[i]);
			sorted[distance] = vegetation[i];
		}

		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		
		processInput(window);

		//glDepthMask(GL_FALSE);
		//glDepthFunc(GL_GREATER);
		
		spotLight->Position = camera->Position;
		spotLight->Direction = camera->Front;
		
		//glBindFramebuffer(GL_FRAMEBUFFER, frameBufferID);
		//glEnable(GL_DEPTH_TEST);
		//glEnable(GL_STENCIL_TEST);
		//glClearColor(0.2f, 0.3f, 0.4f, 1.0f);
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		//glEnable(GL_BLEND);
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		//步骤一：将要画的物体所覆盖的片段的模板缓冲设置为1
		//glStencilFunc(GL_ALWAYS, 1, 0xFF);
		//glStencilMask(0xFF);
		//	

		glEnable(GL_DEPTH_TEST);
		myShader->use();
		glm::mat4 model;
		view = camera->getViewMatrix();
		glUniform3f(glGetUniformLocation(myShader->ID, "viewPos"), camera->Position.x, camera->Position.y, camera->Position.z);
		glUniformMatrix4fv(glGetUniformLocation(myShader->ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(glGetUniformLocation(myShader->ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(myShader->ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		myShader->setUniform3f("directionLight.color", directionLight->LightColor);
		myShader->setUniform3f("directionLight.direction", directionLight->Direction);

		myShader->setUniform3f("pointLight.color", pointLight->LightColor);
		myShader->setUniform3f("pointLight.position", pointLight->Position);
		myShader->setUniform1f("pointLight.constant", 1.0f);
		myShader->setUniform1f("pointLight.linear", 0.09f);
		myShader->setUniform1f("pointLight.quadratic", 0.032f);

		myShader->setUniform3f("spotLight.position", spotLight->Position);
		myShader->setUniform3f("spotLight.color", spotLight->LightColor);
		myShader->setUniform3f("spotLight.direction", spotLight->Direction);
		myShader->setUniform1f("spotLight.cutOff", spotLight->CutOff);
		myShader->setUniform1f("spotLight.outerCutOff", spotLight->OuterCutOff);
		myShader->setUniform1f("spotLight.constant", 1.0f);
		myShader->setUniform1f("spotLight.linear", 0.09f);
		myShader->setUniform1f("spotLight.quadratic", 0.032f);

		glActiveTexture(GL_TEXTURE4);
		myShader->setUniform1i("skybox", 4);
		glBindTexture(GL_TEXTURE_CUBE_MAP, skybox.textureID);
		backpack.Draw(myShader); 
		//

		//singleColorShader->use();
		//glBindVertexArray(VAO);
		//view = camera->getViewMatrix();
		//glUniformMatrix4fv(glGetUniformLocation(singleColorShader->ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
		//glUniformMatrix4fv(glGetUniformLocation(singleColorShader->ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		//for (std::map<float,glm::vec3>::reverse_iterator it = sorted.rbegin();it!=sorted.rend();it++)
		//{
		//	glm::mat4 model2;
		//	model2 = glm::mat4(1.0f);
		//	model2 = glm::translate(model2, it->second);
		//	glUniformMatrix4fv(glGetUniformLocation(singleColorShader->ID, "model"), 1, GL_FALSE, glm::value_ptr(model2));
		//	glActiveTexture(GL_TEXTURE0);
		//	glBindTexture(GL_TEXTURE_2D, grassID);
		//	singleColorShader->setUniform1i("texture1",0);
		//	glDrawArrays(GL_TRIANGLES, 0, 6);
		//}

		//glBindFramebuffer(GL_FRAMEBUFFER, 0);

		//glEnable(GL_DEPTH_TEST);
		//glEnable(GL_STENCIL_TEST);
		//glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//glEnable(GL_BLEND);
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		//glStencilFunc(GL_ALWAYS, 1, 0xFF);
		//glStencilMask(0xFF);

		//singleColorShader->use();
		//glBindVertexArray(VAO);
		//view = camera->getViewMatrix();
		//glUniformMatrix4fv(glGetUniformLocation(singleColorShader->ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
		//glUniformMatrix4fv(glGetUniformLocation(singleColorShader->ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		//for (std::map<float, glm::vec3>::reverse_iterator it = sorted.rbegin(); it != sorted.rend(); it++)
		//{
		//	glm::mat4 model2;
		//	model2 = glm::mat4(1.0f);
		//	model2 = glm::translate(model2, it->second);
		//	glUniformMatrix4fv(glGetUniformLocation(singleColorShader->ID, "model"), 1, GL_FALSE, glm::value_ptr(model2));
		//	glActiveTexture(GL_TEXTURE0);
		//	glBindTexture(GL_TEXTURE_2D, grassID);
		//	singleColorShader->setUniform1i("texture1", 0);
		//	glDrawArrays(GL_TRIANGLES, 0, 6);
		//}

		//frameBufferShader->use();
		//glActiveTexture(GL_TEXTURE1);
		//glBindTexture(GL_TEXTURE_2D, texColorBuffer);
		//frameBufferShader->setUniform1i("texture1", 1);
		//glBindVertexArray(rectangleVAO);
		//glDrawArrays(GL_TRIANGLES, 0, 6);

#pragma region draw border
			//步骤二：画边框：将物体放大一圈，画物体时不是1的片段会通过模板测试，从而画出一个单色边框（内部被步骤一设置为了1，因而会被剔除）
			//glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
			//glStencilMask(0x00);
			//glDisable(GL_DEPTH_TEST);
			//singleColorShader->use();
			//glUniformMatrix4fv(glGetUniformLocation(singleColorShader->ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
			//glUniformMatrix4fv(glGetUniformLocation(singleColorShader->ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
			////model = glm::scale(model, glm::vec3(1.2f));
			//glUniformMatrix4fv(glGetUniformLocation(singleColorShader->ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
			//backpack.Draw(singleColorShader);
			//glStencilMask(0xFF);
			//glEnable(GL_DEPTH_TEST);
#pragma endregion
	
		//画天空盒
		glDepthFunc(GL_LEQUAL);
		skyboxShader->use();
		glm::mat4 skyboxview = glm::mat4(glm::mat3(camera->getViewMatrix()));
		glUniformMatrix4fv(glGetUniformLocation(skyboxShader->ID, "view"), 1, GL_FALSE, glm::value_ptr(skyboxview));
		glUniformMatrix4fv(glGetUniformLocation(skyboxShader->ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		skybox.draw(skyboxShader);
		glDepthFunc(GL_LESS);
		
		//获取鼠标事件和切换buffer
		glfwPollEvents();
		glfwSwapBuffers(window);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	glfwTerminate();
	return 0;
}

#pragma region input declare
void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		camera->processKeyboard(FORWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		camera->processKeyboard(BACKWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		camera->processKeyboard(LEFT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		camera->processKeyboard(RIGHT, deltaTime);
	}
	mousePress = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS;
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

//glfw: whenever the mouse moves, this callback is called
void mouse_callback(GLFWwindow* window,double xposIn,double yposIn){
	if (!mousePress) {
		firstMouse = true; 
		return;
	}
	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);
	if (firstMouse) {
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}
	float xoffset = xpos - lastX;
	float yoffset = ypos - lastY;
	lastX = xpos;
	lastY = ypos;
	camera->processMouseMovement(xoffset, yoffset, true);
}

//glfw: whenever the mouse scroll wheel scrolls,this callback is called;
void scroll_callback(GLFWwindow* window,double xoffset,double yoffset) {
	float yoffsetf = static_cast<float>(yoffset);
}

#pragma endregion



