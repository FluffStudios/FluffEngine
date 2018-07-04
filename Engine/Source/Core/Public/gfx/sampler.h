#pragma once

#include <common.h>
#include <gfx/texture_info.h>

namespace fluff { namespace gfx
{

	class FLUFF_API Sampler
	{
		unsigned int Sampler_;
		TextureFilter Filter_;
	public:
		/*
			Creates new sampler object

			Filter - Filter for sampling
		*/
		explicit Sampler(TextureFilter Filter);

		/*
			Destructor
		*/
		~Sampler();
		
		/*
			Binds the sampler to a location
			
			Location - Location to bind to
		 */
		void Bind(unsigned int Location) const;

		/*
			Unbinds the sampler to a location
			
			Location - Location to unbind
		 */
		void Unbind(unsigned int Location) const;

		/*
			Releases resources held by the sampler
		*/
		void Release();
	};

} }
