// Ŭnicode please 
#include "stdafx.h"
#include "shader.h"

namespace haruna {;
namespace gl {
	Shader::Shader(const std::string &src)
		: shader_(0), src_(src)
	{
	}
	Shader::~Shader()
	{
	}

	bool Shader::IsInit()
	{
		return (shader_ != 0);
	}

	bool Shader::Init()
	{
		if(shader_ != 0) {
			return false;
		}
		GLenum shader_type = GetType();
		shader_ = glCreateShader(shader_type);
		if(shader_ == 0) {
			return false;
		}
		const char *source = src_.data();
		glShaderSource(shader_, 1, &source, NULL);
		glCompileShader(shader_);

		//컴파일 상태 확인
		GLint compiled;
		glGetShaderiv(shader_, GL_COMPILE_STATUS, &compiled);
		if(!compiled) {
			GLint info_len = 0;
			glGetShaderiv(shader_, GL_INFO_LOG_LENGTH, &info_len);
			if(info_len > 1) {
				char *info_log = (char*)malloc(sizeof(char) * info_len);
				glGetShaderInfoLog(shader_, info_len, NULL, info_log);
				fprintf(stderr, "Error compiling shader: \n%s\n", info_log);
				free(info_log);
			}
			glDeleteShader(shader_);
			shader_ = 0;
			return false;
		}

		return true;
	}
	bool Shader::Deinit()
	{
		if(shader_ == 0) {
			return false;
		}
		glDeleteShader(shader_);
		shader_ = 0;
		return true;
	}

	ShaderProgram::ShaderProgram(const VertexShader &vert_shader, const FragmentShader &frag_shader)
		: prog_(0),
		vert_shader_(vert_shader),
		frag_shader_(frag_shader)
	{
	}
	ShaderProgram::~ShaderProgram()
	{
	}

	bool ShaderProgram::Deinit()
	{
		if(prog_ == 0) {
			return false;
		}
		glDeleteProgram(prog_);
		prog_ = 0;

		vert_shader_.Deinit();
		frag_shader_.Deinit();

		return true;
	}

	bool ShaderProgram::Init()
	{
		if(prog_ != 0) {
			return false;
		}

		prog_ = glCreateProgram();
		if(prog_ == 0) {
			return false;
		}

		bool vs_result = vert_shader_.Init();
		if(!vs_result) {
			return false;
		}

		bool fs_result = frag_shader_.Init();
		if(!fs_result) {
			return false;
		}
		glAttachShader(prog_, vert_shader_.handle());
		glAttachShader(prog_, frag_shader_.handle());

		glLinkProgram(prog_);

		GLint linked;
		glGetProgramiv(prog_, GL_LINK_STATUS, &linked);
		if(!linked) {
			GLint info_len = 0;
			glGetProgramiv(prog_, GL_INFO_LOG_LENGTH, &info_len);
			if(info_len > 1) {
				char *info_log = (char*)malloc(sizeof(char) * info_len);
				glGetProgramInfoLog(prog_, info_len, NULL, info_log);
				fprintf(stderr, "Error linking program: \n%s\n", info_log);
				free(info_log);
			}

			glDeleteProgram(prog_);
			prog_ = 0;
			return false;
		}

		return true;
	}
	bool ShaderProgram::Use()
	{
		glUseProgram(prog_);
		return true;
	}
	ShaderLocation ShaderProgram::GetAttribLocation(const std::string &name) const
	{
		GLint loc = glGetAttribLocation(prog_, name.data());
		return ShaderLocation(loc);
	}

	ShaderLocation ShaderProgram::GetUniformLocation(const std::string &name) const
	{
		GLint loc = glGetUniformLocation(prog_, name.data());
		return ShaderLocation(loc);
	}

}	//namespace gl
}	//namespace haruna