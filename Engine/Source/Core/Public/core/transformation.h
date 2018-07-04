#pragma once

#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <common.h>

namespace fluff
{

	class FLUFF_API Transformation
	{
		glm::vec3 Position;
		glm::quat Rotation;
		glm::vec3 Scale;

		glm::mat4 TransformationMatrix; // DO NOT DIRECTLY MODIFY
	public:
		/*
			Creates default transformation
		*/
		Transformation()
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
		Transformation(const glm::vec3 Position, const glm::vec3 Rotation, const glm::vec3 Scale) : Transformation()
		{
			this->Position = Position;
			this->Rotation = glm::quat(glm::radians(Rotation));
			this->Scale = Scale;
		}

		/*
			Creates transformation

			Position - Position to translate to
			Rotation - Rotating quaternion
			Scale - Scaling factor across each axis
		*/
		Transformation(const glm::vec3 Position, const glm::quat Rotation, const glm::vec3 Scale) : Transformation()
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
		glm::vec3 GetRotationInDegrees() const
		{
			return glm::degrees(glm::eulerAngles(this->Rotation));
		}

		/*
			Gets the rotation as a quaternion

			Returns rotation
		*/
		glm::quat GetRotation() const
		{
			return Rotation;
		}

		/*
			Sets the rotation as euler angles

			RotationInDegrees - new rotation
		*/
		void SetRotation(const glm::vec3 RotationInDegrees)
		{
			this->Rotation = glm::quat(glm::radians(RotationInDegrees));
			CreateTransformationMatrix();
		}

		/*
			Sets the rotation as a quaternion

			Rotation - new rotation
		*/
		void SetRotation(const glm::quat Rotation)
		{
			this->Rotation = Rotation;
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
			return (TransformationMatrix = pos * glm::toMat4(Rotation) * sca);
		}
	};

}
