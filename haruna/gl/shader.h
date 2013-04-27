// Ŭnicode please 
#pragma once

namespace haruna {;
namespace gl {

	class ShaderLocation {
	public:
		ShaderLocation(GLint loc) : loc_(loc) {}
		bool IsValid() const { return (loc_ != -1); }

		GLint handle() const { return loc_; }
		operator GLint() const { return loc_; }
	private:
		GLint loc_;
	};

	class Shader {
	public:
		Shader(const std::string &src);
		~Shader();

		bool Init();
		bool Deinit();
		bool IsInit();

		virtual GLenum GetType() = 0;
		GLuint handle() const { return shader_; }
	protected:
		std::string src_;
		GLuint shader_;
	};

	class VertexShader : public Shader {
	public:
		VertexShader(const std::string &src) : Shader(src) {}
		virtual GLenum GetType() { return GL_VERTEX_SHADER; }
	};

	class FragmentShader : public Shader {
	public:
		FragmentShader(const std::string &src) : Shader(src) {}
		virtual GLenum GetType() { return GL_FRAGMENT_SHADER; }
	};

	class ShaderProgram {
	public:
		ShaderProgram(const VertexShader &vert_shader, const FragmentShader &frag_shader);
		~ShaderProgram();

		bool Init();
		bool Deinit();

		GLuint prog() const { return prog_; }
		bool Use();

		ShaderLocation GetAttribLocation(const std::string &name) const;
		ShaderLocation GetUniformLocation(const std::string &name) const;

	private:
		GLuint prog_;

		VertexShader vert_shader_;
		FragmentShader frag_shader_;
	};
}
}
