#pragma once

// STD includes
#include <vector>

/*!
	\namespace Inspro namespace.
*/
namespace insp
{
	/*!
		\namespace Vector namespace.
		All vector related functions are in here
	*/
	namespace vector
	{
		//! Replaces a vector by another vector.
		/*!
			ins::vector::Replace replaces a vector by another vector by either copying the raw together into the other vector 
			buffer or using the copy assignment depending on the type inside the vector.
			The template is the type inside the vector. It's used to detect what replacement function should be used.
			\param current The vector with the data to be copied
			\param target The vector to copy the data to
		*/
		template<typename T>
		static inline void Replace(const std::vector<T>& current, std::vector<T>& target)
		{
			if constexpr (std::is_arithmetic_v<T> || std::is_pod_v<T>)
			{
				// Resize the target vector
				target.resize(current.size());

				memcpy(target.data(), current.data(), sizeof(T) * current.size());
			}
			else
			{
				target = current;
			}
		}
	};
};