// Ŭnicode please 
#pragma once

#include "renderer.h"

namespace haruna {;
class ParametricSurface;
}

class EnvironmentMapping : public AbstractLogic {
public:
	EnvironmentMapping(float width, float height);
	virtual ~EnvironmentMapping();

	virtual bool Init();
	virtual bool Update(float dt);
	virtual void Draw();

private:
	std::unique_ptr<haruna::gl::ShaderProgram> prog_;

	std::unique_ptr<haruna::gl::Texture2D> diffuse_map_;
	std::unique_ptr<haruna::gl::Texture2D> specular_map_;
	std::unique_ptr<haruna::gl::Texture2D> normal_map_;

	std::unique_ptr<haruna::gl::TextureCube> environment_map_;

	std::unique_ptr<haruna::ParametricSurface> mesh_;

	float y_rot_;
};