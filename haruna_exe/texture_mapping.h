// Ŭnicode please 
#include "renderer.h"

class Texture2DMapping : public AbstractLogic {
public:
	Texture2DMapping(float width, float height);
	virtual ~Texture2DMapping();

	virtual bool Init();
	virtual bool Update(float dt);
	virtual void Draw();

private:
	std::unique_ptr<haruna::gl::ShaderProgram> prog_;
	std::unique_ptr<haruna::gl::Texture2D> tex_;
	
	float y_rot_;
};

