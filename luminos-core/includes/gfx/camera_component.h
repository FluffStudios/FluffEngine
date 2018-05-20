#pragma once

#include <common.h>
#include <core/ecs/entity.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#ifndef FORWARD
#define FORWARD glm::vec3(0, 0, 1)
#endif

#ifndef UP
#define UP glm::vec3(0, 1, 0);
#endif

namespace luminos { namespace gfx {

	class LUMINOS_API CameraComponent : public ecs::Component<CameraComponent>
	{
		glm::vec3 Position_;
		glm::vec3 Direction_ = FORWARD;
		glm::vec3 Up_ = UP;
		glm::vec3 Rotation_;
		glm::mat4 ViewMatrix_;
	public:
		/*
		Creates new pinhole camera
		*/
		CameraComponent() { }

		/*
		Sets position

		Position - New position
		*/
		void SetPosition(const glm::vec3& Position)
		{
			this->Position_ = Position;
			ViewMatrix();
		}

		/*
		Sets rotation of camera

		Rotation - New rotation
		*/
		void SetDirection(const glm::vec3& Direction)
		{
			this->Direction_ = Direction;
		}

		/*
		Gets a reference to the camera's position

		Return camera position
		*/
		inline const glm::vec3& GetPosition() const { return Position_; }

		/*
		Gets the rotation of the camera as euler angles

		Returns camera position
		*/
		inline glm::vec3 GetRotation() const
		{
			return Rotation_;
		}

		/*
		Sets the rotation of the camera in euler angles (degrees)

		Rotation - Rotation of camera
		*/
		inline glm::vec3 SetRotation(const glm::vec3& Rotation)
		{
			Rotation_ = Rotation;
			Rotation_.y = Rotation_.y >= 360.0f ? Rotation_.y - 360.0f : (Rotation_.y < 0 ? Rotation_.y + 360.0f : Rotation_.y);
			Rotation_.x = Rotation_.x > 90 ? 90 : (Rotation_.x < -90 ? -90 : Rotation_.x);
			Direction_ = glm::vec3(glm::sin(glm::radians(Rotation_.y)), glm::sin(glm::radians(Rotation_.x)), glm::cos(glm::radians(Rotation_.y)));
			ViewMatrix();
			return Direction_;
		}


		/*
		Gets the camera's view matrix

		@return view matrix
		*/
		inline const glm::mat4& GetViewMatrix() const { return ViewMatrix_; }

		/*
		Checks if two cameras are equivalent to each other

		Other - Camera to compare to
		Returns if the cameras are equivalent
		*/
		inline bool operator == (const CameraComponent & Other) const { return (this->Position_ == Other.Position_) && (this->Direction_ == Other.Direction_) && (this->Up_ == Other.Up_); }

		/*
		Checks if two cameras are not equivalent to each other
		*/
		inline bool operator != (const CameraComponent & Other) const { return !(*this == Other); }
	private:
		void ViewMatrix()
		{
			ViewMatrix_ = glm::lookAt(Position_, Position_ + Direction_, Up_);
		}
	};

} }