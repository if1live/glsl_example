// Ŭnicode please 
#include "renderer.h"

class ToonShader : public AbstractLogic {
public:
	ToonShader(float width, float height);
	virtual ~ToonShader();

	virtual bool Init();
	virtual bool Update(float dt);
	virtual void Draw();

private:
	std::unique_ptr<haruna::gl::ShaderProgram> prog_;

	float y_rot_;
};