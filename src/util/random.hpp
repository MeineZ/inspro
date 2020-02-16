#pragma once

// STD includes
#include <cstdlib>

// Third party includes
#include <glm/vec3.hpp>

/*!
	\namespace Inspro namespace.
*/
namespace insp
{
	static inline float RandomFloat()
	{
		return static_cast<float>( rand() / ( RAND_MAX + 1.0f ) );
	}

	static inline glm::vec3 RandomInUnitSphere()
	{
		return glm::normalize( 2.0f * glm::vec3( RandomFloat(), RandomFloat(), RandomFloat() ) - glm::vec3( 1.0f ) ) * RandomFloat();
	}

	static inline glm::vec3 RandomInUnitDisc()
	{
		return glm::normalize( 2.0f * glm::vec3( RandomFloat(), RandomFloat(), 0.0f ) - glm::vec3( 1.0f, 1.0f, 0.0f ) ) * RandomFloat();
	}
};