cmake_minimum_required(VERSION 3.10)
project(mymath)

macro (add_sources)
    file (RELATIVE_PATH _relPath "${PROJECT_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}")
    foreach (_src ${ARGN})
        if (_relPath)
            list (APPEND SRCS "${_relPath}/${_src}")
        else()
            list (APPEND SRCS "${_src}")
        endif()
    endforeach()
    if (_relPath)
        # propagate SRCS to parent directory
        set (SRCS ${SRCS} PARENT_SCOPE)
    endif()
endmacro()


option(USE_CLANG "build application with clang" ON)

if(USE_CLANG)
SET (CMAKE_C_FLAGS_INIT                "-Wall -std=c++14")
SET (CMAKE_C_FLAGS_DEBUG_INIT          "-g")
SET (CMAKE_C_FLAGS_MINSIZEREL_INIT     "-Os -DNDEBUG")
SET (CMAKE_C_FLAGS_RELEASE_INIT        "-O4 -DNDEBUG")
SET (CMAKE_C_FLAGS_RELWITHDEBINFO_INIT "-O2 -g")

SET (CMAKE_CXX_FLAGS_INIT                "-Wall")
SET (CMAKE_CXX_FLAGS_DEBUG_INIT          "-g")
SET (CMAKE_CXX_FLAGS_MINSIZEREL_INIT     "-Os -DNDEBUG")
SET (CMAKE_CXX_FLAGS_RELEASE_INIT        "-O4 -DNDEBUG")
SET (CMAKE_CXX_FLAGS_RELWITHDEBINFO_INIT "-O2 -g")
SET (CMAKE_TOOLCHAIN_PREFIX              "llvm-")
endif(USE_CLANG)

set(Boost_USE_STATIC_LIBS ON) 
set(Boost_USE_MULTITHREADED ON)  
set(Boost_USE_STATIC_RUNTIME ON) 
find_package(Boost 1.69 COMPONENTS date_time regex thread serialization system log log_setup program_options graph REQUIRED) 

find_package(PythonInterp 3.5 REQUIRED)

# this goes after since it uses PythonInterp as hint
find_package(PythonLibs 3.5 REQUIRED)

# this comes to hand if we also need to use the NumPy C API
exec_program(${PYTHON_EXECUTABLE}
     ARGS "-c \"import numpy; print(numpy.get_include())\""
     OUTPUT_VARIABLE_NUMPY_INCLUDE_DIR
     RETURN_VALUE NUMPY_NOT_FOUND
    )
if(NUMPY_NOT_FOUND)
    message(FATAL_ERROR "NumPy headers not found")
endif()

add_sources(mymath.cc)

add_library(mymath SHARED ${SRCS})

set_target_properties(
    mymath
    PROPERTIES
        PREFIX ""
        OUTPUT_NAME "mymath"
        LINKER_LANGUAGE C
    )

