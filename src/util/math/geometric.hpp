#pragma once

#include <cmath>

namespace insp
{
	
	inline float Schlick( const float cosine, const float refIdx )
	{
		float r0 = ( 1.0f - refIdx ) / ( 1.0f + refIdx );
		r0 = r0 * r0;
		return r0 + ( 1.0f - r0 ) * std::powf( ( 1.0f - cosine ), 5.0f );
	}

}