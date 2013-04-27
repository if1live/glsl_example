// Ŭnicode please
#include "stdafx.h"
#include "frame_buffer.h"
#include "gl_env.h"
#include "texture.h"

namespace haruna {;
namespace gl {
	void FrameBuffer::CreateDepthTex(int w, int h, Texture2D *depth_tex)
	{
		GLuint tex_id = 0;
		//깊이를 텍스쳐에 연결
		glGenTextures(1, &tex_id);
		glBindTexture(GL_TEXTURE_2D, tex_id);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, w, h, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, NULL);
		GLEnv::CheckError("DepthTexture");

		depth_tex->Init(tex_id, w, h);
	}

	void FrameBuffer::CreateDepthStencilTex(int w, int h, Texture2D *depth_tex) 
	{
		GLuint tex_id = 0;
		//깊이를 텍스쳐에 연결
		glGenTextures(1, &tex_id);
		glBindTexture(GL_TEXTURE_2D, tex_id);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, w, h, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
		GLEnv::CheckError("DepthTexture");

		depth_tex->Init(tex_id, w, h);
	}
	void FrameBuffer::CreateNormalRGBAColorTex(int w, int h, Texture2D *color_tex) 
	{
		//색은 그냥 버퍼로 쓰자
		/*
		glGenRenderbuffers(1, &color_rb);
		glBindRenderbuffer(GL_RENDERBUFFER, color_rb);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA4, w, h);
		SR_CHECK_ERROR("1");
		*/
		GLuint tex_id;
		glGenTextures(1, &tex_id);
		glBindTexture(GL_TEXTURE_2D, tex_id);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		haruna::gl::GLEnv::CheckError("ColorTexture");

		color_tex->Init(tex_id, w, h);
	}

	bool FrameBuffer::Init(int w, int h) 
	{
		if(handle_ != 0) {
			return false;
		}
		//CreateDepthTex(w, h, depth_tex);
		CreateDepthStencilTex(w, h, &depth_tex_);
		CreateNormalRGBAColorTex(w, h, &color_tex_);

		//fbo생성
		GLuint fbo = 0;
		glGenFramebuffers(1, &fbo);
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
		//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depth_tex->handle(), 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, depth_tex_.tex(), 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, color_tex_.tex(), 0);
		//glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, color_rb);
		GLEnv::CheckFrameBufferStatus("fb");
		GLEnv::CheckError("Create FB");
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		handle_ = fbo;
		return true;
	}
	bool FrameBuffer::InitWithoutDepth(int w, int h)
	{
		if(handle_ != 0) {
			return false;
		}
		CreateNormalRGBAColorTex(w, h, &color_tex_);

		GLuint fbo = 0;
		glGenFramebuffers(1, &fbo);
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, color_tex_.tex(), 0);
		GLEnv::CheckFrameBufferStatus("fb");
		GLEnv::CheckError("Create FB");
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		handle_ = fbo;
		return true;
	}
	bool FrameBuffer::Deinit() 
	{
		if(handle_ == 0) {
			return false;
		}

		if(color_tex_.tex() != 0) {
			color_tex_.Deinit();
		}
		if(depth_tex_.tex() != 0) {
			depth_tex_.Deinit();
		}

		glDeleteFramebuffers(1, &handle_);
		handle_ = 0;
		return true;
	}
	bool FrameBuffer::IsInit() const
	{
		if(handle_ == 0) {
			return false;
		} else {
			return true;
		}
	}
	void FrameBuffer::Bind() 
	{
		glBindFramebuffer(GL_FRAMEBUFFER, handle_);
	}
	void FrameBuffer::Unbind() 
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
} //namespace gl
} //namespace haruna
