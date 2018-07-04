#pragma once

#include <physics/physics_manager.h>

namespace fluff {
	namespace physics {

		enum GeometryType
		{
			SPHERE,
			BOX,
			PLANE,
			CAPSULE,
			HEIGHTFIELD
		};

		struct GeometryDesc
		{

		};

		struct SphereDesc : public GeometryDesc
		{
			float radius;
		};

		struct BoxDesc : public GeometryDesc
		{
			float HalfWidth;
			float HalfHeight;
			float HalfDepth;
		};

		struct HeightFieldDesc : public GeometryDesc
		{
			float * HeightMap;
			uint32_t NumRows;
			uint32_t NumCols;
			
			float32_t HeightScale;
			float32_t RowScale;
			float32_t ColumnScale;

			PhysicsManager * pManager;
		};

		// ONLY USE ON STATIC ACTORS
		struct PlaneDesc : public GeometryDesc
		{

		};

		struct CapsuleDesc : public GeometryDesc
		{
			float Radius;
			float HalfHeight;
		};

} }