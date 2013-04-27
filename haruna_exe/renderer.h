// Ŭnicode please 
#pragma once

#include <GL/glew.h>
#include <memory>
#include "haruna/primitive_mesh.h"

namespace haruna {;
namespace gl {
	class ShaderProgram;
	class Texture2D;
	class TextureCube;
}	// namespace haruna
}	// namespace gl

class AbstractLogic {
public:
	AbstractLogic(float width, float height) : width_(width), height_(height) {}
	virtual ~AbstractLogic() {}

	virtual bool Init() = 0;
	virtual bool Update(float dt) = 0;
	virtual void Draw() = 0;

	float width() const { return width_; }
	float height() const { return height_; }

private:
	float width_;
	float height_;
};

GLenum ToDrawMode(haruna::DrawType type);