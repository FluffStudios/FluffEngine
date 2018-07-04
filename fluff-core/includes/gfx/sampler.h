#pragma once

#include <common.h>
#include <gfx/texture_info.h>

namespace fluff { namespace gfx
{

	class Sampler
	{
		unsigned int Sampler_;
		TextureFilter Filter_;
	public:
		/*
			Creates new sampler object

			Filter - Filter for sampling
		*/
		explicit FLUFF_API Sampler(TextureFilter Filter);

		/*
			Destructor
		*/
		FLUFF_API ~Sampler();
		
		/*
			Binds the sampler to a location
			
			Location - Location to bind to
		 */
		void FLUFF_API Bind(unsigned int Location) const;

		/*
			Unbinds the sampler to a location
			
			Location - Location to unbind
		 */
		void FLUFF_API Unbind(unsigned int Location) const;

		/*
			Releases resources held by the sampler
		*/
		void FLUFF_API Release();
	};

} }
