#pragma once

#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/glm.hpp>
#include <common.h>
#include <core/beans/property.h>
#include <cmath>

namespace fluff
{
	class TransformationComponent
	{
		glm::vec3 Position;
		glm::vec3 Rotation;
		glm::vec3 Scale;
		
		glm::mat4 TransformationMatrix; // DO NOT DIRECTLY MODIFY
	public:
		/*
			Creates default transformation
		*/
		FLUFF_API TransformationComponent()
		{
			Scale = glm::vec3(1, 1, 1);
			CreateTransformationMatrix();
		}

		/*
			Creates transformation

			Position - Position to translate to
			Rotation - Rotation on each axis in degrees
			Scale - Scaling factor across each axis
		*/
		FLUFF_API TransformationComponent(const glm::vec3 Position, const glm::vec3 Rotation, const glm::vec3 Scale) : TransformationComponent()
		{ 
			this->Position = Position;
			this->Rotation = Rotation;
			this->Scale = Scale;
		}

		/*
			Gets the current position

			Returns position
		*/
		glm::vec3 FLUFF_API GetPosition() const
		{
			return this->Position;
		}

		/*
			Sets the current position

			Position - new position
		*/
		void FLUFF_API SetPosition(const glm::vec3 Position)
		{
			this->Position = Position;
			CreateTransformationMatrix();
		}

		glm::vec3 FLUFF_API GetRotation() const
		{
			return Rotation;
		}

		/*
			Sets the rotation as euler angles

			RotationInDegrees - new rotation
		*/
		void FLUFF_API SetRotation(const glm::vec3 RotationInDegrees)
		{
			glm::vec3 new_rot(fmod(RotationInDegrees.x + 360.0f, 360), fmod(RotationInDegrees.y + 360.0f, 360), fmod(RotationInDegrees.z + 360.0f, 360));
			this->Rotation = new_rot;
			CreateTransformationMatrix();
		}

		/*
			Gets the scale of the transformation

			Returns scale
		*/
		glm::vec3 FLUFF_API GetScale() const
		{
			return this->Scale;
		}

		/*
			Sets the scale of the transformation

			Returns scale
		*/
		void FLUFF_API SetScale(const glm::vec3 Scale)
		{
			this->Scale = Scale;
			CreateTransformationMatrix();
		}

		/*
			Gets the transformation matrix 

			Returns transformation matrix
		*/
		glm::mat4 FLUFF_API GetTransformationMatrix() const
		{
			return this->TransformationMatrix;
		}

	private:
		glm::mat4 FLUFF_API CreateTransformationMatrix()
		{
			glm::mat4 pos(1.0f);
			glm::mat4 sca(1.0f);

			pos = glm::translate(pos, Position);
			auto rot = glm::rotate(glm::radians(Rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
			rot *= glm::rotate(glm::radians(Rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
			rot *= glm::rotate(glm::radians(Rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
			sca = glm::scale(sca, Scale);

			return (TransformationMatrix = pos * rot * sca);
		}
	};
}