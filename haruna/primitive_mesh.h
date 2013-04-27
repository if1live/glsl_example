// Ŭnicode please 
#pragma once
#include <vector>
#include "vertex.h"

namespace haruna {;

typedef enum {
	kDrawPoints,
	kDrawLines,
	kDrawLineStrip,
	kDrawLineLoop,
	kDrawTriangles,
	kDrawTriangleStrip,
	kDrawTriangleFan,
	kDrawTypeCount,
} DrawType;

//기본도형은 normal까지만 지원하낟. 그 이상의 정보가 필요하면
//버텍스 목록에 추가 가공을 다른방식으로 집어넣자
template<typename VertexType>
class DrawCmdData {
public:
	typedef std::vector<VertexType> VertexList;
	typedef std::vector<unsigned short> IndexList;
	
	DrawCmdData() : disable_cull_face(false) { }
	bool disable_cull_face;
	DrawType draw_mode;
	std::vector<VertexType> vertex_list;
	std::vector<unsigned short> index_list;
};

class SolidMeshFactory {
public:
	SolidMeshFactory() {}
	virtual ~SolidMeshFactory() {}

	virtual std::vector<DrawCmdData<Vertex_1P1N1UV>> CreateNormalMesh() = 0;
};

class SolidCubeFactory : public SolidMeshFactory {
public:
	SolidCubeFactory(float width, float height, float depth);
	virtual ~SolidCubeFactory() {}

	virtual std::vector<DrawCmdData<Vertex_1P1N1UV>> CreateNormalMesh();

private:
	float width_;
	float height_;
	float depth_;
};

class SolidSphereFactory : public SolidMeshFactory {
public:
	SolidSphereFactory(float radius, int slices, int stacks);
	virtual ~SolidSphereFactory() {}

	virtual std::vector<DrawCmdData<Vertex_1P1N1UV>> CreateNormalMesh();

private:
	float radius_;
	int slices_;
	int stacks_;
};

/*
class PrimitiveMeshDataFactory {
public:
	PrimitiveMeshDataFactory() {}
	virtual ~PrimitiveMeshDataFactory() {}

	virtual DrawCmdData<Vertex_1P> create() = 0;
};


class SolidCubeDataFactory : public PrimitiveMeshDataFactory {
public:
	SolidCubeDataFactory(float width, float height, float depth);
	virtual ~SolidCubeDataFactory() {}
private:
	float width_;
	float height_;
	float depth_;
};

class WireSphereDataFactory : public PrimitiveMeshDataFactory {
public:
	WireSphereDataFactory(float radius, int slices, int stacks);
	virtual ~WireSphereDataFactory() {}

private:
	float radius_;
	int slices_;
	int stacks_;
};



class AxisDataFactory : public PrimitiveMeshDataFactory {
public:
	AxisDataFactory(float size);
	virtual ~AxisDataFactory() {}
private:
	float size_;
};
*/
}