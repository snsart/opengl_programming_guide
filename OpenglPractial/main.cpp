
#pragma warning(disable: 26495)

#include <iostream>

#define GLEW_STATIC
#include <GL/glew.h>//opengl 跨平台接口
#include <GLFW/glfw3.h>//窗口管理类
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "Shader.h"
#include "Model.h"
#include "Camera.h"
#include "Skybox.h"
#include <vector>

struct CubeFrameBuffer {
	unsigned int* fbos;       // 帧缓冲对象数组
	unsigned int* rbos;       // 渲染缓冲对象数组
	unsigned int cubeMap;     // 立方体贴图
};

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

CubeFrameBuffer createCubeFrameBuffers(int size);
void deleteCubeFrameBuffer(CubeFrameBuffer& fb);

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

float vertices[] = {
	0.0f,0.5f,0.0f,
	-0.5f,-0.5f,0.0f,
	0.5f,-0.5f,0.0f
};

Camera* camera = new Camera();

int main(int argc, char* argv[])
{
	std::string exePath = argv[0];
	#pragma region  open a window
	glfwInit();
	//设置glfw版本号为4.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//设置opengl版本
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//创建opengl 窗口
	GLFWwindow* window = glfwCreateWindow(800, 600, "my opengl game", NULL, NULL);
	if (window == NULL) {
		printf("open window failed");
		glfwTerminate();
	}
	//设置程序上下文为当前窗口
	glfwMakeContextCurrent(window);

	//Init GLEW
	glewExperimental = true;
	if (glewInit() != GLEW_OK) {
		printf("Init glew failed");
		glfwTerminate();
	}
	glViewport(0, 0, 800, 600);
	glClearColor(0.2f, 0.3f, 0.2f, 1.0f);

	#pragma endregion

	#pragma region input callback
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
#pragma endregion

	Shader* bubbleShader = new Shader("bubbleVert.vert", "bubbleFrag.frag");
	Shader* skyboxShader = new Shader("skyboxVert.vert", "skybox.frag");
	Shader* normalLineShader = new Shader("normalLine.vert", "normalLine.frag","normalLine.geom");
	Model planet(exePath.substr(0, exePath.find_last_of('\\')) + "\\model\\planet\\planet.obj");
	Skybox skybox(exePath.substr(0, exePath.find_last_of('\\')) + "\\model\\skybox");

	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 projection;
	projection = glm::perspective(glm::radians(45.0f), (float)800 / 600, 0.1f, 100.0f);
	glm::mat4 fprojection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 100.0f);
	
	#pragma region uniform块
	//链接uniform块至0号位
	unsigned int bubbleMatrices = glGetUniformBlockIndex(bubbleShader->ID, "Matrices");
	glUniformBlockBinding(bubbleShader->ID, bubbleMatrices, 0);
	unsigned int normalLineMatrices = glGetUniformBlockIndex(normalLineShader->ID, "Matrices");
	glUniformBlockBinding(normalLineShader->ID, normalLineMatrices, 0);
	//创建uniform缓冲对象并绑定至0
	unsigned int uboMatrices;
	glGenBuffers(1, &uboMatrices);
	glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glBindBufferRange(GL_UNIFORM_BUFFER, 0, uboMatrices, 0, sizeof(glm::mat4));
	//填充数据至uniform缓冲对象
	glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(projection));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
#pragma endregion

	CubeFrameBuffer cubeFrameBuffer = createCubeFrameBuffers(1024);
	
	Camera* fcamera = new Camera(glm::vec3(0.0, 0.0, 0.0));
	
	while (!glfwWindowShouldClose(window)) {
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		processInput(window);

		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		
		for(unsigned int i=0;i<6;i++){

			glBindFramebuffer(GL_FRAMEBUFFER, cubeFrameBuffer.fbos[i]);
			glViewport(0,0,1024, 1024);
			glClearColor(1.0f,1.0f,0.5f,1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			//旋转相机
			float yaw = 0;
			float pitch = 0;
			switch (i) {
				case 0: yaw = 0;   break;   // +X
				case 1: yaw = 180;  break;   // -X
				case 2: pitch = -90; break;  // +Y（向下看）
				case 3: pitch = 90;  break;  // -Y（向上看）
				case 4: yaw = -90;    break;   // +Z
				case 5: yaw = 90;  break;   // -Z
			}
			fcamera->Yaw = yaw;
			fcamera->Pitch = pitch;
			fcamera->processMouseMovement(0,0,false);
			glm::mat4 fView = fcamera->getViewMatrix();
			//画场景
			for (unsigned int i = 1; i < 3; i++)
			{
				glm::mat4 ballModel;
				ballModel = glm::translate(ballModel, glm::vec3(i * 5.0f, 0.0f, 0.0f));
				bubbleShader->use();
				glActiveTexture(GL_TEXTURE4);
				bubbleShader->setUniform1i("skybox", 4);
				glBindTexture(GL_TEXTURE_CUBE_MAP, skybox.textureID);
				bubbleShader->setUniform3f("viewPos", fcamera->Position);
				glUniformMatrix4fv(glGetUniformLocation(bubbleShader->ID, "view"), 1, GL_FALSE, glm::value_ptr(fView));
				glUniformMatrix4fv(glGetUniformLocation(bubbleShader->ID, "model"), 1, GL_FALSE, glm::value_ptr(ballModel));
				glUniformMatrix4fv(glGetUniformLocation(bubbleShader->ID, "pro"), 1, GL_FALSE, glm::value_ptr(fprojection));
				planet.Draw(bubbleShader);
			}

			glDepthFunc(GL_LEQUAL);
			skyboxShader->use();
			glm::mat4 skyboxView = glm::mat4(glm::mat3(fcamera->getViewMatrix()));
			glUniformMatrix4fv(glGetUniformLocation(skyboxShader->ID, "view"), 1, GL_FALSE, glm::value_ptr(skyboxView));
			glUniformMatrix4fv(glGetUniformLocation(skyboxShader->ID, "pro"), 1, GL_FALSE, glm::value_ptr(fprojection));
			skybox.draw(skyboxShader);
			glDepthFunc(GL_LESS);

			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, 800, 600);

		glm::mat4 view = camera->getViewMatrix();
		/*for (unsigned int i = 0; i < 3; i++)
		{
			glm::mat4 ballModel;
			ballModel = glm::translate(ballModel, glm::vec3(i * 5.0f,0.0f, 0.0f));
			bubbleShader->use();
			glActiveTexture(GL_TEXTURE3);
			bubbleShader->setUniform1i("skybox", 3);
			glBindTexture(GL_TEXTURE_CUBE_MAP, cubeFrameBuffer.cubeMap);
			bubbleShader->setUniform3f("viewPos", camera->Position);
			glUniformMatrix4fv(glGetUniformLocation(bubbleShader->ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
			glUniformMatrix4fv(glGetUniformLocation(bubbleShader->ID, "model"), 1, GL_FALSE, glm::value_ptr(ballModel));
			glUniformMatrix4fv(glGetUniformLocation(bubbleShader->ID, "pro"), 1, GL_FALSE, glm::value_ptr(projection));
			planet.Draw(bubbleShader);
		}*/
		
		/*normalLineShader->use();
		glUniformMatrix4fv(glGetUniformLocation(normalLineShader->ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(normalLineShader->ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
		planet.Draw(normalLineShader);*/

		////glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		glDepthFunc(GL_LEQUAL);
		skyboxShader->use();
		glm::mat4 skyboxView = glm::mat4(glm::mat3(camera->getViewMatrix()));
		glUniformMatrix4fv(glGetUniformLocation(skyboxShader->ID, "view"), 1, GL_FALSE, glm::value_ptr(skyboxView));
		glUniformMatrix4fv(glGetUniformLocation(skyboxShader->ID, "pro"), 1, GL_FALSE, glm::value_ptr(projection));
		skybox.draw(skyboxShader,cubeFrameBuffer.cubeMap);
		glDepthFunc(GL_LESS);

		//获取鼠标事件和切换buffer
		glfwPollEvents();
		glfwSwapBuffers(window);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
	}
	deleteCubeFrameBuffer(cubeFrameBuffer);
	glfwTerminate();
	return 0;
}

CubeFrameBuffer createCubeFrameBuffers(int size) {
	CubeFrameBuffer result;

	// 1. 创建立方体贴图
	glGenTextures(1, &result.cubeMap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, result.cubeMap);
	for (int i = 0; i < 6; i++) {
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, size, size, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	// 2. 创建帧缓冲和渲染缓冲
	result.fbos = new unsigned int[6];
	result.rbos = new unsigned int[6];
	glGenFramebuffers(6, result.fbos);
	glGenRenderbuffers(6, result.rbos);

	for (int i = 0; i < 6; i++) {
		glBindFramebuffer(GL_FRAMEBUFFER, result.fbos[i]);

		// 附加立方体贴图面为颜色附件
		glFramebufferTexture2D(
			GL_FRAMEBUFFER,
			GL_COLOR_ATTACHMENT0,
			GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, // 正确附加到对应面
			result.cubeMap,
			0
		);

		// 附加渲染缓冲为深度/模板附件
		glBindRenderbuffer(GL_RENDERBUFFER, result.rbos[i]);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, size, size);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, result.rbos[i]);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
			std::cerr << "Framebuffer incomplete!" << std::endl;
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	return result;
}

// 使用后需手动释放资源
void deleteCubeFrameBuffer(CubeFrameBuffer& fb) {
	glDeleteTextures(1, &fb.cubeMap);
	glDeleteFramebuffers(6, fb.fbos);
	glDeleteRenderbuffers(6, fb.rbos);
	delete[] fb.fbos;
	delete[] fb.rbos;
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
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {
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
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	float yoffsetf = static_cast<float>(yoffset);
}

#pragma endregion
