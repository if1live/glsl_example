// Ŭnicode please 
#include "toon_shader.h"

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <GL/glew.h>
#include <GL/glfw.h>

#include <vector>

#include "sora/assert_inc.h"
#include "sora/filesystem.h"
#include "sora/low_level_c_file.h"
#include "haruna/gl/shader.h"
#include "haruna/gl/texture.h"
#include "haruna/gl/gl_env.h"
#include "haruna/primitive_mesh.h"

ToonShader::ToonShader(float width, float height)
	: AbstractLogic(width, height), y_rot_(0)
{
}

ToonShader::~ToonShader()
{
	prog_->Deinit();
}

bool ToonShader::Init()
{
	//쉐이더 
	std::string fs_path = sora::Filesystem::GetAppPath("shader/toon.fs");
	std::string vs_path = sora::Filesystem::GetAppPath("shader/toon.vs");
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

	prog_.reset(new haruna::gl::ShaderProgram(vs, fs));
	bool prog_result = prog_->Init();
	if(!prog_result) {
		return false;
	}
	return true;
}
bool ToonShader::Update(float dt)
{
	y_rot_ += 2.0f * dt;
	bool running = !glfwGetKey(GLFW_KEY_ESC) && glfwGetWindowParam(GLFW_OPENED);
	return running;
}
void ToonShader::Draw()
{
	//sample mesh
	//haruna::SolidCubeFactory cube_factory(1, 1, 1);
	//auto data = cube_factory.CreateNormalMesh();
	haruna::SolidSphereFactory sphere_factory(1, 16, 16);
	auto data = sphere_factory.CreateNormalMesh();


	prog_->Use();
	haruna::gl::ShaderLocation pos_loc = prog_->GetAttribLocation("a_position");
	haruna::gl::ShaderLocation normal_loc = prog_->GetAttribLocation("a_normal");
	
	haruna::gl::ShaderLocation light_pos_loc = prog_->GetUniformLocation("u_modelLightPos");
	haruna::gl::ShaderLocation mvp_loc = prog_->GetUniformLocation("u_mvp");
	haruna::gl::ShaderLocation inv_model_loc = prog_->GetUniformLocation("u_invModel");
	
	haruna::gl::ShaderLocation light_color_loc = prog_->GetUniformLocation("u_lightColor");

	//projection
	float aspect = width() / height();
	glm::mat4 proj_mat = glm::perspective(60.0f, aspect, 0.1f, 100.0f);

	float radius = 2;
	glm::vec3 eye(cos(y_rot_) * radius, 0, sin(y_rot_) * radius);
	//glm::vec3 eye(radius, 0, 0);
	glm::vec3 center(0, 0, 0);
	glm::vec3 up(0, 1, 0);
	glm::mat4 view_mat = glm::lookAt(eye, center, up);
		
	//model
	glm::mat4 model_mat = glm::mat4();
	model_mat = glm::rotate(model_mat, y_rot_ * 40, glm::vec3(0, 0, 1));

	glm::mat4 mvp = proj_mat * view_mat * model_mat;

	//draw
	glViewport(0, 0, (int)width(), (int)height());
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_CULL_FACE);

	glUniformMatrix4fv(mvp_loc.handle(), 1, GL_FALSE, glm::value_ptr(mvp));
	glUniformMatrix4fv(inv_model_loc.handle(), 1, GL_FALSE, glm::value_ptr(glm::inverse(model_mat)));

	glEnableVertexAttribArray(pos_loc.handle());
	glEnableVertexAttribArray(normal_loc.handle());

	glm::vec3 light_pos(10, 10, 10);
	glUniform3fv(light_pos_loc.handle(), 1, glm::value_ptr(light_pos));

	glm::vec3 light_color(0, 1, 0);
	glUniform3fv(light_color_loc.handle(), 1, glm::value_ptr(light_color));


	for(auto cmd : data) {
		std::vector<haruna::Vertex_1P1N1UV> &vert_list = cmd.vertex_list;
		std::vector<unsigned short> &index_list = cmd.index_list;

		int stride = sizeof(haruna::Vertex_1P1N1UV);
		glVertexAttribPointer(pos_loc.handle(), 3, GL_FLOAT, GL_FALSE, stride, &vert_list[0].p);
		glVertexAttribPointer(normal_loc.handle(), 3, GL_FLOAT, GL_TRUE, stride, &vert_list[0].n);

		GLenum draw_mode = ToDrawMode(cmd.draw_mode);
		glDrawElements(draw_mode, index_list.size(), GL_UNSIGNED_SHORT, &index_list[0]);
	}

	haruna::gl::GLEnv::CheckError("End Frame");
}