#pragma once

/*! \file defines.hpp
	\brief A file with all Utility macros.

	All utility macros can be found here. These are usually either debug/release dependend or useful and often used macros.
*/

// STD includes
#include <cassert>

/*! \def UNUSED(x)
	\brief Marks a variable/function as unused to prevent a warning.

	Marks a variable/function as unused to prevent warning C4101 - 'identifier' : unreferenced local variable
*/
#define UNUSED(x) ((void)(x))

#ifdef _DEBUG
	/*! \def ASSERT(...)
		\brief shortcut for debug and release dependend assert

		When building in a Debug configuration ASSERT is set to assert based on the arguments that it receives from the user.
		When building in a Release configuration ASSERT doesn't execute anything.
	*/
	#define ASSERT(...) assert(__VA_ARGS__)
#else
	#define ASSERT(...)
#endif