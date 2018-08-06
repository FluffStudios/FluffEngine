#include <gfx/shader.h>

#include <iostream>
#include <common.h>
#include <file/shader_utility.h>
#include <core/debug/debug_message.h>

#include <core/window.h>
#include <glew.h>

namespace fluff {
	namespace gfx {

		Shader::Shader(std::shared_ptr<ECSManager> & Manager, const char *Vertex, const char *Fragment)
			: Manager_(Manager)
		{
			Id_ = glCreateProgram();
			std::vector<unsigned int> stages;
			stages.push_back(CreateShaderStage(Vertex, GL_VERTEX_SHADER));
			stages.push_back(CreateShaderStage(Fragment, GL_FRAGMENT_SHADER));
			LinkProgram();
			VerifyProgram();
			DetachShaderStages(stages);
		}

		Shader::Shader(std::shared_ptr<ECSManager> & Manager, const char *Vertex, const char *Geometry, const char *Fragment)
			: Manager_(Manager)
		{
			Id_ = glCreateProgram();
			std::vector<unsigned int> stages;
			stages.push_back(CreateShaderStage(Vertex, GL_VERTEX_SHADER));
			stages.push_back(CreateShaderStage(Geometry, GL_GEOMETRY_SHADER));
			stages.push_back(CreateShaderStage(Fragment, GL_FRAGMENT_SHADER));
			LinkProgram();
			VerifyProgram();
			DetachShaderStages(stages);
		}

		Shader::Shader(std::shared_ptr<ECSManager> & Manager, const char *Vertex, const char *Geometry, const char *TesselationControl, const char *TesselationEvaluation, const char *Fragment)
			: Manager_(Manager)
		{
			Id_ = glCreateProgram();
			std::vector<unsigned int> stages;
			stages.push_back(CreateShaderStage(Vertex, GL_VERTEX_SHADER));
			stages.push_back(CreateShaderStage(Geometry, GL_GEOMETRY_SHADER));
			stages.push_back(CreateShaderStage(TesselationControl, GL_TESS_CONTROL_SHADER));
			stages.push_back(CreateShaderStage(TesselationEvaluation, GL_TESS_EVALUATION_SHADER));
			stages.push_back(CreateShaderStage(Fragment, GL_FRAGMENT_SHADER));
			LinkProgram();
			VerifyProgram();
			DetachShaderStages(stages);
		}

		Shader* Shader::Recreate(const char* Vertex, const char* Fragment)
		{
			Release();
			Shader(Manager_, Vertex, Fragment);
			return this;
		}

		Shader* Shader::Recreate(const char* Vertex, const char* Geometry, const char* Fragment)
		{
			Release();
			Shader(Manager_, Vertex, Geometry, Fragment);
			return this;
		}

		Shader* Shader::Recreate(const char* Vertex, const char* Geometry, const char* TesselationControl, const char* TesselationEvaluation, const char* Fragment)
		{
			Release();
			Shader(Manager_, Vertex, Geometry, TesselationControl, TesselationEvaluation, Fragment);
			return this;
		}

		void Shader::Bind() const
		{
			glUseProgram(Id_);
		}

		void Shader::Unbind()
		{
			glUseProgram(0);
		}

		void Shader::Release() 
		{
			if (Id_)
			{
				glDeleteProgram(Id_);
				Id_ = 0;
			}
		}

		void Shader::BindTextureLocation(const char * Name, const unsigned int Location) const
		{
			Bind();
			glUniform1i(glGetUniformLocation(Id_, Name), Location);
			Unbind();
		}

		unsigned int Shader::CreateShaderStage(const char *File, const GLuint Type) const
		{
			auto data = utilities::ShaderUtility::LoadShader(File);
			const auto shader_stage = glCreateShader(Type);
			auto chars = data.c_str();
			glShaderSource(shader_stage, 1, &chars, nullptr);
			glCompileShader(shader_stage);
			GLint v_compiled;
			glGetShaderiv(shader_stage, GL_COMPILE_STATUS, &v_compiled);
			if (v_compiled != GL_TRUE)
			{
				GLint len;
				glGetShaderiv(shader_stage, GL_INFO_LOG_LENGTH, &len);
				const auto info_log = static_cast<char*>(malloc(len));
				glGetShaderInfoLog(shader_stage, len, &len, info_log);
				FLUFF_LOG(debug::DebugErrorType::ILLEGAL_STATE, debug::DebugSeverity::FATAL, std::string(info_log));
				free(info_log);
			}
			glAttachShader(Id_, shader_stage);
			return shader_stage;
		}

		void Shader::LinkProgram() const
		{
			glLinkProgram(Id_);
			GLint v_linked;
			glGetProgramiv(Id_, GL_LINK_STATUS, &v_linked);
			if (v_linked != GL_TRUE)
			{
				GLint len;
				glGetProgramiv(Id_, GL_INFO_LOG_LENGTH, &len);
				const auto info_log = static_cast<char*>(malloc(len));
				glGetProgramInfoLog(Id_, len, &len, info_log);
				FLUFF_LOG(debug::DebugErrorType::ILLEGAL_STATE, debug::DebugSeverity::FATAL, std::string(info_log));
				free(info_log);
			}
		}

		void Shader::VerifyProgram() const
		{
			glValidateProgram(Id_);
			GLint v_validated;
			glGetProgramiv(Id_, GL_VALIDATE_STATUS, &v_validated);
			if (v_validated != GL_TRUE)
			{
				GLint len;
				glGetProgramiv(Id_, GL_INFO_LOG_LENGTH, &len);
				const auto info_log = static_cast<char*>(malloc(len));
				glGetProgramInfoLog(Id_, len, &len, info_log);
				FLUFF_LOG(debug::DebugErrorType::ILLEGAL_STATE, debug::DebugSeverity::FATAL, std::string(info_log));
				free(info_log);
			}
		}

		void Shader::DetachShaderStages(std::vector<unsigned int> Stages) const
		{
			while (!Stages.empty())
			{
				const auto shader = Stages.back();
				glDetachShader(Id_, shader);
				glDeleteShader(shader);
				Stages.pop_back();
			}
		}

	}
}