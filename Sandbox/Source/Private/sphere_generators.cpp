#include <sphere_generators.h>

#include <array>
#include <glm/glm.hpp>

namespace fluff { namespace sandbox {

	const float Icosphere::X = 0.525731112119133606f;
	const float Icosphere::Z = 0.850650808352039932f;
	const float Icosphere::N = 0.0f;

	const std::vector<glm::vec3> Icosphere::Positions = 
	{
		{ -X,  N,  Z },
		{  X,  N,  Z },
		{ -X,  N, -Z },
		{  X,  N, -Z },
		{  N,  Z,  X },
		{  N,  Z, -X },
		{  N, -Z,  X },
		{  N, -Z, -X },
		{  Z,  X,  N },
		{ -Z,  X,  N },
		{  Z, -X,  N },
		{ -Z, -X,  N }
	};

	const std::vector<Icosphere::IcosphereTriangle> Icosphere::Triangles =
	{
		{ 0,4,1 },
		{ 0,9,4 },
		{ 9,5,4 },
		{ 4,5,8 },
		{ 4,8,1 },
		{ 8,10,1 },
		{ 8,3,10 },
		{ 5,3,8 },
		{ 5,2,3 },
		{ 2,7,3 },
		{ 7,10,3 },
		{ 7,6,10 },
		{ 7,11,6 },
		{ 11,0,6 },
		{ 0,1,6 },
		{ 6,1,10 },
		{ 9,0,11 },
		{ 9,11,2 },
		{ 9,2,5 },
		{ 7,2,11 }
	};

	gfx::Mesh * Icosphere::GetMesh(uint16_t Subdivisions)
	{
		std::vector<IcosphereTriangle> triangles = Icosphere::Triangles;
		std::vector<glm::vec3> vertices = Icosphere::Positions;

		for (uint16_t i = 0; i < Subdivisions; i++) {
			triangles = Subdivide(vertices, triangles);
		}

		std::vector<float32_t> positions;
		std::vector<uint32_t> indices;

		for (auto & vertex : vertices) {
			positions.push_back(vertex.x);
			positions.push_back(vertex.y);
			positions.push_back(vertex.z);
		}

		for (auto & tri : triangles) {
			indices.push_back(tri.Index[0]);
			indices.push_back(tri.Index[1]);
			indices.push_back(tri.Index[2]);
		}

		return new gfx::Mesh(positions, positions, positions, indices);
	}

	uint32_t Icosphere::GetVertexForEdge(std::unordered_map<glm::ivec2, uint32_t>& EdgeMap, std::vector<glm::vec3>& Vertices, uint32_t First, uint32_t Second)
	{
		glm::ivec2 key;
		if (First > Second) {
			key = glm::ivec2(Second, First);
		}
		else {
			key = glm::ivec2(First, Second);
		}

		auto contained = EdgeMap.insert({
			key, Vertices.size()
		});

		if (contained.second) {
			auto & edge0 = Vertices[First];
			auto & edge1 = Vertices[Second];
			auto point = glm::normalize(edge0 + edge1);
			Vertices.push_back(point);
		}
		return contained.first->second;
	}

	std::vector<Icosphere::IcosphereTriangle> Icosphere::Subdivide(std::vector<glm::vec3> & Vertices, std::vector<IcosphereTriangle> Triangles)
	{
		std::unordered_map<glm::ivec2, uint32_t> EdgeMap;
		std::vector<IcosphereTriangle> results;

		for (auto && tri : Triangles) {
			std::array<uint32_t, 3> mid;
			for (uint8_t edge = 0; edge < 3; ++edge) {
				mid[edge] = GetVertexForEdge(EdgeMap, Vertices, tri.Index[edge], tri.Index[(edge + 1) % 3]);
			}

			results.push_back({ tri.Index[0], mid[0], mid[2] });
			results.push_back({ tri.Index[1], mid[1], mid[0] });
			results.push_back({ tri.Index[2], mid[2], mid[1] });
			results.push_back({ mid[0], mid[1], mid[2] });
		}
		return results;
	}

} }