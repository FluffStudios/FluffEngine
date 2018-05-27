#pragma once

#include <common.h>
#include <gfx/mesh.h>
#include <gfx/model.h>
#include <gfx/texture.h>
#include <gfx/shader.h>
#include <gfx/frame_buffer.h>
#include <core/window.h>
#include <gfx/vertex_layout.h>
#include <core/ecs/ecs_manager.h>

#include <set>
#include <string>
#include <unordered_map>

namespace luminos { namespace gfx {

		class LUMINOS_API Context
		{
			std::vector<Mesh*> Meshes_;
			std::vector<Model*> Models_;
			size_t IndexOffset_ = 0;
			size_t VertexOffset_ = 0;
			Context() {
				
			};

			static std::string Renderer;
			static std::string Vendor;

			static Context Singleton_;
			static bool Init_;

		public:

			/*
				Write to color buffer of bound frame buffer

				ShouldWrite - should write to color buffer
			*/
			static void WriteToColorBuffer(bool ShouldWrite);

			/*
				Write to depth buffer of bound frame buffer

				ShouldWrite - should write to depth buffer
			*/
			static void WriteToDepthBuffer(bool ShouldWrite);

			/*
				Write to stencil buffer of bound frame buffer

				ShouldWrite - should write to stencil buffer
			*/
			static void WriteToStencilBuffer(bool ShouldWrite);

			/*
				Gets a string representing the OpenGL rendering device

				Returns rendering device
			*/
			static const std::string& GetGraphicsCard()
			{
				if (!Init_)
				{
					unsigned char* renderer = const_cast<unsigned char*>(glGetString(GL_RENDERER));
					Renderer = reinterpret_cast<char*>(renderer);

					unsigned char* vendor = const_cast<unsigned char*>(glGetString(GL_VENDOR));
					Vendor = reinterpret_cast<char*>(vendor);
					Init_ = true;
				}
				return Renderer;
			}

			/*
				Gets a string representing the OpenGL vendor

				Returns device vendor
			*/
			static const std::string& GetVendor()
			{
				if (!Init_)
				{
					unsigned char* renderer = const_cast<unsigned char*>(glGetString(GL_RENDERER));
					Renderer = reinterpret_cast<char*>(renderer);

					unsigned char* vendor = const_cast<unsigned char*>(glGetString(GL_VENDOR));
					Vendor = reinterpret_cast<char*>(vendor);
					Init_ = true;
				}
				return Vendor;
			}

			/*
				Loads unorganized data to vertex buffer
				
				Data - Vertex data to include in vertex buffer
				Indices - Array of indices for rendering
				Returns a mesh loaded to a model
			 */
			static Model* LoadMesh(std::unordered_map<VertexLayout, std::vector<float>> Data, std::vector<unsigned int> Indices);

			/*
				Creates a new model from loaded meshes
				
				MeshHandle - Loads mesh to GPU
				Returns a mesh loaded to a model
			 */
			static Model* LoadMesh(Mesh *MeshHandle);

			/*
				Flushes all loaded models to the GPU
				
				Returns a VertexBuffer of loaded models
			 */
			static VertexBuffer* Flush(std::shared_ptr<ECSManager>& Manager);

			/*
				Binds textures to slots
				
				Textures - pointer to texture array
				StartSlot - Start slot for binding
				NumViews - Number of slots to bind
			 */
			static void BindTextureSlots(Texture2D **Textures, int StartSlot, int NumViews);

			/*
				Clears bound buffer
			 */
			static void Clear();

			/*
				Updates buffer sub resources
				
				BufferHandle - pointer to shader buffer
				Data - pointer to data
				Begin - Beginning index
				Size - Size of data buffer
			 */
			static void UpdateSubResource(ShaderBuffer *BufferHandle, void *Data, int Begin, int Size);

			/*
				Creates buffer to map resources to
				
				BufferHandle - pointer to shader buffer
				Offset - Offset in shader buffer
				Size - Size of buffer
				Returns a resource data buffer
			 */
			static void* MapSubResource(ShaderBuffer *BufferHandle, int Offset, int Size);

			/*
				Unmaps the buffer
				
				BufferHandle - pointer to shader buffer
			 */
			static void UnmapSubResource(ShaderBuffer *BufferHandle);

			/*
				Binds a range in the buffer
				
				BufferHandle - pointer to shader buffer
				Begin - Offset in shader buffer
				Size - Size of buffer
				Index - Index of buffer
			 */
			static void BindRange(ShaderBuffer *BufferHandle, int Begin, int Size, int Index);
		};

	}
}
