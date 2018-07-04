#include <gfx/shader_buffer.h>
#include <common.h>
#include <glew.h>

namespace fluff { namespace gfx {

		ShaderBuffer::ShaderBuffer(ShaderBufferDesc Descriptor)
		{
			GLint major;
			GLint minor;

			glGetIntegerv(GL_MAJOR_VERSION, &major);
			glGetIntegerv(GL_MINOR_VERSION, &minor);
			if (major >= 4 && minor >= 5)
			{
				glCreateBuffers(1, &Id_);
			}
			else
			{
				glGenBuffers(1, &Id_);
			}
			this->Descriptor_ = Descriptor;
			this->Set(nullptr);
		}

		void ShaderBuffer::Set(void *Data) const
		{
			Enable();
			if (Descriptor_.Usage == MAP_COHERENT)
			{
				glBufferStorage(GL_UNIFORM_BUFFER, Descriptor_.ByteWidth, Data, Descriptor_.ReadWrite | MAP_COHERENT | GL_MAP_PERSISTENT_BIT | GL_DYNAMIC_STORAGE_BIT);
			}
			else if (Descriptor_.Usage == MAP_PERSISTENT)
			{
				glBufferStorage(GL_UNIFORM_BUFFER, Descriptor_.ByteWidth, Data, Descriptor_.ReadWrite | MAP_PERSISTENT);
			}
			Disable();
		}

		void ShaderBuffer::SubData(void *Data, int Offset, int Size)
		{
			Enable();
			if (Descriptor_.Usage == MAP_COHERENT)
			{
				glBufferSubData(GL_UNIFORM_BUFFER, Offset, Size, Data);
			}
			else if (Descriptor_.Usage == MAP_PERSISTENT)
			{
				this->Data_ = Map(Offset, Size);
				memcpy(this->Data_, Data, Size);
				Unmap();
				Data_ = nullptr;
			}
			Disable();
		}

		void ShaderBuffer::Release()
		{
			glDeleteBuffers(1, &Id_);
			Id_ = 0;
		}

		void ShaderBuffer::Enable() const
		{
			glBindBuffer(GL_UNIFORM_BUFFER, Id_);
		}

		void ShaderBuffer::BindRange(int Index, int Begin, int Size) const
		{
			glBindBufferRange(GL_UNIFORM_BUFFER, Index, Id_, Begin, Size);
		}

		void ShaderBuffer::Disable()
		{
			glBindBuffer(GL_UNIFORM_BUFFER, 0);
		}

		void* ShaderBuffer::Map(int Offset, int Size) const
		{
#if defined(DEBUG) | defined(_DEBUG)
			if (Descriptor_.Usage != MAP_PERSISTENT)
			{
				FLUFF_ERROR_FUNC("Incorrect usage for function.")
				FLUFF_ASSERT(false)
			}
#endif
			Enable();
			return glMapBufferRange(GL_UNIFORM_BUFFER, Offset, Size, Descriptor_.ReadWrite | MAP_PERSISTENT);
		}

		void ShaderBuffer::Unmap()
		{
			glUnmapBuffer(GL_UNIFORM_BUFFER);
			Disable();
		}

	}
}
