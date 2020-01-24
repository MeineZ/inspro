#pragma once
#include <inspro/ray.hpp>

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
			up( 0.0f, 0.0f, 0.0f )
		{ }

		inline void LookAt( glm::vec3 &position, glm::vec3 &focusPoint, glm::vec3 &upDirection, float fovDeg, float aspectRatio )
		{
			float theta = fovDeg * ( glm::pi<float>() / 180.f );
			this->vertical.y = tan( theta / 2.f );
			this->horizontal.x = aspectRatio * this->vertical.y;
			this->lookAt = focusPoint;
			this->up = upDirection;

			// Set origin
			this->origin = position;

			glm::vec3 w = glm::normalize( this->origin - this->lookAt );
			glm::vec3 u = glm::normalize( glm::cross( this->up, w ) );
			glm::vec3 v = glm::cross( w, u );

			this->lowerLeftCorner = this->origin - this->horizontal.x * u - this->vertical.y * v - w;
			this->horizontal = 2.0f * this->horizontal.x * u;
			this->vertical = 2.0f * this->vertical.y * v;
		}

		inline insp::Ray GetRay( float u, float v )
		{
			return Ray( this->origin,
						this->lowerLeftCorner + u * this->horizontal + v * this->vertical - this->origin );
		}

		glm::vec3 origin;
		glm::vec3 lowerLeftCorner;
		glm::vec3 horizontal;
		glm::vec3 vertical;

		glm::vec3 lookAt;
		glm::vec3 up;
	};

} // insp namespace