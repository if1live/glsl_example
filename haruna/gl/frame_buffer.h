// Ŭnicode please
#pragma once

#include "texture.h"

namespace haruna {;
namespace gl {

	class Texture2D;

	class FrameBuffer {
	public:
		FrameBuffer() : handle_(0), width_(0), height_(0) {}
		~FrameBuffer() {}

		Texture2D &color_tex() { return color_tex_; }
		Texture2D &depth_tex() { return depth_tex_; }
		int width() const { return width_; }
		int height() const { return height_; }
		GLuint handle() const { return handle_; }

		void Bind();
		void Unbind();
		
		bool IsInit() const;
		bool Deinit();

		bool Init(int w, int h);
		bool InitWithoutDepth(int w, int h);	

	public:
		static void CreateDepthTex(int w, int h, Texture2D *depth_tex);
		static void CreateDepthStencilTex(int w, int h, Texture2D *depth_tex);
		//unsigned byte + rgba texture
		static void CreateNormalRGBAColorTex(int w, int h, Texture2D *color_tex);

	private:
		GLuint handle_;
		Texture2D color_tex_;
		Texture2D depth_tex_;
		int width_;
		int height_;
	};
} //namespace gl
} //namespace haruna