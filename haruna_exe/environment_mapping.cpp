// Ŭnicode please 
#include "environment_mapping.h"

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
#include "haruna/parametric_equations.h"
#include "haruna/primitive_mesh.h"

EnvironmentMapping::EnvironmentMapping(float width, float height)
	: AbstractLogic(width, height), y_rot_(0)
{
}

EnvironmentMapping::~EnvironmentMapping()
{
	if(diffuse_map_.get() != nullptr) {
		diffuse_map_->Deinit();
	}
	if(specular_map_.get() != nullptr) {
		specular_map_->Deinit();
	}
	if(normal_map_.get() != nullptr) {
		normal_map_->Deinit();
	}
	if(environment_map_.get() != nullptr) {
		environment_map_->Deinit();
	}
	if(prog_.get() != nullptr) {
		prog_->Deinit();
	}
}

bool EnvironmentMapping::Init()
{
	//쉐이더 
	std::string fs_path = sora::Filesystem::GetAppPath("shader/environment_mapping.fs");
	std::string vs_path = sora::Filesystem::GetAppPath("shader/environment_mapping.vs");
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
	std::string diffuse_map_path = sora::Filesystem::GetAppPath("texture/fieldstone_DM.png");
	diffuse_map_.reset(new haruna::gl::Texture2D(diffuse_map_path));
	bool diffuse_map_init_result = diffuse_map_->Init();
	if(!diffuse_map_init_result) {
		return false;
	}

	//create texture
	std::string specular_map_path = sora::Filesystem::GetAppPath("texture/fieldstone_SM.png");
	specular_map_.reset(new haruna::gl::Texture2D(specular_map_path));
	bool specular_map_init_result = specular_map_->Init();
	if(!specular_map_init_result) {
		return false;
	}

	std::string normal_map_path = sora::Filesystem::GetAppPath("texture/fieldstone_NM.png");
	normal_map_.reset(new haruna::gl::Texture2D(normal_map_path));
	bool normal_map_init_result = normal_map_->Init();
	if(!normal_map_init_result) {
		return false;
	}

	//cube map
	environment_map_.reset(new haruna::gl::TextureCube(
		sora::Filesystem::GetAppPath("texture/cubemap_left.png"),
		sora::Filesystem::GetAppPath("texture/cubemap_right.png"),
		sora::Filesystem::GetAppPath("texture/cubemap_bottom.png"),
		sora::Filesystem::GetAppPath("texture/cubemap_top.png"),
		sora::Filesystem::GetAppPath("texture/cubemap_back.png"),
		sora::Filesystem::GetAppPath("texture/cubemap_front.png")
	));
	bool env_map_init_result = environment_map_->Init();
	if(!env_map_init_result) {
		return false;
	}

	// create mesh
	//mesh_.reset(new haruna::Torus(1, 0.6));
	//mesh_.reset(new haruna::TrefoilKnot(2));
	mesh_.reset(new haruna::Sphere(1.8));

	return true;
}
bool EnvironmentMapping::Update(float dt)
{
	y_rot_ += 0.5f * dt;
	bool running = !glfwGetKey(GLFW_KEY_ESC) && glfwGetWindowParam(GLFW_OPENED);
	return running;
}
void EnvironmentMapping::Draw()
{
		//sample mesh
	//haruna::SolidCubeFactory cube_factory(1, 1, 1);
	//auto data = cube_factory.CreateNormalMesh();
	//haruna::SolidSphereFactory sphere_factory(1, 16, 16);
	//auto data = sphere_factory.CreateNormalMesh();

	prog_->Use();
	haruna::gl::ShaderLocation pos_loc = prog_->GetAttribLocation("a_position");
	haruna::gl::ShaderLocation normal_loc = prog_->GetAttribLocation("a_normal");
	haruna::gl::ShaderLocation texcoord_loc = prog_->GetAttribLocation("a_texcoord");
	haruna::gl::ShaderLocation tangent_loc = prog_->GetAttribLocation("a_tangent");
	
	haruna::gl::ShaderLocation light_pos_loc = prog_->GetUniformLocation("u_modelLightPos");
	haruna::gl::ShaderLocation cam_pos_loc = prog_->GetUniformLocation("u_modelCameraPos");

	haruna::gl::ShaderLocation mvp_loc = prog_->GetUniformLocation("u_mvp");
	haruna::gl::ShaderLocation model_loc = prog_->GetUniformLocation("u_model");

	haruna::gl::ShaderLocation diffuse_map_loc = prog_->GetUniformLocation("s_diffuse");
	haruna::gl::ShaderLocation specular_map_loc = prog_->GetUniformLocation("s_specular");
	haruna::gl::ShaderLocation normal_map_loc = prog_->GetUniformLocation("s_normal");
	haruna::gl::ShaderLocation environment_map_loc = prog_->GetUniformLocation("s_environment");


	haruna::gl::ShaderLocation light_color_loc = prog_->GetUniformLocation("u_lightColor");

	//projection
	float aspect = width() / height();
	glm::mat4 proj_mat = glm::perspective(60.0f, aspect, 0.1f, 100.0f);

	float radius = 3;
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
	glEnable(GL_DEPTH_TEST);

	glm::mat4 mvp_mat = proj_mat * view_mat * model_mat;
	glUniformMatrix4fv(mvp_loc, 1, GL_FALSE, glm::value_ptr(mvp_mat));
	glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(model_mat));


	glEnableVertexAttribArray(pos_loc);
	glEnableVertexAttribArray(normal_loc);
	glEnableVertexAttribArray(texcoord_loc);
	glEnableVertexAttribArray(tangent_loc);

	glm::vec3 light_pos(10, 10, 10);
	glUniform3fv(light_pos_loc, 1, glm::value_ptr(light_pos));

	glUniform3fv(cam_pos_loc, 1, glm::value_ptr(eye));

	// bind texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, *diffuse_map_);
	glUniform1i(diffuse_map_loc.handle(), 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, *specular_map_);
	glUniform1i(specular_map_loc, 1);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, *normal_map_);
	glUniform1i(normal_map_loc, 2);

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_CUBE_MAP, *environment_map_);
	glUniform1i(environment_map_loc, 3);

	//color
	glm::vec4 color(1.0f, 1.0f, 1.0f, 1.0f);
	glUniform4fv(light_color_loc, 1, glm::value_ptr(color));


	std::vector<haruna::Vertex_1P1N1UV1T> vert_list;
	std::vector<unsigned short> index_list;
	mesh_->GenerateTriangleIndices(index_list);
	mesh_->GenerateVertices(vert_list);

	int stride = sizeof(haruna::Vertex_1P1N1UV1T);
	glVertexAttribPointer(pos_loc, 3, GL_FLOAT, GL_FALSE, stride, &vert_list[0].p);
	glVertexAttribPointer(texcoord_loc, 2, GL_FLOAT, GL_FALSE, stride, &vert_list[0].uv);
	glVertexAttribPointer(normal_loc, 3, GL_FLOAT, GL_TRUE, stride, &vert_list[0].n);
	glVertexAttribPointer(tangent_loc, 3, GL_FLOAT, GL_TRUE, stride, &vert_list[0].t);

	glDrawElements(GL_TRIANGLES, index_list.size(), GL_UNSIGNED_SHORT, &index_list[0]);

	if(!haruna::gl::GLEnv::CheckError("End Frame")) {
		getchar();
	}
}
