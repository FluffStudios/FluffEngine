#include <gfx/context.h>
#include <core/allocators/allocator.h>
#include <gfx/model_submit_event.h>
#include <core/core.h>
#include <glew.h>

namespace fluff { namespace gfx {

	Context Context::Singleton_;
	std::string Context::Renderer;
	std::string Context::Vendor;
	bool Context::Init_;

	void Context::WriteToColorBuffer(bool ShouldWrite)
	{
		glColorMask(ShouldWrite, ShouldWrite, ShouldWrite, ShouldWrite);
	}

	void Context::WriteToDepthBuffer(bool ShouldWrite)
	{
		glDepthMask(ShouldWrite);
	}

	void Context::WriteToStencilBuffer(bool ShouldWrite)
	{
		glStencilMask(ShouldWrite);
	}

	const std::string & Context::GetGraphicsCard()
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

	const std::string & Context::GetVendor()
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

	Model* Context::LoadMesh(std::unordered_map<VertexLayout, std::vector<float>> Data, std::vector<unsigned int> Indices)
	{
		const auto model = new Model;
		VertexBuffer buffer;
		buffer.Bind();
		for (auto& it : Data)
		{
			buffer.LoadToBuffer(it.second.data(), it.second.size(), it.first);
		}
		buffer.LoadIndices(Indices.data(), Indices.size());
		buffer.Unbind();
		model->IndexCount = static_cast<size_t>(Indices.size());
		model->IndexOffset = 0;
		model->VertexOffset = 0;
		return model;
	}

	Model* Context::LoadMesh(Mesh *pMesh)
	{
		const auto model = new Model;
		model->IndexCount = pMesh->GetIndexCount();
		model->IndexOffset = Singleton_.IndexOffset_;
		model->VertexOffset = Singleton_.VertexOffset_;
		Singleton_.IndexOffset_ += pMesh->GetIndexCount() * sizeof(unsigned int);
		Singleton_.VertexOffset_ += pMesh->GetVertexCount();
		Singleton_.Models_.push_back(model);
		Singleton_.Meshes_.push_back(pMesh);
		return model;
	}

	VertexBuffer* Context::Flush(std::shared_ptr<ECSManager>& Manager)
	{
		if (Singleton_.Meshes_.size() == 0) return nullptr;

		size_t vbo_size = 0;
		size_t index_size = 0;

		for (auto * mesh : Singleton_.Meshes_)
		{
			vbo_size += (mesh->VertexCount * 3);
			index_size += mesh->IndexCount;
		}

		const auto positions = static_cast<float*>(Allocator::Alloc(vbo_size * sizeof(float)));
		const auto uvs = static_cast<float*>(Allocator::Alloc(vbo_size * sizeof(float)));
		const auto normals = static_cast<float*>(Allocator::Alloc(vbo_size * sizeof(float)));
		const auto tangents = static_cast<float*>(Allocator::Alloc(vbo_size * sizeof(float)));
		const auto bitangents = static_cast<float*>(Allocator::Alloc(vbo_size * sizeof(float)));
		const auto indices = static_cast<unsigned int*>(Allocator::Alloc(index_size * sizeof(unsigned int)));

		size_t vertex_offset = 0;
		size_t index_offset = 0;

		for (const auto mesh : Singleton_.Meshes_)
		{
			memcpy(&positions[vertex_offset], mesh->GetPositionVector(), sizeof(float) * 3 * mesh->VertexCount);
			memcpy(&uvs[vertex_offset], mesh->GetUVCoordinates(), sizeof(float) * 3 * mesh->VertexCount);
			memcpy(&normals[vertex_offset], mesh->GetNormalVectors(), sizeof(float) * 3 * mesh->VertexCount); 
			if (mesh->GetTangentVectors()) memcpy(&tangents[vertex_offset], mesh->GetTangentVectors(), sizeof(float) * 3 * mesh->VertexCount);
			if (mesh->GetBitangentVectors()) memcpy(&bitangents[vertex_offset], mesh->GetBitangentVectors(), sizeof(float) * 3 * mesh->VertexCount);
			memcpy(&indices[index_offset], mesh->GetIndexBuffer(), sizeof(unsigned int) * mesh->IndexCount);

			vertex_offset += (3 * mesh->VertexCount);
			index_offset += mesh->IndexCount;

			delete mesh;
		}
		
		Singleton_.Meshes_.clear();
		auto buffer = new VertexBuffer;
		buffer->Bind();

		const VertexLayout pos = {
			0, 3, 0, 0
		};
		buffer->LoadToBuffer(positions, vbo_size, pos);
		const VertexLayout uv = {
			1, 3, 0, 0
		};
		buffer->LoadToBuffer(uvs, vbo_size, uv);
		const VertexLayout normal = {
			2, 3, 0, 0
		};
		buffer->LoadToBuffer(normals, vbo_size, normal);
		const VertexLayout tangent = {
			3, 3, 0, 0
		};
		buffer->LoadToBuffer(tangents, vbo_size, tangent);
		const VertexLayout bitangent = {
			4, 3, 0, 0
		};
		buffer->LoadToBuffer(bitangents, vbo_size, bitangent);

		buffer->LoadIndices(indices, index_size);
		buffer->Unbind();

		for (auto model : Singleton_.Models_)
		{
			model->Buffer = buffer;
		}

		Singleton_.IndexOffset_ = 0;
		Singleton_.VertexOffset_ = 0;
		Singleton_.Models_.clear();
		
		Allocator::Free(positions);
		Allocator::Free(uvs);
		Allocator::Free(normals);
		Allocator::Free(tangents);
		Allocator::Free(bitangents);
		Allocator::Free(indices);

		Manager->GetEventManager()->EmitEvent<ModelSubmittedEvent>();

		return buffer;
	}

	void Context::BindTextureSlots(Texture2D **ppTexture, int StartSlot, int NumViews)
	{
		BindTextures(reinterpret_cast<Texture**>(ppTexture), StartSlot, NumViews);
	}

	void Context::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void Context::UpdateSubResource(ShaderBuffer *BufferHandle, void* Data, int Begin, int Size)
	{
		BufferHandle->SubData(Data, Begin, Size);
	}

	void* Context::MapSubResource(ShaderBuffer* BufferHandle, int Offset, int Size)
	{
		return BufferHandle->Map(Offset, Size);
	}

	void Context::UnmapSubResource(ShaderBuffer* BufferHandle)
	{
		BufferHandle->Unmap();
	}

	void Context::BindRange(ShaderBuffer* BufferHandle, int Begin, int Size, int Index)
	{
		BufferHandle->BindRange(Index, Begin, Size);
	}

} }
