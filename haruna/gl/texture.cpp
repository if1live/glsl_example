// Ŭnicode please 
#include "stdafx.h"
#include "texture.h"
#include "haruna/lodepng.h"

#include "sora/filesystem.h"

namespace haruna {;
namespace gl {
	bool ImageLoader::Load(std::vector<unsigned char> *data, int *w, int *h)
	{
		*w = 0;
		*h = 0;

		std::vector<unsigned char> image;
		unsigned width, height;
		unsigned error = lodepng::decode(image, width, height, filename_);
		
		if(error != 0) {
			std::cout << "error " << error << ": " << lodepng_error_text(error) << std::endl;
			return false;
		}

		// Texture2D size must be power of two for the primitive OpenGL version this is written for. Find next power of two.
		size_t u2 = 1; while(u2 < width) u2 *= 2;
		size_t v2 = 1; while(v2 < height) v2 *= 2;
		
		// Make power of two version of the image.
		std::vector<unsigned char> image2(u2 * v2 * 4);
		for(size_t y = 0; y < height; y++) {
			for(size_t x = 0; x < width; x++) {
				for(size_t c = 0; c < 4; c++) {
					image2[4 * u2 * y + 4 * x + c] = image[4 * width * y + 4 * x + c];
				}
			}
		}

		*w = u2;
		*h = v2;
		*data = image2;

		return true;
	}
	bool ImageLoader::LoadWithoutAlpha(std::vector<unsigned char> *data, int *w, int *h)
	{
		std::vector<unsigned char> tmp;
		bool retval = Load(&tmp, w, h);
		if(retval == false) {
			return false;
		}

		size_t width = *w;
		size_t height = *h;

		std::vector<unsigned char> &img = *data;
		img.resize(width * height * 3);
		for(size_t y = 0; y < height ; y++) {
			for(size_t x = 0; x < width ; x++) {
				for(size_t c = 0; c < 3; c++) {
					img[3 * width * y + 3 * x + c] = tmp[4 * width * y + 4 * x + c];
				}
			}
		}

		return true;
	}


	Texture2D::Texture2D(const std::string &filename)
		: filename_(filename), tex_(0), width_(0), height_(0)
	{
	}
	Texture2D::Texture2D()
		: tex_(0), width_(0), height_(0)
	{
	}
	Texture2D::~Texture2D()
	{
	}

	bool Texture2D::Init(GLuint tex_id, int w, int h)
	{
		if(tex_ != 0) {
			return false;
		}
		tex_ = tex_id;
		width_ = w;
		height_ = h;
		return true;
	}
	bool Texture2D::Init()
	{
		/* reference
		http://lodev.org/lodepng/example_opengl.cpp
		*/
		if(tex_ != 0) {
			return false;
		}

		std::vector<unsigned char> image;
		int w, h;
		ImageLoader loader(filename_);
		bool load_result = loader.Load(&image, &w, &h);
		if(!load_result) {
			return false;
		}
		/*
		GLubyte pixels[] = {
			100, 0, 0,
			0, 100, 0,
			0, 0, 100,
			0, 0, 100,
		};
		*/
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		
		glGenTextures(1, &tex_);
		glBindTexture(GL_TEXTURE_2D, tex_);
		//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 2, 2, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, &image[0]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		
		width_ = w;
		height_ = h;

		return true;
	}
	bool Texture2D::Deinit()
	{
		if(tex_ == 0) {
			return false;
		}

		glDeleteTextures(1, &tex_);
		tex_ = 0;
		width_ = 0;
		height_ = 0;
		return true;
	}

	TextureCube::TextureCube(
			const std::string &file_left, 
			const std::string &file_right,
			const std::string &file_bottom,
			const std::string &file_top,
			const std::string &file_back,
			const std::string &file_front
			)
			: tex_(0),
			file_left_(file_left),
			file_right_(file_right),
			file_bottom_(file_bottom),
			file_top_(file_top),
			file_back_(file_back),
			file_front_(file_front)
	{
	}
	TextureCube::~TextureCube()
	{
	}

	bool TextureCube::Init()
	{
		if(tex_ != 0) {
			return false;
		}


		glGenTextures(1, &tex_);
		glBindTexture(GL_TEXTURE_CUBE_MAP, tex_);
		/*
		GLubyte cubePixels[6][3] = {
			255, 0, 0,
			0, 255, 0,
			0, 0, 255,
			255, 255, 0,
			255, 0, 255,
			255, 255, 255
		};
		int w = 4;
		int h = 4;
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, &cubePixels[0]);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, &cubePixels[1]);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, &cubePixels[0]);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, &cubePixels[1]);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, &cubePixels[0]);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, &cubePixels[1]);
		*/

		{
			ImageLoader loader(file_left_);
			int w, h;
			std::vector<unsigned char> image;
			bool load_result = loader.LoadWithoutAlpha(&image, &w, &h);
			if(!load_result) {
				return false;
			}

			//glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGB, 1, 1, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, image.data());
		}
		{
			ImageLoader loader(file_right_);
			int w, h;
			std::vector<unsigned char> image;
			bool load_result = loader.LoadWithoutAlpha(&image, &w, &h);
			if(!load_result) {
				return false;
			}
			//glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.data());
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, image.data());
		}
		{
			ImageLoader loader(file_bottom_);
			int w, h;
			std::vector<unsigned char> image;
			bool load_result = loader.LoadWithoutAlpha(&image, &w, &h);
			if(!load_result) {
				return false;
			}
			//glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.data());
			glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, image.data());
		}
		{
			ImageLoader loader(file_top_);
			int w, h;
			std::vector<unsigned char> image;
			bool load_result = loader.LoadWithoutAlpha(&image, &w, &h);
			if(!load_result) {
				return false;
			}
			//glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.data());
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, image.data());
		}
		{
			ImageLoader loader(file_back_);
			int w, h;
			std::vector<unsigned char> image;
			bool load_result = loader.LoadWithoutAlpha(&image, &w, &h);
			if(!load_result) {
				return false;
			}
			//glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.data());
			glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, image.data());
		}
		{
			ImageLoader loader(file_front_);
			int w, h;
			std::vector<unsigned char> image;
			bool load_result = loader.LoadWithoutAlpha(&image, &w, &h);
			if(!load_result) {
				return false;
			}
			//glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.data());
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, image.data());
		}
		
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		
		return true;
		
	}
	bool TextureCube::Deinit()
	{
		if(tex_ == 0) {
			return false;
		}
		glDeleteTextures(1, &tex_);
		tex_ = 0;
		return true;
	}
} // namespace gl
} // namespace haruna