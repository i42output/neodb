cmake_minimum_required(VERSION 3.15)
cmake_policy(VERSION 3.15)

# meta
project(neodb VERSION 0.0.1 LANGUAGES CXX)

# deployment
IF(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  SET(CMAKE_INSTALL_PREFIX "/opt/neodb" CACHE PATH "Default intstall prefix" FORCE)
ENDIF(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)

include(GNUInstallDirs)
set(CMAKE_INSTALL_CMAKEDIR "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}")

# compiler support
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED YES)

# project
if(MSVC)
    set(CMAKE_DEBUG_POSTFIX "d")
    add_definitions(-D_UNICODE)
endif()

add_definitions(-DNEOLIB_HOSTED_ENVIRONMENT)                                                        

# dependencies
find_package(Threads REQUIRED)
find_package(Boost COMPONENTS chrono filesystem system REQUIRED)
find_package(OpenSSL REQUIRED COMPONENTS SSL)
find_package(ZLIB REQUIRED)

file(GLOB_RECURSE EXPORTED_HEADER_FILES include/*.*)
set(HEADER_FILES ${EXPORTED_HEADER_FILES})

set(CONFIG_DIR "/etc/opt/neodb" CACHE PATH "Configuration Files")

set(COMMON_LIBRARY_PATH "/usr/local/lib" CACHE PATH "Common library path")
link_directories(${COMMON_LIBRARY_PATH})

add_subdirectory(client)
add_subdirectory(server)
add_subdirectory(console)
add_subdirectory(unit_tests)

install(DIRECTORY include/neodb DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")
install(TARGETS client DESTINATION "${CMAKE_INSTALL_LIBDIR}" EXPORT client)
install(TARGETS server DESTINATION "${CMAKE_INSTALL_BINDIR}")
install(TARGETS console DESTINATION "${CMAKE_INSTALL_BINDIR}")
install(FILES "LICENSE" DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/licenses/${PROJECT_NAME}")
