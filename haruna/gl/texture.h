// Ŭnicode please 
#pragma once

namespace haruna {;
namespace gl {
	class ImageLoader {
	public:
		ImageLoader(const std::string &filename) : filename_(filename) {}
		~ImageLoader() {}

		bool Load(std::vector<unsigned char> *data, int *w, int *h);
		bool LoadWithoutAlpha(std::vector<unsigned char> *data, int *w, int *h);
	private:
		const std::string filename_;
	};

	class Texture2D {
	public:
		Texture2D();
		Texture2D(const std::string &filename);
		~Texture2D();

		bool Init();
		bool Init(GLuint tex_id, int w, int h);
		bool Deinit();

		GLuint tex() const { return tex_; }
		operator GLuint() const { return tex_; }
	private:
		GLuint tex_;
		std::string filename_;
		int width_;
		int height_;
	};

	class TextureCube {
	public:
		TextureCube(
			const std::string &file_left, 
			const std::string &file_right,
			const std::string &file_bottom,
			const std::string &file_top,
			const std::string &file_back,
			const std::string &file_front
			);
		~TextureCube();

		bool Init();
		bool Deinit();

		GLuint tex() const { return tex_; }
		operator GLuint() const { return tex_; }
	private:
		GLuint tex_;
		std::string file_left_;
		std::string file_right_;
		std::string file_bottom_;
		std::string file_top_;
		std::string file_back_;
		std::string file_front_;
	};
} // namespace gl
} // namespace haruna
