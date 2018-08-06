#pragma once

#include <gfx/mesh.h>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/gtx/hash.hpp>

#include <unordered_map>

namespace fluff { namespace sandbox {

	class Icosphere {
		struct IcosphereTriangle {
			uint32_t Index[3];
		};

		struct IcosphereMesh {
			std::vector<glm::vec3> Positions;
			std::vector<glm::vec3> Normals;
			std::vector<glm::vec3> UVs;
			std::vector<uint32_t>  Indices;
		};

		static const float X;
		static const float Z;
		static const float N;

		static const std::vector<glm::vec3> Positions;
		static const std::vector<IcosphereTriangle> Triangles;
	public:
		gfx::Mesh * GetMesh(uint16_t Subdivisions);
	private:
		uint32_t GetVertexForEdge(std::unordered_map<glm::ivec2, uint32_t> & EdgeMap, 
			std::vector<glm::vec3> & Vertices, uint32_t First, uint32_t Second);
		std::vector<IcosphereTriangle> Subdivide(std::vector<glm::vec3> & Vertices, std::vector<IcosphereTriangle> Triangles);
	};

	class UVSphere {

	};

} }