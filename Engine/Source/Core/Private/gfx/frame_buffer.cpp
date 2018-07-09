#include <gfx/frame_buffer.h>
#include <core/debug/debug_message.h>
#include <common.h>
#include <glew.h>

namespace fluff { namespace gfx {

	uint32_t FrameBuffer::BoundDrawBuffer_ = 0;
	uint32_t FrameBuffer::BoundReadBuffer_ = 0;
	uint32_t FrameBuffer::BoundBuffer_ = 0;

	FrameBuffer::FrameBuffer(std::shared_ptr<ECSManager> Manager, uint32_t Width, uint32_t Height)
		: Manager_(Manager), Width_(Width), Height_(Height)
	{
		glGenFramebuffers(1, &ID_);
		if (!ID_)
		{
			debug::DebugMessage(Manager_, debug::DebugErrorType::ILLEGAL_STATE, debug::DebugSeverity::FATAL, static_cast<size_t>(__LINE__), std::string(__FILE__), "Could not create frame buffer.");
		}
	}

	FrameBuffer::FrameBuffer(std::shared_ptr<ECSManager> Manager, uint32_t Width, uint32_t Height, uint32_t Depth)
		: FrameBuffer(Manager, Width, Height)
	{
		this->Depth_ = Depth;
	}

	void FrameBuffer::Release()
	{
		for (Texture tex : AttachedTextures_)
		{
			glDeleteTextures(1, tex.ID);
			delete tex.ID;
		}
		for (Buffer buf : AttachedBuffers_)
		{
			glDeleteRenderbuffers(1, buf.ID);
			delete buf.ID;
		}

		AttachedBuffers_.clear();
		AttachedTextures_.clear();
	}

	void FrameBuffer::Bind() const
	{
		if (BoundBuffer_ != ID_)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, ID_);
			BoundBuffer_ = ID_;
		}
	}

	void FrameBuffer::Bind(ACTION Action) const
	{
		static bool bind = false;
		switch (Action)
		{
		case DRAW:
		{
			if (BoundDrawBuffer_ != ID_)
			{
				BoundDrawBuffer_ = ID_;
				bind = true;
			}
			break;
		}
		case READ:
		{
			if (BoundReadBuffer_ != ID_)
			{
				BoundReadBuffer_ = ID_;
				bind = true;
			}
			break;
		}
		}
		if (bind) glBindFramebuffer(Action, ID_);
	}

	void FrameBuffer::Unbind() const
	{
		if (BoundBuffer_ != 0)
		{
			BoundBuffer_ = 0;
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}
		if (BoundReadBuffer_ != 0)
		{
			BoundReadBuffer_ = 0;
			glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
		}
		if (BoundDrawBuffer_ != 0)
		{
			BoundDrawBuffer_ = 0;
			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		}
	}

	void FrameBuffer::Unbind(uint32_t Width, uint32_t Height) const
	{
		this->Unbind();
		glViewport(0, 0, Width, Height);
	}

	void FrameBuffer::Resize(uint32_t Width, uint32_t Height)
	{
		Width_ = Width;
		Height_ = Height;
		for (auto & tex : AttachedTextures_)
		{
			glDeleteTextures(1, tex.ID);
			glGenTextures(1, tex.ID);
			glBindTexture(GL_TEXTURE_2D, *(tex.ID));
			glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA16F, Width_, Height_);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glFramebufferTexture2D(GL_FRAMEBUFFER, tex.AttachmentLocation, GL_TEXTURE_2D, *tex.ID, 0);
		}

		if (DepthTexture_)
		{
			glDeleteTextures(1, &DepthTexture_);
			uint32_t id;
			glGenTextures(1, &id);
			glBindTexture(GL_TEXTURE_2D, id);
			glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH_COMPONENT24, Width_, Height_);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, id, 0);
			this->DepthTexture_ = id;
		}

		for (auto & tex : AttachedBuffers_)
		{
			glDeleteRenderbuffers(1, tex.ID);
			glGenRenderbuffers(1, tex.ID);
			glBindRenderbuffer(GL_RENDERBUFFER, *tex.ID);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA8, Width_, Height_);
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, tex.AttachmentLocation, GL_RENDERBUFFER, *tex.ID);
		}

		if (DepthBuffer_)
		{
			glDeleteRenderbuffers(1, &DepthBuffer_);
			uint32_t id;
			glGenRenderbuffers(1, &id);
			glBindRenderbuffer(GL_RENDERBUFFER, id);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, Width_, Height_);
			glFramebufferRenderbuffer(GL_RENDERBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, id);
			this->DepthBuffer_ = id;
		}
	}

	int32_t FrameBuffer::AddTexture()
	{
		uint32_t id;
		glGenTextures(1, &id);
		glBindTexture(GL_TEXTURE_2D, id);
		glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA16F, Width_, Height_);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, static_cast<GLenum>(GL_COLOR_ATTACHMENT0 + this->AttachedTextures_.size() + this->AttachedBuffers_.size()), GL_TEXTURE_2D, id, 0);
		size_t loc = this->AttachedTextures_.size() + this->AttachedBuffers_.size();
		Texture tex = {
			nullptr,
			static_cast<GLenum>(GL_COLOR_ATTACHMENT0 + this->AttachedTextures_.size() + this->AttachedBuffers_.size()),
			GL_TEXTURE_2D
		};
		tex.ID = new uint32_t;
		*(tex.ID) = id;
		this->AttachedTextures_.push_back(std::move(tex));
		return static_cast<int32_t>(loc);
	}

	int32_t FrameBuffer::AddDepthTexture()
	{
		uint32_t id;
		glGenTextures(1, &id);
		glBindTexture(GL_TEXTURE_2D, id);
		glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH_COMPONENT24, Width_, Height_);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, id, 0);
		this->DepthTexture_ = id;
		return id;
	}

	int32_t FrameBuffer::AddDepthTexture(uint32_t ID)
	{
		glBindTexture(GL_TEXTURE_2D, ID);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, ID, 0);
		this->DepthTexture_ = ID;
		return ID;
	}

	int32_t FrameBuffer::AddBuffer()
	{
		uint32_t id;
		glGenRenderbuffers(1, &id);
		glBindRenderbuffer(GL_RENDERBUFFER, id);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA8, Width_, Height_);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, static_cast<GLenum>(GL_COLOR_ATTACHMENT0 + this->AttachedTextures_.size() + this->AttachedBuffers_.size()), GL_RENDERBUFFER, id);
		size_t loc = this->AttachedTextures_.size() + this->AttachedBuffers_.size();
		Buffer buf = {
			nullptr,
			static_cast<GLenum>(GL_COLOR_ATTACHMENT0 + this->AttachedTextures_.size() + this->AttachedBuffers_.size()),
			GL_TEXTURE_2D
		};
		buf.ID = new uint32_t;
		*(buf.ID) = id;
		this->AttachedBuffers_.push_back(buf);
		return static_cast<int32_t>(loc);
	}

	int32_t FrameBuffer::AddDepthBuffer()
	{
		uint32_t id;
		glGenRenderbuffers(1, &id);
		glBindRenderbuffer(GL_RENDERBUFFER, id);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, Width_, Height_);
		glFramebufferRenderbuffer(GL_RENDERBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, id);
		this->DepthBuffer_ = id;
		return id;
	}

	bool FrameBuffer::IsValid() const
	{
		return glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
	}

	void FrameBuffer::SetDrawBuffers(uint32_t * IDs, uint32_t Count) const
	{
		uint32_t Bufs[32];
		for (size_t i = 0; i < Count; i++) Bufs[i] = GL_COLOR_ATTACHMENT0 + IDs[i];
		glDrawBuffers(Count, Bufs);
	}

	void FrameBuffer::DrawToScreen(uint32_t Width, uint32_t Height) const
	{
		this->Bind(READ);
		glReadBuffer(GL_COLOR_ATTACHMENT0);
		if (BoundDrawBuffer_ != 0)
		{
			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
			BoundDrawBuffer_ = 0;
		}
		glBlitFramebuffer(0, 0, Width_, Height_, 0, 0, Width, Height, GL_COLOR_BUFFER_BIT, GL_LINEAR);
		this->Unbind(Width, Height);
	}

} }
