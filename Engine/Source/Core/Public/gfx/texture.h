#pragma once

#include <gfx/texture_info.h>
#include <string>
#include <vector>
#include <unordered_map>
#include <gfx/texture_data.h>

namespace fluff { namespace gfx {

	class FLUFF_API Texture
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
		Texture() { }

		/*
			Destructor
		*/
		virtual ~Texture() { Release(); }

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
		void Release();

		/*
			Enables texture
		*/
		virtual void Enable() { }

		/*
			Disables texture
		*/
		virtual void Disable() { }

		/*
			Makes texture handle resident to GPU
		*/
		void MakeTextureHandleResident() const;
	};

	class FLUFF_API Texture2D : public Texture
	{
	public:
		/*
			Creates a new Texture2D from Texture

			Tex - Parent texture object
		*/
		explicit Texture2D(Texture Tex)
			: Texture(Tex), Location(0)
		{
		}

		Texture2D() { }

		~Texture2D() { }

		/*
			Creates new Texture2D from data 
			
			Data - byte array
			CreateInfo - texture creation parameters
		 */
		Texture2D(std::vector<unsigned char*> &Data, TextureCreateInfo CreateInfo);

		Texture2D(std::vector<unsigned char> Data, TextureCreateInfo CreateInfo);

		Texture2D(TextureData & Data);

		/*
			Enable texture
		 */
		void Enable() override;

		/*
			Disable texture
		 */
		void Disable() override;

		uint32_t Location;
	private:
		void Initialize(std::vector<unsigned char*> &Data);
		void Initialize(std::vector<unsigned char> Data);
	};

	class FLUFF_API Texture3D : public Texture
	{
	public:
		/*
			Creates new Texture3D
			
			@param Tex - base texture
		 */
		explicit Texture3D(Texture Tex)
			: Texture(Tex)
		{
		}

		~Texture3D() { }

		/*
			Creates new Texture3D from data 
			
			Data - byte array
			CreateInfo - texture creation parameters
		 */
		Texture3D(std::vector<unsigned char*> &Data, TextureCreateInfo CreateInfo);

		/*
			Enable texture
		 */
		void Enable() override;

		/*
			Disable texture
		 */
		void Disable() override;
	private:
		void Initialize(std::vector<unsigned char*> &Data);
	};

	class FLUFF_API TextureCubeMap : public Texture
	{
	public:
		/*
			Creates new TextureCubeMap
			
			Tex - base texture
		 */
		explicit TextureCubeMap(Texture Tex)
			: Texture(Tex)
		{
		}

		~TextureCubeMap() { }

		/*
			Creates new TextureCubeMap from data 
			
			Data - byte array
			CreateInfo - texture creation parameters
		 */
		TextureCubeMap(std::vector<unsigned char*> &Data, TextureCreateInfo CreateInfo);

		/*
			Enable texture
		 */
		void Enable() override;

		/*
			Disable texture
		 */
		void Disable() override;
	private:
		void Initialize(std::vector<unsigned char*> &Data);
	};

	/*
		Binds texture array
		
		Textures - Texture array
		StartSlot - Starting texture slot
		NumViews - Number of views
	 */
	void BindTextures(Texture **Textures, int StartSlot, int NumViews);

} }
