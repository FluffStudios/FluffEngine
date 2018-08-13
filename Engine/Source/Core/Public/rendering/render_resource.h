#pragma once

namespace fluff { namespace render {

	class IRenderResource 
	{
		unsigned int RefCount_;
	public:
		virtual ~IRenderResource();

		void Consume();
		void AddReference();

		virtual void Construct() = 0;
		virtual void Destruct() = 0;
	};

} }