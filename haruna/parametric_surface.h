// Ŭnicode please 
#pragma once

#include <vector>

#include "vertex.h"

namespace haruna {;

struct ISurface {
	virtual int GetVertexCount() const = 0;
	virtual int GetLineIndexCount() const = 0;
	virtual int GetTriangleIndexCount() const = 0;
	virtual void GenerateVertices(std::vector<Vertex_1P1N1UV> &vertices) const = 0;
	virtual void GenerateVertices(std::vector<Vertex_1P1N1UV1T> &vertices) const = 0;
	virtual void GenerateLineIndices(std::vector<unsigned short>& indices) const = 0;
	virtual void GenerateTriangleIndices(std::vector<unsigned short>& indices) const = 0;
	virtual ~ISurface() {}
};


struct ParametricInterval {
	glm::ivec2 divisions;
	glm::vec2 upper_bound;
	glm::vec2 texture_count;
};

class ParametricSurface : public ISurface {
public:
	int GetVertexCount() const;
	int GetLineIndexCount() const;
	int GetTriangleIndexCount() const;
	//position, normal, texcoord 전부 함유
	void GenerateVertices(std::vector<Vertex_1P1N1UV> &vertices) const;
	void GenerateVertices(std::vector<Vertex_1P1N1UV1T> &vertices) const;
	void GenerateLineIndices(std::vector<unsigned short>& indices) const;
	void GenerateTriangleIndices(std::vector<unsigned short>& indices) const;

protected:
	void SetInterval(const ParametricInterval& interval);
	virtual glm::vec3 Evaluate(const glm::vec2& domain) const = 0;
	virtual bool InvertNormal(const glm::vec2& domain) const { return false; }

private:
	//필요에 따라서 normal만 있는거, texcoord만 있는거 같은식으로 구분하면
	//복잡도가 커져서 그냥 이거 안쓰고 Vertex를 쓰는거로 수정햇다
	void GenerateVertices(std::vector<float>& vertices, unsigned char flags) const;
	glm::vec2 ComputeDomain(float i, float j) const;
	glm::ivec2 m_slices;
	glm::ivec2 m_divisions;
	glm::vec2 m_upperBound;
	glm::vec2 m_textureCount;
};

}	// namespace haruna