// Ŭnicode please 
#pragma once

#include "renderer.h"
#include "haruna/vertex.h"
#include "haruna/gl/buffer_object.h"

//reference
//http://fabiensanglard.net/shadowmapping/index.php

namespace haruna {;
class ParametricSurface;
namespace gl {
	class FrameBuffer;
}	// namespace gl
}	// namespace haruna

class ShadowMapping : public AbstractLogic {
public:
	ShadowMapping(float width, float height);
	virtual ~ShadowMapping();

	virtual bool Init();
	virtual bool Update(float dt);
	virtual void Draw();

	void DrawObjects(haruna::gl::ShaderProgram &prog);
	void DrawGround(haruna::gl::ShaderProgram &prog);
	void DrawCube(haruna::gl::ShaderProgram &prog);

private:
	bool InitShader(const std::string &vs_filename, const std::string &fs_filename, std::unique_ptr<haruna::gl::ShaderProgram> &prog);
	bool InitSimpleShader();
	bool InitShaderMapShader();
	bool InitShadowFBO();

private:
	std::unique_ptr<haruna::gl::ShaderProgram> simple_prog_;
	std::unique_ptr<haruna::gl::ShaderProgram> shadow_map_prog_;

	GLuint fbo_;
	GLuint fbo_depth_tex_;
	int shadow_map_width_;
	int shadow_map_height_;

	//camera / light pos info
	glm::vec3 cam_pos_;
	glm::vec3 cam_lookat_;
	glm::vec3 light_pos_;
	glm::vec3 light_lookat_;
};
