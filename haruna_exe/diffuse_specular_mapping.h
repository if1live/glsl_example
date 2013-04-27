// Ŭnicode please 
#include "renderer.h"

class DiffuseSpecularMapping : public AbstractLogic {
public:
	DiffuseSpecularMapping(float width, float height);
	virtual ~DiffuseSpecularMapping();

	virtual bool Init();
	virtual bool Update(float dt);
	virtual void Draw();

private:
	std::unique_ptr<haruna::gl::ShaderProgram> prog_;
	std::unique_ptr<haruna::gl::Texture2D> diffuse_map_;
	std::unique_ptr<haruna::gl::Texture2D> specular_map_;

	float y_rot_;
};