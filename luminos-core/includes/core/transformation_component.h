#pragma once

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <common.h>
#include <core/beans/property.h>
#include <cmath>

namespace luminos
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
		TransformationComponent()
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
		TransformationComponent(const glm::vec3 Position, const glm::vec3 Rotation, const glm::vec3 Scale) : TransformationComponent()
		{ 
			this->Position = Position;
			this->Rotation = Rotation;
			this->Scale = Scale;
		}

		/*
			Gets the current position

			Returns position
		*/
		glm::vec3 GetPosition() const
		{
			return this->Position;
		}

		/*
			Sets the current position

			Position - new position
		*/
		void SetPosition(const glm::vec3 Position)
		{
			this->Position = Position;
			CreateTransformationMatrix();
		}

		/*
			Gets the rotation in euler angles as degrees

			Returns rotation
		*/
		glm::vec3 GetRotation() const
		{
			return this->Rotation;
		}

		/*
			Sets the rotation as euler angles

			RotationInDegrees - new rotation
		*/
		void SetRotation(const glm::vec3 RotationInDegrees)
		{
			glm::vec3 new_rot(fmod(RotationInDegrees.x + 360.0f, 360), fmod(RotationInDegrees.y + 360.0f, 360), fmod(RotationInDegrees.z + 360.0f, 360));
			this->Rotation = new_rot;
			CreateTransformationMatrix();
		}

		/*
			Gets the scale of the transformation

			Returns scale
		*/
		glm::vec3 GetScale() const
		{
			return this->Scale;
		}

		/*
			Sets the scale of the transformation

			Returns scale
		*/
		void SetScale(const glm::vec3 Scale)
		{
			this->Scale = Scale;
			CreateTransformationMatrix();
		}

		/*
			Gets the transformation matrix 

			Returns transformation matrix
		*/
		glm::mat4 GetTransformationMatrix() const
		{
			return this->TransformationMatrix;
		}

	private:
		glm::mat4 CreateTransformationMatrix()
		{
			glm::mat4 pos(1.0f);
			glm::mat4 sca(1.0f);

			pos = glm::translate(pos, Position);
			sca = glm::scale(sca, Scale);

			glm::mat4 rot(1.0f);
			rot = glm::rotate(rot, glm::radians(Rotation.x), glm::vec3(1, 0, 0));
			rot = glm::rotate(rot, glm::radians(Rotation.y), glm::vec3(0, 1, 0));
			rot = glm::rotate(rot, glm::radians(Rotation.z), glm::vec3(0, 0, 1));
			return (TransformationMatrix = pos * rot * sca);
		}
	};
}