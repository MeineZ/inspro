#pragma once

#include <cmath>

namespace insp
{
	inline float ffmin( float a, float b )
	{
		return a < b ? a : b;
	}
	
	inline float ffmax( float a, float b )
	{
		return a > b ? a : b;
	}
}