#pragma once

#include <debug.h>
#include <cereal/types/polymorphic.hpp>

#define VERSION_MAJOR	0
#define VERSION_MINOR	0
#define VERSION_BUGFIX	0

#define ENGINE_VERSION ((VERSION_MAJOR << 24) + (VERSION_MINOR << 16) + (VERSION_BUGFIX << 8) + 0)

#define REGISTER_POLYMORPHIC_CLASS(Base, Derived) \
	CEREAL_REGISTER_TYPE(Derived) \
	CEREAL_REGISTER_POLYMORPHIC_RELATION(Base, Derived)

#define REGISTER_POLYMORPHIC_CLASS_CRTP(IBase, Base, Derived) \
	REGISTER_POLYMORPHIC_CLASS(IBase, Base) \
	REGISTER_POLYMORPHIC_CLASS(Base, Derived) 