#pragma once

#include <common.h>

namespace fluff { namespace gfx {

		struct Float
		{
			float32_t Value;
		};

		struct Int
		{
			int32_t Value;
		};

		struct Vec2
		{
			float32_t Value[2];
		};

		struct Vec3
		{
			float32_t Value[3];
			float32_t buffer;
		};

		struct Vec4
		{
			float32_t Value[4];
		};

		struct IVec2
		{
			int32_t Value[2];
		};

		struct IVec3
		{
			int32_t Value[3];
			int32_t buffer;
		};

		struct IVec4
		{
			int32_t Value[4];
		};

		struct Mat2x2
		{
			float32_t Value[2][2];
		};

		struct Mat3x3
		{
			float32_t Value[3][3];
			float buffer[7]; // align to mat4x4
		};

		struct Mat4x4
		{
			float32_t Value[4][4];
		};

} }