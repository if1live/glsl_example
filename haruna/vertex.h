// Ŭnicode please 
#pragma once

#include <glm/glm.hpp>

namespace haruna {;
class Vertex_1P {
public:
	glm::vec3 p;
};

class Vertex_1P1C {
public:
	glm::vec3 p;
	unsigned char color[3];
};

class Vertex_1P1UV {
public:
	glm::vec3 p;
	glm::vec2 uv;
};

class Vertex_1P1N {
public:
	glm::vec3 p;
	glm::vec3 n;
};

class Vertex_1P1N1UV {
public:
	glm::vec3 p;
	glm::vec3 n;
	glm::vec2 uv;
};

class Vertex_1P1N1UV1T {
public:
	glm::vec3 p;
	glm::vec3 n;
	glm::vec2 uv;
	glm::vec3 t;	//tangent
};

}	// namespace haruna