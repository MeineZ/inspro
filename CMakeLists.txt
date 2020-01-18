
cmake_minimum_required(VERSION 3.14)

# === Output directories === #
set(PROJECT_ROOT_DIR ${CMAKE_SOURCE_DIR})
set(PROJECT_SOURCE_DIR ${PROJECT_ROOT_DIR}/src)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_ROOT_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_ROOT_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_ROOT_DIR}/bin)

set(INSPRO_DIR ${PROJECT_SOURCE_DIR}/inspro)

# === Configuration === #
set(PROJECT_NAME "InsproProject")
set(INSPRO_EXTENSION ".exe")
set(INSPRO_NAME "Inspro")

# === Display Init Info === #
message("\n")
message(STATUS "Project root dir:\t\t" ${PROJECT_ROOT_DIR})
message(STATUS "Project source dir:\t\t" ${PROJECT_SOURCE_DIR})
message(STATUS "CMake archive output dir:\t" ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})
message(STATUS "CMake library output dir:\t" ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
message(STATUS "CMake runtime output dir:\t" ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} "\n\n")

# === Create solution === #
project(${PROJECT_NAME})

######################### INSPRO #############################
# === Files === #
file(GLOB INSPRO_HEADERS "${INSPRO_DIR}/*.hpp")
file(GLOB INSPRO_SOURCES "${INSPRO_DIR}/*.cpp")

# === Source groups === #
source_group("Main" FILES ${INSPRO_HEADERS} ${INSPRO_SOURCES})

# === Create inspro project === #
add_executable(${INSPRO_NAME} 
	${INSPRO_HEADERS} ${INSPRO_SOURCES})

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
target_link_libraries(${INSPRO_NAME} ${GLM_NAME} ${UTIL_NAME} ${KRRT_NAME})

# === Additional properties === #
set_target_properties(${INSPRO_NAME} PROPERTIES SUFFIX ${INSPRO_EXTENSION})
set_target_properties(${INSPRO_NAME} PROPERTIES CXX_STANDARD 20)
set_target_properties(${INSPRO_NAME} PROPERTIES CXX_EXTENSIONS OFF)
set_target_properties(${INSPRO_NAME} PROPERTIES CMAKE_CXX_STANDARD_REQUIRED ON)
set_target_properties(${INSPRO_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${PROJECT_ROOT_DIR}/")

if (MSVC)
	target_compile_options(${INSPRO_NAME} PRIVATE /W4 /permissive- /MP /Gm- /WX)
else()
	target_compile_options(${INSPRO_NAME} PRIVATE -Wall -Wextra -pedantic -Werror)
endif()

# === Change the startup project to this project (not the CMake projects) === #
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${INSPRO_NAME})