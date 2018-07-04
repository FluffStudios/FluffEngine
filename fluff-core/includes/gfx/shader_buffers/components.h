#pragma once

#include <common.h>

namespace fluff { namespace gfx {

		struct alignas(sizeof(float32_t)) Float
		{
			float32_t Value;
		};

		struct alignas(sizeof(int32_t)) Int
		{
			int32_t Value;
		};

		struct alignas(sizeof(float32_t) * 2) Vec2
		{
			float32_t Value[2];
		};

		struct alignas(sizeof(float32_t) * 4) Vec3
		{
			float32_t Value[3];
		};

		struct alignas(sizeof(float32_t) * 4) Vec4
		{
			float32_t Value[4];
		};

		struct alignas(sizeof(int32_t) * 2) IVec2
		{
			int32_t Value[2];
		};

		struct alignas(sizeof(int32_t) * 4) IVec3
		{
			int32_t Value[3];
		};

		struct alignas(sizeof(int32_t) * 4) IVec4
		{
			int32_t Value[4];
		};

		struct alignas(sizeof(float32_t) * 2 * 2) Mat2x2
		{
			float32_t Value[2][2];
		};

		struct alignas(sizeof(float32_t) * 4 * 4) Mat3x3
		{
			float32_t Value[3][3];
		};

		struct alignas(sizeof(float32_t) * 4 * 4) Mat4x4
		{
			float32_t Value[4][4];
		};

} }