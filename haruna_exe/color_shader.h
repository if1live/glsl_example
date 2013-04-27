// Ŭnicode please 
#include "renderer.h"

class ColorShader : public AbstractLogic {
public:
	ColorShader(float width, float height);
	virtual ~ColorShader();

	virtual bool Init();
	virtual bool Update(float dt);
	virtual void Draw();

private:
	std::unique_ptr<haruna::gl::ShaderProgram> prog_;
	
	float y_rot_;
};
