// Ŭnicode please 
#include "diffuse_specular_mapping.h"

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

DiffuseSpecularMapping::DiffuseSpecularMapping(float width, float height)
	: AbstractLogic(width, height), y_rot_(0)
{
}

DiffuseSpecularMapping::~DiffuseSpecularMapping()
{
	diffuse_map_->Deinit();
	specular_map_->Deinit();
	prog_->Deinit();
}

bool DiffuseSpecularMapping::Init()
{
	//쉐이더 
	std::string fs_path = sora::Filesystem::GetAppPath("shader/diffuse_specular_map.fs");
	std::string vs_path = sora::Filesystem::GetAppPath("shader/diffuse_specular_map.vs");
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

	//create texture
	std::string diffuse_map_path = sora::Filesystem::GetAppPath("texture/glazed_brick_D.png");
	diffuse_map_.reset(new haruna::gl::Texture2D(diffuse_map_path));
	bool diffuse_map_init_result = diffuse_map_->Init();
	if(!diffuse_map_init_result) {
		return false;
	}

	//create texture
	std::string specular_map_path = sora::Filesystem::GetAppPath("texture/glazed_brick_S.png");
	specular_map_.reset(new haruna::gl::Texture2D(specular_map_path));
	bool specular_map_init_result = specular_map_->Init();
	if(!specular_map_init_result) {
		return false;
	}

	return true;
}

bool DiffuseSpecularMapping::Update(float dt)
{
	y_rot_ += 2.0f * dt;
	bool running = !glfwGetKey(GLFW_KEY_ESC) && glfwGetWindowParam(GLFW_OPENED);
	return running;
}

void DiffuseSpecularMapping::Draw()
{
	//sample mesh
	//haruna::SolidCubeFactory cube_factory(1, 1, 1);
	//auto data = cube_factory.CreateNormalMesh();
	haruna::SolidSphereFactory sphere_factory(1, 16, 16);
	auto data = sphere_factory.CreateNormalMesh();


	prog_->Use();
	haruna::gl::ShaderLocation pos_loc = prog_->GetAttribLocation("a_position");
	haruna::gl::ShaderLocation normal_loc = prog_->GetAttribLocation("a_normal");
	haruna::gl::ShaderLocation texcoord_loc = prog_->GetAttribLocation("a_texcoord");
	
	haruna::gl::ShaderLocation light_pos_loc = prog_->GetUniformLocation("u_modelLightPos");
	haruna::gl::ShaderLocation cam_pos_loc = prog_->GetUniformLocation("u_modelCameraPos");
	haruna::gl::ShaderLocation model_loc = prog_->GetUniformLocation("u_model");
	haruna::gl::ShaderLocation view_loc = prog_->GetUniformLocation("u_view");
	haruna::gl::ShaderLocation proj_loc = prog_->GetUniformLocation("u_proj");

	haruna::gl::ShaderLocation diffuse_map_loc = prog_->GetUniformLocation("s_diffuse");
	haruna::gl::ShaderLocation specular_map_loc = prog_->GetUniformLocation("s_specular");

	haruna::gl::ShaderLocation light_color_loc = prog_->GetUniformLocation("u_lightColor");

	//projection
	float aspect = width() / height();
	glm::mat4 proj_mat = glm::perspective(60.0f, aspect, 0.1f, 100.0f);

	float radius = 2;
	glm::vec3 eye(cos(y_rot_) * radius, 0, sin(y_rot_) * radius);
	glm::vec3 center(0, 0, 0);
	glm::vec3 up(0, 1, 0);
	glm::mat4 view_mat = glm::lookAt(eye, center, up);
		
	//model
	glm::mat4 model_mat = glm::mat4();

	//draw
	glViewport(0, 0, (int)width(), (int)height());
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_CULL_FACE);

	//glUniformMatrix4fv(mvp_loc.handle(), 1, GL_FALSE, glm::value_ptr(mvp));
	glUniformMatrix4fv(model_loc.handle(), 1, GL_FALSE, glm::value_ptr(model_mat));
	glUniformMatrix4fv(view_loc.handle(), 1, GL_FALSE, glm::value_ptr(view_mat));
	glUniformMatrix4fv(proj_loc.handle(), 1, GL_FALSE, glm::value_ptr(proj_mat));


	glEnableVertexAttribArray(pos_loc.handle());
	glEnableVertexAttribArray(normal_loc.handle());
	glEnableVertexAttribArray(texcoord_loc.handle());

	glm::vec3 light_pos(10, 10, 10);
	glUniform3fv(light_pos_loc.handle(), 1, glm::value_ptr(light_pos));

	glUniform3fv(cam_pos_loc.handle(), 1, glm::value_ptr(eye));

	// bind texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, diffuse_map_->tex());
	glUniform1i(diffuse_map_loc.handle(), 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, specular_map_->tex());
	glUniform1i(specular_map_loc.handle(), 1);

	//color
	glm::vec4 color(0.7f, 0.5f, 1.0f, 1.0f);
	glUniform4fv(light_color_loc.handle(), 1, glm::value_ptr(color));

	for(auto cmd : data) {
		std::vector<haruna::Vertex_1P1N1UV> &vert_list = cmd.vertex_list;
		std::vector<unsigned short> &index_list = cmd.index_list;

		int stride = sizeof(haruna::Vertex_1P1N1UV);
		glVertexAttribPointer(pos_loc.handle(), 3, GL_FLOAT, GL_FALSE, stride, &vert_list[0].p);
		glVertexAttribPointer(normal_loc.handle(), 3, GL_FLOAT, GL_FALSE, stride, &vert_list[0].n);
		glVertexAttribPointer(texcoord_loc.handle(), 2, GL_FLOAT, GL_FALSE, stride, &vert_list[0].uv);

		GLenum draw_mode = ToDrawMode(cmd.draw_mode);
		glDrawElements(draw_mode, index_list.size(), GL_UNSIGNED_SHORT, &index_list[0]);
	}

	haruna::gl::GLEnv::CheckError("End Frame");
}
