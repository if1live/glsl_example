// Ŭnicode please 
#include "renderer.h"

class Lighting : public AbstractLogic {
public:
	Lighting(float width, float height);
	virtual ~Lighting();

	virtual bool Init();
	virtual bool Update(float dt);
	virtual void Draw();

private:
	std::unique_ptr<haruna::gl::ShaderProgram> prog_;

	float y_rot_;
};

