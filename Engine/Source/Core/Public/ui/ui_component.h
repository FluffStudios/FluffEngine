#pragma once

#include <common.h>

namespace fluff { namespace ui {

	class UIComponent 
	{
	protected:
		bool Destructed;
	public:
		virtual FLUFF_API ~UIComponent();
		virtual void Construct() = 0;
		virtual void Destruct() = 0;
		virtual void Draw() = 0;
	};

} }