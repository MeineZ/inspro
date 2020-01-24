#pragma once

// STD includes
#include <cstdlib>

/*!
	\namespace Inspro namespace.
*/
namespace insp
{
	static inline float RandomFloat()
	{
		return static_cast<float>(rand() / ( RAND_MAX + 1.0f));
	}
};