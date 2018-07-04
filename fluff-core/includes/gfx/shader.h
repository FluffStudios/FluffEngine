#pragma once

#include <vector>
#include <core/ecs/ecs_manager.h>

#include "shader_buffer.h"

#include <sstream>
#include <common.h>
#include <fstream>

#define MAX_UNIFORM_COUNT 128

namespace fluff { namespace gfx {

	/**
	 * Represents a shader program.
	 * 
	 * @author Nick Clark
	 * @version 18-October-17
	 */
	class Shader
	{
		uint32_t Id_ = 0;
		std::shared_ptr<ECSManager> & Manager_;
	public:
		/*
			Creates a new shader program
			
			Vertex - Vertex shader file
			Fragment - Fragment shader file
		 */
		FLUFF_API Shader(std::shared_ptr<ECSManager> & Manager, const char *Vertex, const char *Fragment);

		/*
			Creates a new shader program
			
			Vertex - Vertex shader file
			Geometry - Geometry shader file
			Fragment - Fragment shader file
		 */
		FLUFF_API Shader(std::shared_ptr<ECSManager> & Manager, const char *Vertex, const char *Geometry, const char *Fragment);
		
		/*
			Creates a new shader program
			
			Vertex - Vertex shader file
			Geometry - Geometry shader file
			TesselationControl - Tesselation control shader file
			TesselationEvaluation - Tesselation evaluation shader file
			Fragment - Fragment shader file
		 */
		FLUFF_API Shader(std::shared_ptr<ECSManager> & Manager, const char *Vertex, const char *Geometry, const char *TesselationControl, const char *TesselationEvaluation, const char *Fragment);

		FLUFF_API ~Shader() {
			Release(); 
		}
		
		/*
			Checks if two shaders are the same GPU program

			Other - other shader
			Returns if same shader
		*/
		bool FLUFF_API operator == (const Shader & Other) const { return Id_ == Other.Id_; }

		/*
			Checks if two shaders are not the same GPU program

			Other - other shader
			Returns if not the same shader
		*/
		bool FLUFF_API operator != (const Shader & Other) const { return Id_ != Other.Id_; }

		/*
			Recreates shader program
			
			Vertex - Vertex shader file
			Fragment - Fragment shader file
			Returns pointer to this shader
		 */
		Shader FLUFF_API * Recreate(const char * Vertex, const char * Fragment);
		
		/*
			Recreates shader program
			
			Vertex - Vertex shader file
			Geometry - Geometry shader file
			Fragment - Fragment shader file
			Returns pointer to this shader
		 */
		Shader FLUFF_API * Recreate(const char * Vertex, const char * Geometry, const char * Fragment);

		/*
			Recreates shader program
			
			Vertex - Vertex shader file
			Geometry - Geometry shader file
			TesselationControl - Tesselation control shader file
			TesselationEvaluation - Tesselation evaluation shader file
			Fragment - Fragment shader file
			Returns pointer to this shader
		 */
		Shader FLUFF_API * Recreate(const char * Vertex, const char * Geometry, const char * TesselationControl, const char * TesselationEvaluation, const char * Fragment);

		/*
			Binds shader for drawing
		*/
		void FLUFF_API Bind() const;

		/*
			Unbinds shader
		*/
		static void Unbind();

		/*
			Releases shader resources
		 */
		void FLUFF_API Release();

		/*
			Gets the handle of the shader

			Returns handle of shader
		 */
		uint32_t FLUFF_API GetHandle() const { return Id_; }

		/*
			Binds texture to location
			
			Name - Name of variable
			Location - Location of texture
		 */
		void FLUFF_API BindTextureLocation(const char * Name, unsigned int Location) const;

	private:
		unsigned int FLUFF_API CreateShaderStage(const char *File, uint32_t Type) const;
		void FLUFF_API LinkProgram() const;
		void FLUFF_API VerifyProgram() const;
		void FLUFF_API DetachShaderStages(std::vector<unsigned int> Stages) const;
	};

	inline std::string ReadTextFile(const char* Filename)
	{
		std::ifstream stream(Filename);
		std::stringstream buffer;
		buffer << stream.rdbuf();
		return buffer.str().c_str();
	}

} }
