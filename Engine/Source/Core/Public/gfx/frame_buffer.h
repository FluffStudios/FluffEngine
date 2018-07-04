#pragma once

#define MAX_ATTACHMENTS 16

#include <vector>
#include <common.h>
#include <core/ecs/ecs_manager.h>

namespace fluff { namespace gfx {

	enum ACTION
	{
		DRAW = 0x8CA9,
		READ = 0x8CA8
	};

	class FLUFF_API FrameBuffer
	{
		struct Buffer
		{
			uint32_t * ID;
			uint32_t AttachmentLocation;
			uint64_t Usage;
		};

		struct Texture
		{
			uint32_t * ID;
			uint32_t AttachmentLocation;
			uint64_t Usage;
		};

		uint32_t Width_;
		uint32_t Height_;
		uint32_t Depth_;

		std::vector<Texture> AttachedTextures_;
		std::vector<Buffer> AttachedBuffers_;

		static uint32_t BoundDrawBuffer_;
		static uint32_t BoundReadBuffer_;
		static uint32_t BoundBuffer_;

		uint32_t Samples_ = 0;
		uint32_t ID_;
		uint32_t DepthBuffer_;
		uint32_t DepthTexture_;

		std::shared_ptr<ECSManager> Manager_;
	public:
		/*
			Creates a new 2D framebuffer

			Width - Width of framebuffer attachments
			Height - Height of framebuffer attachments
		*/
		FrameBuffer(std::shared_ptr<ECSManager> Manager, uint32_t Width, uint32_t Height);

		/*
			Creates a new 3D framebuffer

			Width - Width of framebuffer attachments
			Height - Height of framebuffer attachments
			Depth - Depth of framebuffer attachments
		*/
		FrameBuffer(std::shared_ptr<ECSManager> Manager, uint32_t Width, uint32_t Height, uint32_t Depth);

		~FrameBuffer() { Release(); }

		uint32_t Width() const { return Width_; }
		uint32_t Height() const { return Height_; }

		/*
			Releases the framebuffer and its attachments
		*/
		void Release();

		/*
			Binds the framebuffer
		*/
		void Bind() const;

		/*
			Binds the framebuffer for an action

			Action - Read or Write opreation
		*/
		void Bind(ACTION Action) const;

		/*
			Unbinds the framebuffer
		*/
		void Unbind() const;

		/*
			Unbinds the framebuffer and sets the dimensions of
			the viewport

			Width - width of viewport
			Height - height of viewport
		*/
		void Unbind(uint32_t Width, uint32_t Height) const;

		/*
			Resizes the attachments of the framebuffer

			PRECONDITION: Framebuffer bound

			Width - new width of attachments
			Height - new height of attachments
		*/
		void Resize(uint32_t Width, uint32_t Height);

		/*
			Adds color texture to framebuffer

			PRECONDITION: Framebuffer bound

			Returns color ID
		*/
		int32_t AddTexture();

		/*
			Adds depth texture to framebuffer

			PRECONDITION: Framebuffer bound

			Returns depth ID
		*/
		int32_t AddDepthTexture();

		/* 
			Adds pre-existing depth texture to framebuffer

			PRECONDITION: Framebuffer bound

			ID - ID of existing texture
			Returns ID
		*/
		int32_t AddDepthTexture(uint32_t ID);

		/*
			Adds color buffer to framebuffer

			PRECONDITION: Framebuffer bound

			Returns ID
		*/
		int32_t AddBuffer();

		/*
			Adds depth buffer to framebuffer

			PRECONDITION: Framebuffer is bound

			Returns ID
		*/
		int32_t AddDepthBuffer();

		/*
			Gets the ID of the framebuffer generated by
			OpenGL

			Returns framebuffer ID
		*/
		uint32_t GetID() const { return ID_; }

		/*
			Returns a vector of pointers to OpenGL IDs for the
			attached textures

			Returns texture IDs
		*/
		std::vector<uint32_t*> GetTextureIDs()
		{
			std::vector<uint32_t*> ids;
			for (Texture tex : AttachedTextures_) ids.push_back(tex.ID);
			return ids;
		}

		/*
			Gets a pointer to the ID of the depth buffer

			Returns pointer to ID
		*/
		uint32_t * GetDepthID() { return &DepthTexture_;  }

		/*
			Checks if the framebuffer is valid

			PRECONDITION: Framebuffer must be bound
			Returns if framebuffer is valid
		*/
		bool IsValid() const;

		/*
			Sets the buffers to draw to in the framebuffer

			PRECONDITION: Framebuffer must be bound
			
			IDs - Array of ids
			Count - Number of framebuffers
		*/
		void SetDrawBuffers(uint32_t * IDs, uint32_t Count) const;

		/*
			Copies the framebuffer to the default framebuffer

			Width - how wide the screen is
			Height - how tall the screen is
		*/
		void DrawToScreen(uint32_t Width, uint32_t Height) const;
	};

} }