#pragma once

#include <gfx/texture_info.h>
#include <string>
#include <vector>
#include <unordered_map>
#include <gfx/texture_data.h>

namespace fluff { namespace gfx {

	class Texture
	{
	protected:
		uint32_t Id_ = 0;
		uint64_t Handle_ = 0;
		TextureCreateInfo CreateInfo_;
		std::string Name_ = "null";

		static std::unordered_map<uint32_t, std::string> IDMap_;
	public:
		/*
			Creates a new texture
		*/
		FLUFF_API Texture() { }

		/*
			Destructor
		*/
		virtual FLUFF_API ~Texture() { Release(); }

		/*
			Gets the texture handle
			
			Returns handle
		 */
		inline uint32_t & GetHandle() { return Id_; }

		/*
			Sets the name of the texture

			Name - Texture Name
		*/
		inline void SetName(std::string Name)
		{
			Name_ = Name;
			IDMap_.find(Id_)->second = Name;
		}

		/*
			Gets the texture name

			Returns texture name
		*/
		inline std::string GetName()
		{
			return Name_;
		}

		inline static std::string GetName(uint32_t ID)
		{
			return IDMap_.find(ID)->second;
		}

		/*
			Releases texture resources
		*/
		void FLUFF_API Release();

		/*
			Enables texture
		*/
		virtual void FLUFF_API Enable() { }

		/*
			Disables texture
		*/
		virtual void FLUFF_API Disable() { }

		/*
			Makes texture handle resident to GPU
		*/
		void FLUFF_API MakeTextureHandleResident() const;
	};

	class Texture2D : public Texture
	{
	public:
		/*
			Creates a new Texture2D from Texture

			Tex - Parent texture object
		*/
		explicit FLUFF_API Texture2D(Texture Tex)
			: Texture(Tex), Location(0)
		{
		}

		FLUFF_API Texture2D() { }

		FLUFF_API ~Texture2D() { }

		/*
			Creates new Texture2D from data 
			
			Data - byte array
			CreateInfo - texture creation parameters
		 */
		FLUFF_API Texture2D(std::vector<unsigned char*> &Data, TextureCreateInfo CreateInfo);

		FLUFF_API Texture2D(std::vector<unsigned char> Data, TextureCreateInfo CreateInfo);

		explicit FLUFF_API Texture2D(TextureData & Data);

		/*
			Enable texture
		 */
		void FLUFF_API Enable() override;

		/*
			Disable texture
		 */
		void FLUFF_API Disable() override;

		uint32_t Location;
	private:
		void FLUFF_API Initialize(std::vector<unsigned char*> &Data);
		void FLUFF_API Initialize(std::vector<unsigned char> Data);
	};

	class Texture3D : public Texture
	{
	public:
		/*
			Creates new Texture3D
			
			@param Tex - base texture
		 */
		explicit FLUFF_API Texture3D(Texture Tex)
			: Texture(Tex)
		{
		}

		FLUFF_API ~Texture3D() { }

		/*
			Creates new Texture3D from data 
			
			Data - byte array
			CreateInfo - texture creation parameters
		 */
		FLUFF_API Texture3D(std::vector<unsigned char*> &Data, TextureCreateInfo CreateInfo);

		/*
			Enable texture
		 */
		void FLUFF_API Enable() override;

		/*
			Disable texture
		 */
		void FLUFF_API Disable() override;
	private:
		void FLUFF_API Initialize(std::vector<unsigned char*> &Data);
	};

	class TextureCubeMap : public Texture
	{
	public:
		/*
			Creates new TextureCubeMap
			
			Tex - base texture
		 */
		explicit FLUFF_API TextureCubeMap(Texture Tex)
			: Texture(Tex)
		{
		}

		FLUFF_API ~TextureCubeMap() { }

		/*
			Creates new TextureCubeMap from data 
			
			Data - byte array
			CreateInfo - texture creation parameters
		 */
		FLUFF_API TextureCubeMap(std::vector<unsigned char*> &Data, TextureCreateInfo CreateInfo);

		/*
			Enable texture
		 */
		void FLUFF_API Enable() override;

		/*
			Disable texture
		 */
		void FLUFF_API Disable() override;
	private:
		void FLUFF_API Initialize(std::vector<unsigned char*> &Data);
	};

	/*
		Binds texture array
		
		Textures - Texture array
		StartSlot - Starting texture slot
		NumViews - Number of views
	 */
	void BindTextures(Texture **Textures, int StartSlot, int NumViews);

} }
