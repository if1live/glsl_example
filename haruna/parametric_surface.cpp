// Ŭnicode please 
#include "stdafx.h"
#include "parametric_surface.h"

namespace haruna {;

enum VertexFlags {
	kVertexFlagsNormals = 1 << 0,
	kVertexFlagsTexCoords = 1 << 1,
	kVertexFlagsTangents = 1 << 2,
};

void ParametricSurface::SetInterval(const ParametricInterval& interval) 
{
	m_divisions = interval.divisions;
	m_upperBound = interval.upper_bound;
	m_textureCount = interval.texture_count;
	m_slices = m_divisions - glm::ivec2(1, 1);
}

int ParametricSurface::GetVertexCount() const 
{
	return m_divisions.x * m_divisions.y;
}

int ParametricSurface::GetLineIndexCount() const 
{
	return 4 * m_slices.x * m_slices.x;
}

int ParametricSurface::GetTriangleIndexCount() const 
{
	return 6 * m_slices.x * m_slices.y;
}

glm::vec2 ParametricSurface::ComputeDomain(float x, float y) const 
{
	return glm::vec2(x * m_upperBound.x / m_slices.x, y * m_upperBound.y / m_slices.y);
}

void ParametricSurface::GenerateVertices(std::vector<float>& vertices, unsigned char flags) const 
{
	int floatsPerVertex = 3;
	if (flags & kVertexFlagsNormals) {
		floatsPerVertex += 3;
	}
	if (flags & kVertexFlagsTexCoords) {
		floatsPerVertex += 2;
	}
	if(flags & kVertexFlagsTangents) {
		floatsPerVertex += 3;
	}


	vertices.resize(GetVertexCount() * floatsPerVertex);
	float* attribute = &vertices[0];

	for (int j = 0; j < m_divisions.y; j++) {
		for (int i = 0; i < m_divisions.x; i++) {

			// Compute Position
			glm::vec2 domain = ComputeDomain((float)i, (float)j);
			glm::vec3 range = Evaluate(domain);
			memcpy(attribute, &range, sizeof(range));
			attribute = (float*)(((unsigned char*)attribute) + sizeof(range));
			//attribute = range.Write(attribute);

			// Compute Normal
			if (flags & kVertexFlagsNormals) {
				float s = (float)i, t = (float)j;

				// Nudge the point if the normal is indeterminate.
				if (i == 0) s += 0.01f;
				if (i == m_divisions.x - 1) s -= 0.01f;
				if (j == 0) t += 0.01f;
				if (j == m_divisions.y - 1) t -= 0.01f;

				// Compute the tangents and their cross product.
				glm::vec3 p = Evaluate(ComputeDomain(s, t));
				glm::vec3 u = Evaluate(ComputeDomain(s + 0.01f, t)) - p;
				glm::vec3 v = Evaluate(ComputeDomain(s, t + 0.01f)) - p;
				glm::vec3 normal = glm::normalize(glm::cross(u, v));
				if (InvertNormal(domain)) {
					normal = -normal;
				}
				//attribute = normal.Write(attribute);
				memcpy(attribute, &normal, sizeof(normal));
				attribute = (float*)(((unsigned char*)attribute) + sizeof(normal));
			}

			// Compute Texture2D2D Coordinates
			if (flags & kVertexFlagsTexCoords) {
				float s = m_textureCount.x * i / m_slices.x;
				float t = m_textureCount.y * j / m_slices.y;
				glm::vec2 texcoord(s, t);
				//attribute = vec2(s, t).Write(attribute);
				memcpy(attribute, &texcoord, sizeof(texcoord));
				attribute = (float*)(((unsigned char*)attribute) + sizeof(texcoord));
			}

			if(flags & kVertexFlagsTangents) {
				float s = (float)i, t = (float)j;
				glm::vec3 p = Evaluate(ComputeDomain(s, t));
				glm::vec3 u = Evaluate(ComputeDomain(s + 0.01f, t)) - p;
				if(InvertNormal(domain)) {
					u = -u;
				}
				memcpy(attribute, &u, sizeof(u));
				attribute = (float*)(((unsigned char*)attribute) + sizeof(u));
			}
		}
	}
}

void ParametricSurface::GenerateLineIndices(std::vector<unsigned short>& indices) const 
{
	indices.resize(GetLineIndexCount());
	std::vector<unsigned short>::iterator index = indices.begin();
	for (int j = 0, vertex = 0; j < m_slices.y; j++) {
		for (int i = 0; i < m_slices.x; i++) {
			int next = (i + 1) % m_divisions.x;
			*index++ = vertex + i;
			*index++ = vertex + next;
			*index++ = vertex + i;
			*index++ = vertex + i + m_divisions.x;
		}
		vertex += m_divisions.x;
	}
}

void ParametricSurface::GenerateTriangleIndices(std::vector<unsigned short>& indices) const 
{
	indices.resize(GetTriangleIndexCount());
	std::vector<unsigned short>::iterator index = indices.begin();
	for (int j = 0, vertex = 0; j < m_slices.y; j++) {
		for (int i = 0; i < m_slices.x; i++) {
			int next = (i + 1) % m_divisions.x;
			*index++ = vertex + i;
			*index++ = vertex + next;
			*index++ = vertex + i + m_divisions.x;
			*index++ = vertex + next;
			*index++ = vertex + next + m_divisions.x;
			*index++ = vertex + i + m_divisions.x;
		}
		vertex += m_divisions.x;
	}
}

void ParametricSurface::GenerateVertices(std::vector<Vertex_1P1N1UV> &vertices) const 
{
	std::vector<float> raw_vert_list;
	GenerateVertices(raw_vert_list, kVertexFlagsNormals | kVertexFlagsTexCoords);

	vertices.clear();
	vertices.resize(raw_vert_list.size() / 8);

	for(size_t i = 0 ; i < raw_vert_list.size() / 8 ; i++) {
		Vertex_1P1N1UV &v = vertices[i];
		v.p[0] = raw_vert_list[i*8 + 0];
		v.p[1] = raw_vert_list[i*8 + 1];
		v.p[2] = raw_vert_list[i*8 + 2];

		v.n[0] = raw_vert_list[i*8 + 3];
		v.n[1] = raw_vert_list[i*8 + 4];
		v.n[2] = raw_vert_list[i*8 + 5];

		v.uv[0] = raw_vert_list[i*8 + 6];
		v.uv[1] = raw_vert_list[i*8 + 7];
	}
}

void ParametricSurface::GenerateVertices(std::vector<Vertex_1P1N1UV1T> &vertices) const 
{
	std::vector<float> raw_vert_list;
	unsigned int flag = 0;
	flag |= kVertexFlagsNormals;
	flag |= kVertexFlagsTexCoords;
	flag |= kVertexFlagsTangents;
	GenerateVertices(raw_vert_list, flag);

	int scanline = 11;
	vertices.clear();
	vertices.resize(raw_vert_list.size() / scanline);

	for(size_t i = 0 ; i < raw_vert_list.size() / scanline ; i++) {
		Vertex_1P1N1UV1T &v = vertices[i];
		v.p[0] = raw_vert_list[i*scanline + 0];
		v.p[1] = raw_vert_list[i*scanline + 1];
		v.p[2] = raw_vert_list[i*scanline + 2];

		v.n[0] = raw_vert_list[i*scanline + 3];
		v.n[1] = raw_vert_list[i*scanline + 4];
		v.n[2] = raw_vert_list[i*scanline + 5];

		v.uv[0] = raw_vert_list[i*scanline + 6];
		v.uv[1] = raw_vert_list[i*scanline + 7];

		v.t[0] = raw_vert_list[i*scanline + 8];
		v.t[1] = raw_vert_list[i*scanline + 9];
		v.t[2] = raw_vert_list[i*scanline + 10];
	}
}
}	// namespace haruna