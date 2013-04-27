// Ŭnicode please 
#include "renderer.h"

namespace haruna {;
class ParametricSurface;
}


class UVAnimation : public AbstractLogic {
public:
	UVAnimation(float width, float height);
	virtual ~UVAnimation();

	virtual bool Init();
	virtual bool Update(float dt);
	virtual void Draw();

private:
	std::unique_ptr<haruna::gl::ShaderProgram> prog_;
	std::unique_ptr<haruna::gl::Texture2D> diffuse_map_;
	std::unique_ptr<haruna::gl::Texture2D> specular_map_;

	std::unique_ptr<haruna::ParametricSurface> mesh_;

	float y_rot_;
	float elapsed_time_;
};