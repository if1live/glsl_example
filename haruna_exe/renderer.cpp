// Ŭnicode please 
#include "renderer.h"

#include <GL/glew.h>
#include "sora/OVR_Types.h"

GLenum ToDrawMode(haruna::DrawType type)
{
	if(type == haruna::kDrawTriangles) {
		return GL_TRIANGLES;
	} else {
		OVR_ASSERT(!"what is?");
		return GL_TRIANGLES;
	}
}
