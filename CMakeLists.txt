
cmake_minimum_required(VERSION 3.14)

# === Output directories === #
set(PROJECT_ROOT_DIR ${CMAKE_SOURCE_DIR})
set(PROJECT_SOURCE_DIR ${PROJECT_ROOT_DIR}/src)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_ROOT_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_ROOT_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_ROOT_DIR}/bin)

set(INSPRO_SOURCE_DIR ${PROJECT_SOURCE_DIR}/inspro)
set(UTIL_SOURCE_DIR ${PROJECT_SOURCE_DIR}/util)
set(GLM_SOURCE_DIR ${PROJECT_ROOT_DIR}/deps/glm/glm)


# === Configuration === #
set(PROJECT_NAME "InsproProject")
set(INSPRO_EXTENSION ".exe")
set(INSPRO_NAME "Inspro")
set(UTIL_NAME "InsproUtil")
set(GLM_NAME "InsproGLM")


# === Display Init Info === #
message("\n")
message(STATUS "Project root dir:\t\t" ${PROJECT_ROOT_DIR})
message(STATUS "Project source dir:\t\t" ${PROJECT_SOURCE_DIR})
message(STATUS "CMake archive output dir:\t" ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})
message(STATUS "CMake library output dir:\t" ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
message(STATUS "CMake runtime output dir:\t" ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} "\n\n")


# === Create solution === #
project(${PROJECT_NAME})


######################### GLM #############################
# === GLM Files === #
file(GLOB GLM_ROOT_SOURCE ${GLM_SOURCE_DIR}/*.cpp)
file(GLOB GLM_ROOT_INLINE ${GLM_SOURCE_DIR}/*.inl)
file(GLOB GLM_ROOT_HEADER ${GLM_SOURCE_DIR}/*.hpp)
file(GLOB_RECURSE GLM_CORE_SOURCE ${GLM_SOURCE_DIR}/detail/*.cpp)
file(GLOB_RECURSE GLM_CORE_INLINE ${GLM_SOURCE_DIR}/detail/*.inl)
file(GLOB_RECURSE GLM_CORE_HEADER ${GLM_SOURCE_DIR}/detail/*.hpp)
file(GLOB_RECURSE GLM_EXT_SOURCE ${GLM_SOURCE_DIR}/ext/*.cpp)
file(GLOB_RECURSE GLM_EXT_INLINE ${GLM_SOURCE_DIR}/ext/*.inl)
file(GLOB_RECURSE GLM_EXT_HEADER ${GLM_SOURCE_DIR}/ext/*.hpp)
file(GLOB_RECURSE GLM_GTC_SOURCE ${GLM_SOURCE_DIR}/gtc/*.cpp)
file(GLOB_RECURSE GLM_GTC_INLINE ${GLM_SOURCE_DIR}/gtc/*.inl)
file(GLOB_RECURSE GLM_GTC_HEADER ${GLM_SOURCE_DIR}/gtc/*.hpp)
file(GLOB_RECURSE GLM_GTX_SOURCE ${GLM_SOURCE_DIR}/gtx/*.cpp)
file(GLOB_RECURSE GLM_GTX_INLINE ${GLM_SOURCE_DIR}/gtx/*.inl)
file(GLOB_RECURSE GLM_GTX_HEADER ${GLM_SOURCE_DIR}/gtx/*.hpp)
file(GLOB_RECURSE GLM_SIMD_SOURCE ${GLM_SOURCE_DIR}/simd/*.cpp)
file(GLOB_RECURSE GLM_SIMD_INLINE ${GLM_SOURCE_DIR}/simd/*.inl)
file(GLOB_RECURSE GLM_SIMD_HEADER ${GLM_SOURCE_DIR}/simd/*.h)

# === GLM Source groups === #
source_group("Core Files" FILES ${GLM_CORE_SOURCE})
source_group("Core Files" FILES ${GLM_CORE_INLINE})
source_group("Core Files" FILES ${GLM_CORE_HEADER})
source_group("EXT Files" FILES ${GLM_EXT_SOURCE})
source_group("EXT Files" FILES ${GLM_EXT_INLINE})
source_group("EXT Files" FILES ${GLM_EXT_HEADER})
source_group("GTC Files" FILES ${GLM_GTC_SOURCE})
source_group("GTC Files" FILES ${GLM_GTC_INLINE})
source_group("GTC Files" FILES ${GLM_GTC_HEADER})
source_group("GTX Files" FILES ${GLM_GTX_SOURCE})
source_group("GTX Files" FILES ${GLM_GTX_INLINE})
source_group("GTX Files" FILES ${GLM_GTX_HEADER})
source_group("SIMD Files" FILES ${GLM_SIMD_SOURCE})
source_group("SIMD Files" FILES ${GLM_SIMD_INLINE})
source_group("SIMD Files" FILES ${GLM_SIMD_HEADER})

# === Create GLM library === #
add_library(${GLM_NAME} STATIC ${GLM_ROOT_TEXT} ${GLM_ROOT_MD} ${GLM_ROOT_NAT}
	${GLM_ROOT_SOURCE}    ${GLM_ROOT_INLINE}    ${GLM_ROOT_HEADER}
	${GLM_CORE_SOURCE}    ${GLM_CORE_INLINE}    ${GLM_CORE_HEADER}
	${GLM_EXT_SOURCE}     ${GLM_EXT_INLINE}     ${GLM_EXT_HEADER}
	${GLM_GTC_SOURCE}     ${GLM_GTC_INLINE}     ${GLM_GTC_HEADER}
	${GLM_GTX_SOURCE}     ${GLM_GTX_INLINE}     ${GLM_GTX_HEADER}
	${GLM_SIMD_SOURCE}    ${GLM_SIMD_INLINE}    ${GLM_SIMD_HEADER})

# === GLM include directories === #
target_include_directories(${GLM_NAME} PUBLIC ${GLM_SOURCE_DIR}/..)
target_include_directories(${GLM_NAME} PUBLIC ${GLM_SOURCE_DIR})

# === GLM additional properties === #
set_target_properties(${GLM_NAME} PROPERTIES CXX_STANDARD 20)
set_target_properties(${GLM_NAME} PROPERTIES CXX_EXTENSIONS OFF)
set_target_properties(${GLM_NAME} PROPERTIES CMAKE_CXX_STANDARD_REQUIRED ON)

######################### UTIL #############################
# === Util files === #
file(GLOB UTIL_HEADERS "${UTIL_SOURCE_DIR}/*.hpp")
file(GLOB UTIL_SOURCES "${UTIL_SOURCE_DIR}/*.cpp")
file(GLOB_RECURSE UTIL_STB_HEADERS "${UTIL_SOURCE_DIR}/stb/*.hpp")
file(GLOB_RECURSE UTIL_STB_SOURCES "${UTIL_SOURCE_DIR}/stb/*.cpp")
file(GLOB_RECURSE UTIL_MATH_HEADERS "${UTIL_SOURCE_DIR}/math/*.hpp")
file(GLOB_RECURSE UTIL_MATH_SOURCES "${UTIL_SOURCE_DIR}/math/*.cpp")

# === Util source groups === #
source_group("Main" FILES ${UTIL_HEADERS} ${UTIL_SOURCES})
source_group("stb" FILES ${UTIL_STB_HEADERS} ${UTIL_STB_SOURCES})
source_group("math" FILES ${UTIL_MATH_HEADERS} ${UTIL_MATH_SOURCES})

# === Create Util library === #
add_library(${UTIL_NAME} STATIC 
	${UTIL_HEADERS} ${UTIL_SOURCES}
	${UTIL_STB_HEADERS} ${UTIL_STB_SOURCES}
	${UTIL_MATH_HEADERS} ${UTIL_MATH_SOURCES})

# === Util include directories === #
target_include_directories(${UTIL_NAME} PUBLIC ${UTIL_SOURCE_DIR})
target_include_directories(${UTIL_NAME} PRIVATE ${PROJECT_ROOT_DIR}/deps/stb)

# === Util additional properties === #
set_target_properties(${UTIL_NAME} PROPERTIES CXX_STANDARD 20)
set_target_properties(${UTIL_NAME} PROPERTIES CXX_EXTENSIONS OFF)
set_target_properties(${UTIL_NAME} PROPERTIES CMAKE_CXX_STANDARD_REQUIRED ON)


######################### INSPRO #############################
# === Files === #
file(GLOB INSPRO_HEADERS "${INSPRO_SOURCE_DIR}/*.hpp")
file(GLOB INSPRO_SOURCES "${INSPRO_SOURCE_DIR}/*.cpp")
# === Source groups === #
source_group("Main" FILES ${INSPRO_HEADERS} ${INSPRO_SOURCES})

# === Create inspro project === #
add_executable(${INSPRO_NAME} 
	${INSPRO_HEADERS} ${INSPRO_SOURCES}
	${INSPRO_UTIL_HEADERS} ${INSPRO_UTIL_SOURCES})

# === CXX FLAGS === #
set (CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
set (CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} /MT")

# === Include directories === #
target_include_directories(${INSPRO_NAME} PUBLIC ${PROJECT_ROOT_DIR}/src)
target_include_directories(${INSPRO_NAME} PRIVATE ${PROJECT_ROOT_DIR}/deps/stb)

# === Link directories === #
link_directories(${PROJECT_ROOT_DIR}/lib)

# === Link libraries === #
target_link_libraries(${INSPRO_NAME} ${GLM_NAME} ${UTIL_NAME})


# === Additional properties === #
set_target_properties(${INSPRO_NAME} PROPERTIES SUFFIX ${INSPRO_EXTENSION})
set_target_properties(${INSPRO_NAME} PROPERTIES CXX_STANDARD 20)
set_target_properties(${INSPRO_NAME} PROPERTIES CXX_EXTENSIONS OFF)
set_target_properties(${INSPRO_NAME} PROPERTIES CMAKE_CXX_STANDARD_REQUIRED ON)
set_target_properties(${INSPRO_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${PROJECT_ROOT_DIR}/")

if (MSVC)
	target_compile_options(${INSPRO_NAME} PRIVATE /W4 /permissive- /MP /Gm- /WX)
	target_compile_options(${UTIL_NAME} PRIVATE /W4 /permissive- /MP /Gm- /WX)
	target_compile_options(${UTIL_NAME} PRIVATE /W0 /permissive- /MP /Gm- /WX-)
else()
	target_compile_options(${INSPRO_NAME} PRIVATE -Wall -Wextra -pedantic -Werror)
	target_compile_options(${UTIL_NAME} PRIVATE -Wall -Wextra -pedantic -Werror)
	target_compile_options(${UTIL_NAME} PRIVATE -W0)
endif()

# === Change the startup project to this project (not the CMake projects) === #
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${INSPRO_NAME})