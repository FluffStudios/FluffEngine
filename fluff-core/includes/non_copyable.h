#pragma once

namespace fluff {

	class NonCopyable
	{
	protected:
		NonCopyable() = default;
		~NonCopyable() = default;
		NonCopyable(const NonCopyable&) = delete; // delete copy mechanism
		NonCopyable& operator = (const NonCopyable &) = delete; // delete copy mechanism
	};

}