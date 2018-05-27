#include <physics/physics_error_callback.h>

#include <sstream>
#include <common.h>

#include <core/debug_message.h>

namespace luminos { namespace physics {

	void PhysicsErrorCallback::reportError(physx::PxErrorCode::Enum code, const char *message, const char *file, int line)
	{
		std::stringstream stream;
		stream << "PhysX Error!" << std::endl;
		stream << "PhysX Error Code : " << code << std::endl;
		stream << "Error Description: " << message << std::endl;
		stream << "Error in File:     " << file << std::endl;
		stream << "Error on Line:     " << line << std::endl;
		std::cout << stream.str() << std::endl;
		LUMINOS_ASSERT()
	}

} }