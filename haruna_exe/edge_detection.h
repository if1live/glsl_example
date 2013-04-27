// Ŭnicode please 
#pragma once

#include "renderer.h"

namespace haruna {;
class ParametricSurface;
namespace gl {
	class FrameBuffer;
}
}

class EdgeDetection : public AbstractLogic {
public:
	EdgeDetection(float width, float height);
	virtual ~EdgeDetection();

	virtual bool Init();
	virtual bool Update(float dt);
	virtual void Draw();

	bool DrawPostEffect(haruna::gl::ShaderProgram &prog);

private:
	bool InitNoEffect();
	bool InitEdgeDetection();
	bool InitEmboss();

private:
	std::unique_ptr<haruna::gl::ShaderProgram> prog_;

	std::unique_ptr<haruna::gl::Texture2D> diffuse_map_;
	std::unique_ptr<haruna::gl::Texture2D> specular_map_;
	std::unique_ptr<haruna::gl::Texture2D> normal_map_;

	std::unique_ptr<haruna::gl::TextureCube> environment_map_;

	std::unique_ptr<haruna::ParametricSurface> mesh_;

	std::unique_ptr<haruna::gl::FrameBuffer> fb_;

	std::unique_ptr<haruna::gl::ShaderProgram> no_effect_prog_;
	std::unique_ptr<haruna::gl::ShaderProgram> emboss_prog_;
	std::unique_ptr<haruna::gl::ShaderProgram> edge_detection_prog_;

	int effect_type_;

	float y_rot_;
};