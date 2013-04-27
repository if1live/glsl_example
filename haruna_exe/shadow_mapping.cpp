// Ŭnicode please 
#include "shadow_mapping.h"

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <GL/glew.h>
#include <GL/glfw.h>

#include "sora/assert_inc.h"
#include "sora/filesystem.h"
#include "sora/low_level_c_file.h"
#include "haruna/parametric_equations.h"
#include "haruna/gl/shader.h"
#include "haruna/gl/frame_buffer.h"
#include "haruna/gl/gl_env.h"
#include "haruna/primitive_mesh.h"

const float kShadowMapRatio = 2.0f;

ShadowMapping::ShadowMapping(float width, float height)
	: AbstractLogic(width, height), fbo_(0), fbo_depth_tex_(0),
	shadow_map_width_(0),
	shadow_map_height_(0)
{
	cam_pos_ = glm::vec3(32, 20, 0);
	cam_lookat_ = glm::vec3(2, 0, -10);
	light_pos_ = glm::vec3(3, 20, 0);
	light_lookat_ = glm::vec3(0, 0, -5);

	//shadow_map_width_ = width * kShadowMapRatio;
	//shadow_map_height_ = height * kShadowMapRatio;
	shadow_map_width_ = 2048;
	shadow_map_height_ = 2048;
}

ShadowMapping::~ShadowMapping()
{
	if(simple_prog_.get() != nullptr) {
		simple_prog_->Deinit();
	}
	if(shadow_map_prog_.get() != nullptr) {
		shadow_map_prog_->Deinit();
	}
	if(fbo_ != 0) {
		glDeleteFramebuffers(1, &fbo_);
		fbo_ = 0;
	}
	if(fbo_depth_tex_ != 0) {
		glDeleteTextures(1, &fbo_depth_tex_);
	}
}

bool ShadowMapping::InitShader(const std::string &vs_filename, const std::string &fs_filename, std::unique_ptr<haruna::gl::ShaderProgram> &prog)
{
	std::string fs_path = sora::Filesystem::GetAppPath(fs_filename);
	std::string vs_path = sora::Filesystem::GetAppPath(vs_filename);
	sora::ReadonlyCFile fs_file = sora::ReadonlyCFile(fs_path);
	sora::ReadonlyCFile vs_file = sora::ReadonlyCFile(vs_path);
	bool fs_open_result = fs_file.Open();
	bool vs_open_result = vs_file.Open();
	if(!fs_open_result) {
		return false;
	}
	if(!vs_open_result) {
		return false;
	}
	
	std::string fs_src(static_cast<const char*>(fs_file.GetBuffer()));
	std::string vs_src(static_cast<const char*>(vs_file.GetBuffer()));
	
	haruna::gl::VertexShader vs(vs_src);
	haruna::gl::FragmentShader fs(fs_src);

	prog.reset(new haruna::gl::ShaderProgram(vs, fs));
	bool prog_result = prog->Init();
	if(!prog_result) {
		return false;
	}

	return true;
}
bool ShadowMapping::InitSimpleShader()
{
	//return InitShader("shader/create_shadow_map.vs", "shader/create_shadow_map.fs", create_shadow_map_prog_);
	return InitShader("shader/simple_red.vs", "shader/simple_red.fs", simple_prog_);
}
bool ShadowMapping::InitShaderMapShader()
{
	return InitShader("shader/shadow_map.vs", "shader/shadow_map.fs", shadow_map_prog_);
}

bool ShadowMapping::Init()
{
	if(!InitShaderMapShader()) {
		return false;
	}
	if(!InitSimpleShader()) {
		return false;
	}
	if(!InitShadowFBO()) {
		return false;
	}
	return true;
}

bool ShadowMapping::InitShadowFBO()
{
	//float shadow_map_w = width() * kShadowMapRatio;
	//float shadow_map_h = height() * kShadowMapRatio;
	float shadow_map_w = shadow_map_width_;
	float shadow_map_h = shadow_map_height_;

	// 0 : depth stencil
	// shadow map 정보 생성에는 깊이만 잇어도 되니까 깊이만 fbo에 추가함
	glGenTextures(1, &fbo_depth_tex_);
	glBindTexture(GL_TEXTURE_2D, fbo_depth_tex_);
	
	// GL_LINEAR does not make sense for depth texture. However, next tutorial shows usage of GL_LINEAR and PCF
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// Remove artifact on the edges of the shadowmap
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	// No need to force GL_DEPTH_COMPONENT24, drivers usually give you the max precision if available
	//glTexImage2D( GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadow_map_w, shadow_map_h, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, shadow_map_w, shadow_map_h, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, shadow_map_w, shadow_map_h, 0, GL_DEPTH_COMPONENT, GL_INT, NULL);	
	haruna::gl::GLEnv::CheckError("DepthTexture");

	glBindTexture(GL_TEXTURE_2D, 0);

	//init fb
	glGenFramebuffers(1, &fbo_);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo_);

	// Instruct openGL that we won't bind a color texture with the currently bound FBO
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, fbo_depth_tex_, 0);
	haruna::gl::GLEnv::CheckFrameBufferStatus("fb");
	haruna::gl::GLEnv::CheckError("Create FB");
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return true;
}

bool ShadowMapping::Update(float dt)
{
	//y_rot_ += 2.0f * dt;
	//y_rot_ = sora::kPi/3.0f;

	//Light mouvement circle radius
	static float elapsed_time = 0.0f;
	
	float light_mvnt = 30.0f;
	light_pos_[0] = light_mvnt * cos(elapsed_time);
	light_pos_[2] = light_mvnt * sin(elapsed_time);
	elapsed_time += dt;

	bool running = !glfwGetKey(GLFW_KEY_ESC) && glfwGetWindowParam(GLFW_OPENED);
	return running;
}
void ShadowMapping::Draw()
{
	glBindFramebuffer(GL_FRAMEBUFFER, fbo_);
	{
		glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

		auto prog = simple_prog_.get();
		prog->Use();

		//glViewport(0, 0, width(), height());
		glViewport(0, 0, shadow_map_width_, shadow_map_height_);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);
		glCullFace(GL_FRONT);

		auto pos_loc = prog->GetAttribLocation("a_position");
		auto mvp_loc = prog->GetUniformLocation("u_mvp");
		glEnableVertexAttribArray(pos_loc);

		float aspect = width() / height();
		//glm::mat4 proj_mat = glm::perspective(60.0f, aspect, 0.1f, 100.0f);
		glm::mat4 proj_mat = glm::perspective(45.0f, width()/height(),10.0f,40000.0f);
		glm::mat4 view_mat = glm::lookAt(light_pos_, light_lookat_, glm::vec3(0,1,0));


		glm::mat4 world_mat;
		glm::mat4 mvp_mat;

		mvp_mat = proj_mat * view_mat * world_mat;
		glUniformMatrix4fv(mvp_loc, 1, GL_FALSE, glm::value_ptr(mvp_mat));
		DrawGround(*prog);

		glm::mat4 cube_1_world_mat = glm::translate(0.0f, 4.0f, -16.0f);
		mvp_mat = proj_mat * view_mat * cube_1_world_mat;
		glUniformMatrix4fv(mvp_loc, 1, GL_FALSE, glm::value_ptr(mvp_mat));
		DrawCube(*prog);

		glm::mat4 cube_2_world_mat = glm::translate(0.0f, 4.0f, -5.0f);
		mvp_mat = proj_mat * view_mat * cube_2_world_mat;
		glUniformMatrix4fv(mvp_loc, 1, GL_FALSE, glm::value_ptr(mvp_mat));
		DrawCube(*prog);
		
		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	if(shadow_map_prog_.get() != nullptr) {
		auto prog = shadow_map_prog_.get();
		prog->Use();

		glViewport(0, 0, width(), height());
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);
		glCullFace(GL_BACK);

		auto pos_loc = prog->GetAttribLocation("a_position");
		auto normal_loc = prog->GetAttribLocation("a_normal");
		auto mvp_loc = prog->GetUniformLocation("u_mvp");
		auto world_loc = prog->GetUniformLocation("u_world");
		auto light_mvp_loc = prog->GetUniformLocation("u_lightMVP");
		auto shadow_loc = prog->GetUniformLocation("s_shadow");

		auto light_color_loc = prog->GetUniformLocation("u_lightColor");
		auto light_pos_loc = prog->GetUniformLocation("u_lightPos");

		glEnableVertexAttribArray(pos_loc);
		glEnableVertexAttribArray(normal_loc);

		float aspect = width() / height();
		//glm::mat4 proj_mat = glm::perspective(60.0f, aspect, 0.1f, 100.0f);
		glm::mat4 proj_mat = glm::perspective(45.0f, width()/height(),10.0f,40000.0f);
		glm::mat4 light_view_mat = glm::lookAt(light_pos_, light_lookat_, glm::vec3(0,1,0));
		glm::mat4 cam_view_mat = glm::lookAt(cam_pos_, cam_lookat_, glm::vec3(0,1,0));

		glm::mat4 world_mat;
		glm::mat4 light_mvp_mat;

		glm::mat4 light_view_proj_mat = proj_mat * light_view_mat;

		glm::mat4 cam_mvp_mat;

		glBindTexture(GL_TEXTURE_2D, fbo_depth_tex_);
		glActiveTexture(GL_TEXTURE0);
		glUniform1i(shadow_loc, 0);

		glUniform3fv(light_pos_loc, 1, glm::value_ptr(light_pos_));

		//glm::vec4 ground_color(0.3f,0.3f,0.3f,1);
		glm::vec4 ground_color(0.6f,0.6f,0.6f,1);
		glUniform4fv(light_color_loc, 1, glm::value_ptr(ground_color));

		light_mvp_mat = light_view_proj_mat * world_mat;
		cam_mvp_mat = proj_mat * cam_view_mat * world_mat;
		glUniformMatrix4fv(mvp_loc, 1, GL_FALSE, glm::value_ptr(cam_mvp_mat));
		glUniformMatrix4fv(light_mvp_loc, 1, GL_FALSE, glm::value_ptr(light_mvp_mat));
		glUniformMatrix4fv(world_loc, 1, GL_FALSE, glm::value_ptr(world_mat));
		DrawGround(*prog);

		glm::vec4 cube_color(1.0f, 1.0f, 1.0f,1);
		glUniform4fv(light_color_loc, 1, glm::value_ptr(cube_color));

		glm::mat4 cube_1_world_mat = glm::translate(0.0f, 4.0f, -16.0f);
		light_mvp_mat = light_view_proj_mat * cube_1_world_mat;
		cam_mvp_mat = proj_mat * cam_view_mat * cube_1_world_mat;
		glUniformMatrix4fv(mvp_loc, 1, GL_FALSE, glm::value_ptr(cam_mvp_mat));
		glUniformMatrix4fv(light_mvp_loc, 1, GL_FALSE, glm::value_ptr(light_mvp_mat));
		glUniformMatrix4fv(world_loc, 1, GL_FALSE, glm::value_ptr(cube_1_world_mat));
		DrawCube(*prog);

		glm::mat4 cube_2_world_mat = glm::translate(0.0f, 4.0f, -5.0f);
		light_mvp_mat = light_view_proj_mat * cube_2_world_mat;
		cam_mvp_mat = proj_mat * cam_view_mat * cube_2_world_mat;
		glUniformMatrix4fv(mvp_loc, 1, GL_FALSE, glm::value_ptr(cam_mvp_mat));
		glUniformMatrix4fv(light_mvp_loc, 1, GL_FALSE, glm::value_ptr(light_mvp_mat));
		glUniformMatrix4fv(world_loc, 1, GL_FALSE, glm::value_ptr(cube_2_world_mat));
		DrawCube(*prog);	
	}
}

void ShadowMapping::DrawGround(haruna::gl::ShaderProgram &prog)
{
	auto pos_loc = prog.GetAttribLocation("a_position");
	auto normal_loc = prog.GetAttribLocation("a_normal");

	// Ground
	
	float vertex_data[] = {
		-35, 2, -35,
		-35, 2, 15,
		15, 2, 15,
		15, 2, -35
	};
	float normal_data[] = {
		0, 1, 0,
		0, 1, 0,
		0, 1, 0,
		0, 1, 0
	};

	glVertexAttribPointer(pos_loc, 3, GL_FLOAT, GL_FALSE, 0, vertex_data);
	glVertexAttribPointer(normal_loc, 3, GL_FLOAT, GL_FALSE, 0, normal_data);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}
void ShadowMapping::DrawCube(haruna::gl::ShaderProgram &prog)
{
	auto pos_loc = prog.GetAttribLocation("a_position");
	auto normal_loc = prog.GetAttribLocation("a_normal");

	haruna::SolidCubeFactory factory(4, 4, 4);
	auto cube_data = factory.CreateNormalMesh();
	//haruna::SolidSphereFactory factory(2, 16, 16);
	//auto cube_data = factory.CreateNormalMesh();
	for(auto cmd : cube_data) {
		GLenum draw_mode = ToDrawMode(cmd.draw_mode);
		int stride = sizeof(cmd.vertex_list[0]);
		glVertexAttribPointer(pos_loc, 3, GL_FLOAT, GL_FALSE, stride, &cmd.vertex_list[0].p);
		glVertexAttribPointer(normal_loc, 3, GL_FLOAT, GL_FALSE, stride, &cmd.vertex_list[0].n);
		glDrawElements(draw_mode, cmd.index_list.size(), GL_UNSIGNED_SHORT, cmd.index_list.data());
	}
}
