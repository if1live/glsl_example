// Ŭnicode please 
#pragma once

#include "renderer.h"

namespace haruna {;
class ParametricSurface;
namespace gl {
	class FrameBuffer;
}
}


class ColorConversion : public AbstractLogic {
public:
	ColorConversion(float width, float height);
	virtual ~ColorConversion();

	virtual bool Init();
	virtual bool Update(float dt);
	virtual void Draw();

	bool DrawPostEffect(haruna::gl::ShaderProgram &prog);

private:
	bool InitNoEffect();
	bool InitGrayScale();
	bool InitSepia();

private:
	std::unique_ptr<haruna::gl::ShaderProgram> prog_;

	std::unique_ptr<haruna::gl::Texture2D> diffuse_map_;
	std::unique_ptr<haruna::gl::Texture2D> specular_map_;
	std::unique_ptr<haruna::gl::Texture2D> normal_map_;

	std::unique_ptr<haruna::gl::TextureCube> environment_map_;

	std::unique_ptr<haruna::ParametricSurface> mesh_;

	std::unique_ptr<haruna::gl::FrameBuffer> fb_;

	//effect
	std::unique_ptr<haruna::gl::ShaderProgram> no_effect_prog_;
	std::unique_ptr<haruna::gl::ShaderProgram> gray_scale_prog_;
	std::unique_ptr<haruna::gl::ShaderProgram> sepia_prog_;

	int effect_type_;

	float y_rot_;
};