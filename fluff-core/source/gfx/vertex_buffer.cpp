#include <gfx/vertex_buffer.h>
#include <gfx/mesh.h>
#include "gfx/vertex_layout.h"

namespace luminos { namespace gfx {

	VertexBuffer::VertexBuffer()
	{
		GLint major;
		GLint minor;

		glGetIntegerv(GL_MAJOR_VERSION, &major);
		glGetIntegerv(GL_MINOR_VERSION, &minor);
		if (major >= 4 && minor >= 5)
		{
			glCreateVertexArrays(1, &Vao_);
		}
		else
		{
			glGenVertexArrays(1, &Vao_);
		}
	}

	void VertexBuffer::Bind()
	{
		glBindVertexArray(Vao_);
		for (auto attr : Attribs_) 
		{
			glEnableVertexAttribArray(attr);
		}
	}

	void VertexBuffer::Unbind()
	{
		for (auto attr : Attribs_)
		{
			glDisableVertexAttribArray(attr);
		}
		glBindVertexArray(0);
	}

	void VertexBuffer::Release()
	{
		if (Vao_)
		{
			glDeleteBuffers(static_cast<GLsizei>(Vbos_.size()), Vbos_.data());
			glDeleteVertexArrays(1, &Vao_);
			Vao_ = 0;
		}
	}

	void VertexBuffer::LoadToBuffer(float* Data, size_t Count, VertexLayout Layout)
	{
		if (Count > 0)
		{
			GLuint vbo;
			GLint major;
			GLint minor;

			glGetIntegerv(GL_MAJOR_VERSION, &major);
			glGetIntegerv(GL_MINOR_VERSION, &minor);
			if (major >= 4 && minor >= 5)
			{
				glCreateBuffers(1, &vbo);
			}
			else
			{
				glGenBuffers(1, &vbo);
			}

			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glBufferStorage(GL_ARRAY_BUFFER, Count * sizeof(float), Data, 0);

			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glVertexAttribPointer(Layout.Attribute, Layout.Dimensions, GL_FLOAT, GL_FALSE, static_cast<GLsizei>(Layout.Stride), reinterpret_cast<const void*>(Layout.Offset));
						
			Vbos_.push_back(vbo);
			Attribs_.insert(static_cast<const unsigned int>(Layout.Attribute));
		}
	}

	void VertexBuffer::LoadToBuffer(unsigned int* Data, size_t Count, VertexLayout Layout)
	{
		GLuint vbo;
		GLint major;
		GLint minor;

		glGetIntegerv(GL_MAJOR_VERSION, &major);
		glGetIntegerv(GL_MINOR_VERSION, &minor);
		if (major >= 4 && minor >= 5)
		{
			glCreateBuffers(1, &vbo);
		}
		else
		{
			glGenBuffers(1, &vbo);
		}
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferStorage(GL_ARRAY_BUFFER, Count * sizeof(float), Data, 0);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glVertexAttribPointer(Layout.Attribute, Layout.Dimensions, GL_UNSIGNED_INT, GL_FALSE, static_cast<GLsizei>(Layout.Stride), reinterpret_cast<const void*>(Layout.Offset));
		Vbos_.push_back(vbo);
		Attribs_.insert(static_cast<const unsigned int>(Layout.Attribute));
	}

	void VertexBuffer::LoadToBuffer(int* Data, size_t Count, VertexLayout Layout)
	{
		GLuint vbo;
		GLint major;
		GLint minor;

		glGetIntegerv(GL_MAJOR_VERSION, &major);
		glGetIntegerv(GL_MINOR_VERSION, &minor);
		if (major >= 4 && minor >= 5)
		{
			glCreateBuffers(1, &vbo);
		}
		else
		{
			glGenBuffers(1, &vbo);
		}
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferStorage(GL_ARRAY_BUFFER, Count * sizeof(float), Data, 0);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glVertexAttribPointer(Layout.Attribute, Layout.Dimensions, GL_INT, GL_FALSE, static_cast<GLsizei>(Layout.Stride), reinterpret_cast<const void*>(Layout.Offset));
		Vbos_.push_back(vbo);
		Attribs_.insert(static_cast<const unsigned int>(Layout.Attribute));
	}

	void VertexBuffer::LoadToBuffer(std::vector<float> Positions, std::vector<float> VertexData)
	{
		GLuint pos;
		GLint major;
		GLint minor;

		glGetIntegerv(GL_MAJOR_VERSION, &major);
		glGetIntegerv(GL_MINOR_VERSION, &minor);
		if (major >= 4 && minor >= 5)
		{
			glCreateBuffers(1, &pos);
		}
		else
		{
			glGenBuffers(1, &pos);
		}
		glBindBuffer(GL_ARRAY_BUFFER, pos);
		glBufferStorage(GL_ARRAY_BUFFER, Positions.size() * sizeof(float), Positions.data(), 0);
		glBindBuffer(GL_ARRAY_BUFFER, pos);
		glVertexAttribPointer(POSITION_ATTRIB, 3, GL_FLOAT, GL_FALSE, 0, 0);
		Vbos_.push_back(pos);
		Attribs_.insert(POSITION_ATTRIB);
		GLuint buf;
		if (major >= 4 && minor >= 5)
		{
			glCreateBuffers(1, &buf);
		}
		else
		{
			glGenBuffers(1, &buf);
		}
		glBindBuffer(GL_ARRAY_BUFFER, buf);
		glBufferStorage(GL_ARRAY_BUFFER, VertexData.size() * sizeof(float), VertexData.data(), 0);
		glBindBuffer(GL_ARRAY_BUFFER, buf);
		glVertexAttribPointer(UV_ATTRIB, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex) - 3 * sizeof(float), reinterpret_cast<const void*>(offsetof(Vertex, Vertex::Position) - offsetof(Vertex, Vertex::Position)));
		glVertexAttribPointer(NORMAL_ATTRIB, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex) - 3 * sizeof(float), reinterpret_cast<const void*>(offsetof(Vertex, Vertex::TextureCoordinates) - offsetof(Vertex, Vertex::Position)));
		glVertexAttribPointer(TANGENT_ATTRIB, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex) - 3 * sizeof(float), reinterpret_cast<const void*>(offsetof(Vertex, Vertex::Normal) - offsetof(Vertex, Vertex::Position)));
		glVertexAttribPointer(BITANGENT_ATTRIB, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex) - 3 * sizeof(float), reinterpret_cast<const void*>(offsetof(Vertex, Vertex::Tangent) - offsetof(Vertex, Vertex::Position)));
		glVertexAttribPointer(DIFFUSE_ATTRIB, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex) - 3 * sizeof(float), reinterpret_cast<const void*>(offsetof(Vertex, Vertex::BiTangent) - offsetof(Vertex, Vertex::Position)));
		glVertexAttribPointer(SPECULAR_ATTRIB, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex) - 3 * sizeof(float), reinterpret_cast<const void*>(offsetof(Vertex, Vertex::Diffuse) - offsetof(Vertex, Vertex::Position)));
		Vbos_.push_back(buf);
		Attribs_.insert(static_cast<const unsigned int>(UV_ATTRIB));
		Attribs_.insert(static_cast<const unsigned int>(NORMAL_ATTRIB));
		Attribs_.insert(static_cast<const unsigned int>(TANGENT_ATTRIB));
		Attribs_.insert(static_cast<const unsigned int>(BITANGENT_ATTRIB));
		Attribs_.insert(static_cast<const unsigned int>(DIFFUSE_ATTRIB));
		Attribs_.insert(static_cast<const unsigned int>(SPECULAR_ATTRIB));
	}

	void VertexBuffer::LoadIndices(unsigned int* Data, size_t Count)
	{
		GLuint index;
		GLint major;
		GLint minor;

		glGetIntegerv(GL_MAJOR_VERSION, &major);
		glGetIntegerv(GL_MINOR_VERSION, &minor);
		if (major >= 4 && minor >= 5)
		{
			glCreateBuffers(1, &index);
		}
		else
		{
			glGenBuffers(1, &index);
		}
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index);
		glBufferStorage(GL_ELEMENT_ARRAY_BUFFER, Count * sizeof(unsigned int), Data, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index);
		Vbos_.push_back(index);
	}

} }
