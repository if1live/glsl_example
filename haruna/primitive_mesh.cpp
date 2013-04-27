// Ŭnicode please 
#include "stdafx.h"
#include "primitive_mesh.h"
#include "sora/math_helper.h"

using glm::vec3;
using glm::vec2;

namespace haruna {;

SolidCubeFactory::SolidCubeFactory(float width, float height, float depth)
	: width_(width), height_(height), depth_(depth)
{
	SR_ASSERT(width > 0 && height > 0 && depth > 0);
}

std::vector<DrawCmdData<Vertex_1P1N1UV>> SolidCubeFactory::CreateNormalMesh()
{
	float width = width_/2;
	float height = height_/2;
	float depth = depth_/2;

	DrawCmdData<Vertex_1P1N1UV> cmd;
	cmd.draw_mode = kDrawTriangles;
	DrawCmdData<Vertex_1P1N1UV>::VertexList &vert_list = cmd.vertex_list;
	std::vector<unsigned short> &index_list = cmd.index_list;

	//normal
	{
		// Front Face
		int baseIndex = vert_list.size();
		vec3 normal(0, 0, +1);

		vec2 texCoord1(0, 0);	vec3 vertex1(-width, -height, depth);
		vec2 texCoord2(1, 0);	vec3 vertex2( width, -height, depth);
		vec2 texCoord3(1, 1);	vec3 vertex3( width,  height, depth);
		vec2 texCoord4(0, 1);	vec3 vertex4(-width,  height, depth);

		//add vertex
		Vertex_1P1N1UV v1;  v1.p = vertex1; v1.uv = texCoord1;  v1.n = normal;
		Vertex_1P1N1UV v2;  v2.p = vertex2; v2.uv = texCoord2;  v2.n = normal;
		Vertex_1P1N1UV v3;  v3.p = vertex3; v3.uv = texCoord3;  v3.n = normal;
		Vertex_1P1N1UV v4;  v4.p = vertex4; v4.uv = texCoord4;  v4.n = normal;

		vert_list.push_back(v1);
		vert_list.push_back(v2);
		vert_list.push_back(v3);
		vert_list.push_back(v4);

		//add index
		index_list.push_back(baseIndex + 0);
		index_list.push_back(baseIndex + 1);
		index_list.push_back(baseIndex + 2);

		index_list.push_back(baseIndex + 0);
		index_list.push_back(baseIndex + 2);
		index_list.push_back(baseIndex + 3);
	}
	{
		// Back Face
		int baseIndex = vert_list.size();
		vec3 normal(0, 0, -1);

		vec2 texCoord1(1, 0);	vec3 vertex1(-width, -height, -depth);
		vec2 texCoord2(1, 1);	vec3 vertex2(-width,  height, -depth);
		vec2 texCoord3(0, 1);	vec3 vertex3( width,  height, -depth);
		vec2 texCoord4(0, 0);	vec3 vertex4( width, -height, -depth);

		//add vertex
		Vertex_1P1N1UV v1;  v1.p = vertex1; v1.uv = texCoord1;  v1.n = normal;
		Vertex_1P1N1UV v2;  v2.p = vertex2; v2.uv = texCoord2;  v2.n = normal;
		Vertex_1P1N1UV v3;  v3.p = vertex3; v3.uv = texCoord3;  v3.n = normal;
		Vertex_1P1N1UV v4;  v4.p = vertex4; v4.uv = texCoord4;  v4.n = normal;

		vert_list.push_back(v1);
		vert_list.push_back(v2);
		vert_list.push_back(v3);
		vert_list.push_back(v4);

		//add index
		index_list.push_back(baseIndex + 0);
		index_list.push_back(baseIndex + 1);
		index_list.push_back(baseIndex + 2);

		index_list.push_back(baseIndex + 0);
		index_list.push_back(baseIndex + 2);
		index_list.push_back(baseIndex + 3);
	}

	{
		// Top Face
		int baseIndex = vert_list.size();
		vec3 normal(0, 1, 0);

		vec2 texCoord1(0, 1);	vec3 vertex1(-width, height, -depth);
		vec2 texCoord2(0, 0);	vec3 vertex2(-width, height,  depth);
		vec2 texCoord3(1, 0);	vec3 vertex3( width, height,  depth);
		vec2 texCoord4(1, 1);	vec3 vertex4( width, height, -depth);

		//add vertex
		Vertex_1P1N1UV v1;  v1.p = vertex1; v1.uv = texCoord1;  v1.n = normal;
		Vertex_1P1N1UV v2;  v2.p = vertex2; v2.uv = texCoord2;  v2.n = normal;
		Vertex_1P1N1UV v3;  v3.p = vertex3; v3.uv = texCoord3;  v3.n = normal;
		Vertex_1P1N1UV v4;  v4.p = vertex4; v4.uv = texCoord4;  v4.n = normal;

		vert_list.push_back(v1);
		vert_list.push_back(v2);
		vert_list.push_back(v3);
		vert_list.push_back(v4);

		//add index
		index_list.push_back(baseIndex + 0);
		index_list.push_back(baseIndex + 1);
		index_list.push_back(baseIndex + 2);

		index_list.push_back(baseIndex + 0);
		index_list.push_back(baseIndex + 2);
		index_list.push_back(baseIndex + 3);
	}

	{
		// Bottom Face
		int baseIndex = vert_list.size();
		vec3 normal(0, -1, 0);

		vec2 texCoord1(1, 1);	vec3 vertex1(-width, -height, -depth);
		vec2 texCoord2(0, 1);	vec3 vertex2( width, -height, -depth);
		vec2 texCoord3(0, 0);	vec3 vertex3( width, -height,  depth);
		vec2 texCoord4(1, 0);	vec3 vertex4(-width, -height,  depth);

		//add vertex
		Vertex_1P1N1UV v1;  v1.p = vertex1; v1.uv = texCoord1;  v1.n = normal;
		Vertex_1P1N1UV v2;  v2.p = vertex2; v2.uv = texCoord2;  v2.n = normal;
		Vertex_1P1N1UV v3;  v3.p = vertex3; v3.uv = texCoord3;  v3.n = normal;
		Vertex_1P1N1UV v4;  v4.p = vertex4; v4.uv = texCoord4;  v4.n = normal;

		vert_list.push_back(v1);
		vert_list.push_back(v2);
		vert_list.push_back(v3);
		vert_list.push_back(v4);

		//add index
		index_list.push_back(baseIndex + 0);
		index_list.push_back(baseIndex + 1);
		index_list.push_back(baseIndex + 2);

		index_list.push_back(baseIndex + 0);
		index_list.push_back(baseIndex + 2);
		index_list.push_back(baseIndex + 3);
	}

	{
		// Right face
		int baseIndex = vert_list.size();
		vec3 normal(1, 0, 0);

		vec2 texCoord1(1, 0);	vec3 vertex1(width, -height, -depth);
		vec2 texCoord2(1, 1);	vec3 vertex2(width,  height, -depth);
		vec2 texCoord3(0, 1);	vec3 vertex3(width,  height,  depth);
		vec2 texCoord4(0, 0);	vec3 vertex4(width, -height,  depth);

		//add vertex
		Vertex_1P1N1UV v1;  v1.p = vertex1; v1.uv = texCoord1;  v1.n = normal;
		Vertex_1P1N1UV v2;  v2.p = vertex2; v2.uv = texCoord2;  v2.n = normal;
		Vertex_1P1N1UV v3;  v3.p = vertex3; v3.uv = texCoord3;  v3.n = normal;
		Vertex_1P1N1UV v4;  v4.p = vertex4; v4.uv = texCoord4;  v4.n = normal;

		vert_list.push_back(v1);
		vert_list.push_back(v2);
		vert_list.push_back(v3);
		vert_list.push_back(v4);

		//add index
		index_list.push_back(baseIndex + 0);
		index_list.push_back(baseIndex + 1);
		index_list.push_back(baseIndex + 2);

		index_list.push_back(baseIndex + 0);
		index_list.push_back(baseIndex + 2);
		index_list.push_back(baseIndex + 3);
	}

	{
		// Left Face
		int baseIndex = vert_list.size();
		vec3 normal(-1, 0, 0);

		vec2 texCoord1(0, 0);	vec3 vertex1(-width, -height, -depth);
		vec2 texCoord2(1, 0);	vec3 vertex2(-width, -height,  depth);
		vec2 texCoord3(1, 1);	vec3 vertex3(-width,  height,  depth);
		vec2 texCoord4(0, 1);	vec3 vertex4(-width,  height, -depth);

		//add vertex
		Vertex_1P1N1UV v1;  v1.p = vertex1; v1.uv = texCoord1;  v1.n = normal;
		Vertex_1P1N1UV v2;  v2.p = vertex2; v2.uv = texCoord2;  v2.n = normal;
		Vertex_1P1N1UV v3;  v3.p = vertex3; v3.uv = texCoord3;  v3.n = normal;
		Vertex_1P1N1UV v4;  v4.p = vertex4; v4.uv = texCoord4;  v4.n = normal;

		vert_list.push_back(v1);
		vert_list.push_back(v2);
		vert_list.push_back(v3);
		vert_list.push_back(v4);

		//add index
		index_list.push_back(baseIndex + 0);
		index_list.push_back(baseIndex + 1);
		index_list.push_back(baseIndex + 2);

		index_list.push_back(baseIndex + 0);
		index_list.push_back(baseIndex + 2);
		index_list.push_back(baseIndex + 3);
	}

	std::vector<DrawCmdData<Vertex_1P1N1UV>> data_list;
	data_list.push_back(cmd);
	return data_list;
}

SolidSphereFactory::SolidSphereFactory(float radius, int slices, int stacks)
	: radius_(radius), slices_(slices), stacks_(stacks)
{
	
}

std::vector<DrawCmdData<Vertex_1P1N1UV>> SolidSphereFactory::CreateNormalMesh()
{
	bool normals = true;
	float nsign = 1.0f;
	float drho = static_cast<float>(sora::kPi / stacks_);
	float dtheta = static_cast<float>(2.0f * sora::kPi / slices_);

	float ds = 1.0f / slices_;
	float dt = 1.0f / stacks_;
	float t = 1.0f; // because loop now runs from 0
	int imin = 0;
	int imax = stacks_;

	typedef DrawCmdData<Vertex_1P1N1UV>::VertexList VertexList;
	typedef DrawCmdData<Vertex_1P1N1UV>::IndexList IndexList;

	DrawCmdData<Vertex_1P1N1UV> cmd;
	cmd.draw_mode = kDrawTriangles;
	VertexList &vert_list = cmd.vertex_list;
	IndexList &index_list = cmd.index_list;

	// draw intermediate stacks as quad strips
	for (int i = imin; i < imax; i++) {
		float rho = i * drho;

		//quad strip로 구성된 vertex 목록 구성하기
		VertexList tmp_vert_list;
		float s = 0.0f;
		for (int j = 0; j <= slices_; j++) {
			float theta = (j == slices_) ? 0.0f : j * dtheta;
			float x = -sin(theta) * sin(rho);
			float y = cos(theta) * sin(rho);
			float z = nsign * cos(rho);

			Vertex_1P1N1UV vert1;
			vert1.n = (vec3(x * nsign, y * nsign, z * nsign));
			vert1.uv = vec2(s, t);
			vert1.p = vec3(x * radius_, y * radius_, z * radius_);
			tmp_vert_list.push_back(vert1);

			x = -sin(theta) * sin(rho + drho);
			y = cos(theta) * sin(rho + drho);
			z = nsign * cos(rho + drho);

			Vertex_1P1N1UV vert2;
			vert2.n = vec3(x * nsign, y * nsign, z * nsign);
			vert2.uv = vec2(s, t - dt);
			s += ds;
			vert2.p = vec3(x * radius_, y * radius_, z * radius_);
			tmp_vert_list.push_back(vert2);
		}

		//DrawCmdData<vec3> cmd;
		//cmd.draw_mode = kDrawTriangleStrip;
		//quad strip -> triangle strip
		//cmd.vertex_list = vert_list;
		//this->cmd_list_->push_back(cmd);
		int base_vert_list_size = vert_list.size();
		std::copy(tmp_vert_list.begin(), tmp_vert_list.end(), back_inserter(vert_list));

		for(int i = 0 ; i < (int)tmp_vert_list.size()-2 ; ++i) {
			unsigned short idx1, idx2, idx3;
			if(i % 2 == 1) {
				idx1 = i;
				idx2 = i + 2;
				idx3 = i + 1;
			} else {
				idx1 = i;
				idx2 = i + 1;
				idx3 = i + 2;
			}
			index_list.push_back(base_vert_list_size + idx1);
			index_list.push_back(base_vert_list_size + idx2);
			index_list.push_back(base_vert_list_size + idx3);
		}

		t -= dt;
	}
	std::vector<DrawCmdData<Vertex_1P1N1UV>> data_list;
	data_list.push_back(cmd);
	return data_list;
}
/*
WireCubeDataFactory::WireCubeDataFactory(float width, float height, float depth)
	: width_(width), height_(height), depth_(depth)
{
	SR_ASSERT(width > 0 && height > 0 && depth > 0);
	width = width/2;
	height = height/2;
	depth = depth/2;

	vec3 v0(-width, height, -depth);
	vec3 v1(width, height, -depth);
	vec3 v2(-width, height, depth);
	vec3 v3(width, height, depth);

	vec3 v4(-width, -height, -depth);
	vec3 v5(width, -height, -depth);
	vec3 v6(-width, -height, depth);
	vec3 v7(width, -height, depth);

	vector<vec3> pos_list(8);
	pos_list[0] = v0;
	pos_list[1] = v1;
	pos_list[2] = v2;
	pos_list[3] = v3;
	pos_list[4] = v4;
	pos_list[5] = v5;
	pos_list[6] = v6;
	pos_list[7] = v7;

	DrawCmdData<Vertex> cmd;
	cmd.draw_mode = kDrawLines;
	for(size_t i = 0 ; i < pos_list.size() ; ++i) {
		Vertex vert;
		vert.set_pos(pos_list[i]);
		cmd.vertex_list.push_back(vert);
	}

	//GL_LINES용 index list
	std::array<GLushort, 24> index_list = {
		0,1,	1,3,	2,3,	0,2,
		4,5,	5,7,	6,7,	4,6,
		0,4,	1,5,	2,6,	3,7
	};
	cmd.index_list.resize(index_list.size());
	copy(index_list.begin(), index_list.end(), cmd.index_list.begin());
	this->cmd_list_->push_back(cmd);
}

WireSphereDataFactory::WireSphereDataFactory(float radius, int slices, int stacks)
	: radius_(radius), slices_(slices), stacks_(stacks)
{
	float nsign = 1.0f;
	float drho = sora::kPi / stacks;
	float dtheta = 2.0f * sora::kPi / slices;

	//한방에 그릴수 잇도록하자.
	//vertex list + index로 구성을 변경한다는 소리
	//구성 방법은 gl_lines
	DrawCmdData<Vertex> cmd;
	cmd.draw_mode = kDrawLines;
	vector<Vertex> &vert_list = cmd.vertex_list;
	vector<unsigned short> &index_list = cmd.index_list;

	// draw stack lines
	for (int i = 1 ; i < stacks ; i++) { // stack line at i==stacks-1 was missing here
		float rho = i * drho;

		vector<Vertex> tmp_vert_list;
		for (int j = 0; j < slices; j++) {
			float theta = j * dtheta;
			float x = cos(theta) * sin(rho);
			float y = sin(theta) * sin(rho);
			float z = cos(rho);

			Vertex vert;
			vert.n[0] = x * nsign;
			vert.n[1] = y * nsign;
			vert.n[2] = z * nsign;

			vert.p[0] = x * radius;
			vert.p[1] = y * radius;
			vert.p[2] = z * radius;
			tmp_vert_list.push_back(vert);
		}
		int base_vert_list_size = vert_list.size();
		//copy vertex
		std::copy(tmp_vert_list.begin(), tmp_vert_list.end(), back_inserter(vert_list));

		for(int i = 0 ; i < (int)tmp_vert_list.size() ; i++) {
			unsigned short idx1 = base_vert_list_size + i;
			unsigned short idx2 = base_vert_list_size + ((i + 1) % (int)tmp_vert_list.size());
			index_list.push_back(idx1);
			index_list.push_back(idx2);
		}
	}
	// draw slice lines
	for (int j = 0; j < slices; j++) {
		float theta = j * dtheta;

		vector<Vertex> tmp_vert_list;
		for (int i = 0; i <= stacks; i++) {
			float rho = i * drho;
			float x = cos(theta) * sin(rho);
			float y = sin(theta) * sin(rho);
			float z = cos(rho);

			Vertex vert;
			vert.n[0] = x * nsign;
			vert.n[1] = y * nsign;
			vert.n[2] = z * nsign;

			vert.p[0] = x * radius;
			vert.p[1] = y * radius;
			vert.p[2] = z * radius;
			tmp_vert_list.push_back(vert);
		}
		int base_vert_list_size = vert_list.size();
		std::copy(tmp_vert_list.begin(), tmp_vert_list.end(), back_inserter(vert_list));

		for(int i = 0 ; i < (int)tmp_vert_list.size() - 1 ; i++) {
			unsigned short idx1 = base_vert_list_size + i;
			unsigned short idx2 = base_vert_list_size + ((i + 1) % (int)tmp_vert_list.size());
			index_list.push_back(idx1);
			index_list.push_back(idx2);
		}
	}
	this->cmd_list_->push_back(cmd);
}

AxisDataFactory::AxisDataFactory(float size)
	: size_(size)
{
	DrawCmdData<Vertex> cmd;
	cmd.draw_mode = kDrawLines;
	VertexList &vert_list = cmd.vertex_list;

	//vertex list 생성
	vec3 xPos(size, 0, 0);
	vec3 yPos(0, size, 0);
	vec3 zPos(0, 0, size);
	vec3 zero(0, 0, 0);

	sora::vec4ub red(255, 0, 0, 255);
	sora::vec4ub green(0, 255, 0, 255);
	sora::vec4ub blue(0, 0, 255, 255);

	{
		// x axis - r
		Vertex x_zero_vert;
		x_zero_vert.color = red;
		x_zero_vert.p = zero;
		vert_list.push_back(x_zero_vert);

		Vertex x_one_vert;
		x_one_vert.color = red;
		x_one_vert.p = xPos;
		vert_list.push_back(x_one_vert);
	}
	{
		//y axis - g
		Vertex zero_vert;
		zero_vert.color = green;
		zero_vert.p = zero;
		vert_list.push_back(zero_vert);

		Vertex y_vert;
		y_vert.color = green;
		y_vert.p = yPos;
		vert_list.push_back(y_vert);
	}
	{
		//z axis - b
		Vertex zero_vert;
		zero_vert.color = blue;
		zero_vert.p = zero;
		vert_list.push_back(zero_vert);

		Vertex z_vert;
		z_vert.color = blue;
		z_vert.p = zPos;
		vert_list.push_back(z_vert);
	}
	this->cmd_list_->push_back(cmd);
}
*/

}	//namespace haruna