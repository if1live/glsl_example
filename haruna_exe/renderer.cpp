// Ŭnicode please 
#include "renderer.h"

#include <GL/glew.h>
#include "sora/assert_inc.h"

GLenum ToDrawMode(haruna::DrawType type)
{
	if(type == haruna::kDrawTriangles) {
		return GL_TRIANGLES;
	} else {
		SR_ASSERT(!"what is?");
		return GL_TRIANGLES;
	}
}
