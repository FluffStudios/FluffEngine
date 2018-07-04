#pragma once

#include <rendering/material.h>
#include <gfx/model.h>

namespace fluff { namespace render {

	using namespace gfx;
	class Renderable
	{
	protected:
		Material * Material_;
		Model * Model_;
	public:
		/*
			Default renderable constructor
		*/
		FLUFF_API Renderable() : Renderable(nullptr, nullptr) { }

		/*
			Creates a new renderable object in the world
			
			MaterialHandle - pointer to material
			ModelHandle - pointer to model
		 */
		FLUFF_API Renderable(Material* MaterialHandle, Model* ModelHandle)
		{
			Material_ = MaterialHandle;
			Model_ = ModelHandle;
		}

		/*
			Gets the model associated with the renderable object

			Returns model pointer
		 */
		inline Model* GetModel() const { return Model_; }

		/*
			Gets the material associated with the renderable object

			Returns material pointer
		*/
		inline Material* GetMaterial() const { return Material_; }
	};

} }
