#pragma once

// Inspro includes
#include <inspro/raytracing/ray.hpp>

// Util includes
#include <util/random.hpp>

// Third party includes
#include <glm/gtc/constants.hpp>
#include <glm/geometric.hpp>


namespace insp
{
	class Camera
	{
	public:
		Camera():
			lowerLeftCorner( -2.0f, -1.0f, -1.0f ),
			horizontal( 4.0f, 0.0f, 0.0f ),
			vertical( 0.0f, 2.0f, 0.0f ),
			origin( 0.0f, 0.0f, 0.0f ),
			lookAt( 0.0f, 0.0f, 0.0f ),
			up( 0.0f, 0.0f, 0.0f ),
			u( 0.0f ),
			v( 0.0f ),
			w( 0.0f ),
			lensRadius( 0.0f )
		{ }

		inline void LookAt( glm::vec3 &position, glm::vec3 &focusPoint, glm::vec3 &upDirection, float fovDeg, float aspectRatio, float aperature, float focusDistance )
		{
			this->lensRadius = aperature / 2.0f;
			float theta = fovDeg * ( glm::pi<float>() / 180.f );
			this->vertical.y = tan( theta / 2.f );
			this->horizontal.x = aspectRatio * this->vertical.y;
			this->lookAt = focusPoint;
			this->up = upDirection;

			// Set origin
			this->origin = position;

			this->w = glm::normalize( this->origin - this->lookAt );
			this->u = glm::normalize( glm::cross( this->up, w ) );
			this->v = glm::cross( w, u );

			this->lowerLeftCorner = this->origin - this->horizontal.x * focusDistance * u - this->vertical.y * focusDistance * v - focusDistance * w;
			this->horizontal = 2.0f * this->horizontal.x * focusDistance * u;
			this->vertical = 2.0f * this->vertical.y * focusDistance * v;
		}

		inline insp::Ray GetRay( float s, float t )
		{
			glm::vec3 radius = lensRadius * insp::RandomInUnitDisc();
			glm::vec3 offset = this->u * radius.x + this->v * radius.y;
			return Ray( this->origin + offset,
						this->lowerLeftCorner + s * this->horizontal + t * this->vertical - this->origin - offset );
		}

		glm::vec3 origin;
		glm::vec3 lowerLeftCorner;
		glm::vec3 horizontal;
		glm::vec3 vertical;

		glm::vec3 lookAt;
		glm::vec3 up;
		glm::vec3 u;
		glm::vec3 v;
		glm::vec3 w;
		float lensRadius;
	};

} // insp namespace