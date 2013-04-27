// Ŭnicode please
#include "stdafx.h"
#include "gl_env.h"

#include "sora/logger.h"
#include "sora/common_string.h"

namespace haruna {;
namespace gl {
	bool GLEnv::CheckFrameBufferStatus(const char *name) {
		GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if(status == GL_FRAMEBUFFER_COMPLETE) {
			return true;
		}
		//else,,
		const char *error_msg = NULL;
		switch(status) {
		case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
			error_msg = "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT";
			break;
#if SR_WIN && (SR_GLES == 0)
		case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
#else
		case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS:
#endif
			error_msg = "GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS";
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
			error_msg = "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT";
			break;
		case GL_FRAMEBUFFER_UNSUPPORTED:
			error_msg = "GL_FRAMEBUFFER_UNSUPPORTED";
			break;
#if SR_WIN && (SR_GLES == 0)
		case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER :
			error_msg = "GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER";
			break;
#endif
		default:
			CheckError(name);
			error_msg = "Unknown Error";
			break;  
		}
		LOGE("FrameBuffer Error [%s] : %s", name, error_msg);
		//SR_ASSERT(false);

		return false;
	}

	bool GLEnv::CheckError(const char *name) {
		int error = glGetError();
		if (error != GL_NO_ERROR) {
			const char *error_msg;
			switch (error) {
			case GL_INVALID_ENUM:
				error_msg = "GL_INVALID_ENUM";
				break;
			case GL_INVALID_VALUE:
				error_msg = "GL_INVALID_VALUE";
				break;
			case GL_INVALID_OPERATION:
				error_msg = "GL_INVALID_OPERATION";
				break;
			case GL_INVALID_FRAMEBUFFER_OPERATION:
				error_msg = "GL_INVALID_FRAMEBUFFER_OPERATION";
				break;
			case GL_OUT_OF_MEMORY:
				error_msg = "GL_OUT_OF_MEMORY";
				break;
			default:
				error_msg = "UNKNOWN";
				break;
			}

			LOGE("OpenGL Error [%s] : %s", name, error_msg);
#if SR_ANDROID
			exit(-1);
#endif
			return false;
		}
		return true;
	}
	const std::vector<std::string> &GLEnv::GetExtensionList() const {
		using std::vector;
		using std::string;

		static vector<string> ext_list;
		static bool run = false;
		if (run == false) {
			run = true;
			const char *str = (const char*)glGetString(GL_EXTENSIONS);
			string ext_str = str;

			sora::Split(ext_str, ' ', &ext_list);
		}
		return ext_list;
	}

	const std::string &GLEnv::GetVersion() const {
		static bool run = false;
		static std::string version;
		if (run == false) {
			run = true;
			const char *str = (const char *)glGetString(GL_VERSION);
			version = str;
		}
		return version;
	}
	const std::string &GLEnv::GetVender() const {
		static bool run = false;
		static std::string version;
		if (run == false) {
			run = true;
			const char *str = (const char *)glGetString(GL_VENDOR);
			version = str;
		}
		return version;
	}
	const std::string &GLEnv::GetRenderer() const {
		static bool run = false;
		static std::string version;
		if (run == false) {
			run = true;
			const char *str = (const char *)glGetString(GL_RENDERER);
			version = str;
		}
		return version;
	}
} //namespace gl
} //namespace haruna
