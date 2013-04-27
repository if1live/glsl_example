// Ŭnicode please 
#include <GL/glew.h>
#include <GL/glfw.h>

#include <iostream>
#include <memory>
#include "renderer.h"

#include "color_shader.h"
#include "texture_mapping.h"
#include "lighting.h"
#include "diffuse_specular_mapping.h"
#include "toon_shader.h"
#include "normal_mapping.h"
#include "environment_mapping.h"
#include "uv_animation.h"
#include "color_conversion.h"
#include "edge_detection.h"
#include "shadow_mapping.h"

const float kWidth = 640;
const float kHeight = 480;

bool InitWindow(int width, int height) 
{
	if(!glfwInit()) {
		exit(EXIT_FAILURE);
	}

	if(!glfwOpenWindow(width, height, 0, 0, 0, 0, 0, 0, GLFW_WINDOW)) {
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	GLenum err = glewInit();
	if(GLEW_OK != err) {
		/* Problem: glewInit failed, something is seriously wrong. */
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));

		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
	return true;
}

std::unique_ptr<AbstractLogic> CreateLogicByMenu()
{
	printf("Select Example\n");
	printf("1. Color Shader\n");
	printf("2. Texture2D Mapping\n");
	printf("3. Lighting\n");
	printf("4. Diffuse Specular Mapping\n");
	printf("5. ToonShader\n");
	printf("6. Normal Mapping\n");
	printf("7. Environment Mapping\n");
	printf("8. UV Animation\n");
	printf("9. Shadow Mapping\n");
	printf("10. Color Conversion(Press 1/2/3 after running)\n");
	printf("11. Edge Detection(Press 1/2/3 after running)\n");
	int select = 0;
	scanf("%d", &select);

	std::unique_ptr<AbstractLogic> logic;
	switch(select) {
	case 1:
		logic.reset(new ColorShader(kWidth, kHeight));
		break;
	case 2:
		logic.reset(new Texture2DMapping(kWidth, kHeight));
		break;
	case 3:
		logic.reset(new Lighting(kWidth, kHeight));
		break;
	case 4:
		logic.reset(new DiffuseSpecularMapping(kWidth, kHeight));
		break;
	case 5:
		logic.reset(new ToonShader(kWidth, kHeight));
		break;
	case 6:
		logic.reset(new NormalMapping(kWidth, kHeight));
		break;
	case 7:
		logic.reset(new EnvironmentMapping(kWidth, kHeight));
		break;
	case 8:
		logic.reset(new UVAnimation(kWidth, kHeight));
		break;
	case 9:
		logic.reset(new ShadowMapping(kWidth, kHeight));
		break;
	case 10:
		logic.reset(new ColorConversion(kWidth, kHeight));
		break;
	case 11:
		logic.reset(new EdgeDetection(kWidth, kHeight));
		break;

	default:
		printf("Not support : %d\n", select);
		getchar();
		exit(EXIT_FAILURE);
	}
	return logic;
}

int main()
{
	std::unique_ptr<AbstractLogic> logic = CreateLogicByMenu();
	//std::unique_ptr<AbstractLogic> logic(new NormalMapping(kWidth, kHeight));
	//std::unique_ptr<AbstractLogic> logic(new DiffuseSpecularMapping(kWidth, kHeight));
	//std::unique_ptr<AbstractLogic> logic(new EnvironmentMapping(kWidth, kHeight));
	//std::unique_ptr<AbstractLogic> logic(new UVAnimation(kWidth, kHeight));
	//std::unique_ptr<AbstractLogic> logic(new ColorConversion(kWidth, kHeight));
	//std::unique_ptr<AbstractLogic> logic(new EdgeDetection(kWidth, kHeight));
	//std::unique_ptr<AbstractLogic> logic(new ShadowMapping(kWidth, kHeight));

	InitWindow(static_cast<int>(kWidth), static_cast<int>(kHeight));

	bool running = true;
	bool init_result = logic->Init();
	if(init_result == false) {
		getchar();
		running = false;
	}

	double old_time = glfwGetTime();
	while(running) {
		double current_time = glfwGetTime();

		float dt = static_cast<float>(current_time - old_time);
		running = logic->Update(dt);
		logic->Draw();

		glfwSwapBuffers();
		old_time = current_time;
	}

	logic.reset(NULL);
	glfwTerminate();
	return 0;	
}	
