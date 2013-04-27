// Ŭnicode please
#pragma once


#include <vector>
#include <string>

namespace haruna {;
namespace gl {
	typedef GLint ShaderVariableHandleType;
	typedef GLuint ShaderHandleType;
	typedef GLuint Texture2D2DHandleType;

	class GLEnv {
	public:
		static bool CheckFrameBufferStatus(const char *name);
		static bool CheckFrameBufferStatus(const std::string &name) {
			return CheckFrameBufferStatus(name.c_str());
		}
		static bool CheckError(const char *name);
		static bool CheckError(const std::string &name) {
			return CheckError(name.c_str());
		}

		//템플릿에서 사용되는 인터페이스측
	public:  
		// gl information
		const std::string &GetVersion() const;
		const std::string &GetVender() const;
		const std::string &GetRenderer() const;
		const std::vector<std::string> &GetExtensionList() const;
	};


	template<typename T>
	struct TypeToEnum { };
	template<>  struct TypeToEnum<float> { enum { value = GL_FLOAT }; };
	template<>  struct TypeToEnum<int> { enum { value = GL_INT }; };
	template<>  struct TypeToEnum<unsigned int> { enum { value = GL_UNSIGNED_INT }; };
	template<>  struct TypeToEnum<char> { enum { value = GL_BYTE }; };
	template<>  struct TypeToEnum<unsigned char> { enum { value = GL_UNSIGNED_BYTE }; };
	template<>  struct TypeToEnum<short> { enum { value = GL_SHORT }; };
	template<>  struct TypeToEnum<unsigned short> { enum { value = GL_UNSIGNED_SHORT }; };
} //namespace gl
} //namespace haruna
