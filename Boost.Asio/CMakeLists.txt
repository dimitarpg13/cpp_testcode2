SET(PROGNAME myprog)

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
find_package(Boost 1.65.1 COMPONENTS date_time regex thread serialization system log log_setup program_options graph REQUIRED) 

if(Boost_FOUND)
   include_directories(${Boost_INCLUDE_DIRS}) 
   add_executable(${PROGNAME} main.cpp) 
   target_link_libraries(${PROGNAME} ${Boost_LIBRARIES})
endif()
