file(GLOB_RECURSE SOURCE_FILES src/*.cpp)
file(GLOB_RECURSE LOCAL_HEADER_FILES hdr/*.*)

set(HEADER_FILES
	${HEADER_FILES}
	${LOCAL_HEADER_FILES}
	PARENT_SCOPE)

add_executable(console ${SOURCE_FILES} ${PLATFORM_SOURCE_FILES} ${LOCAL_HEADER_FILES} ${HEADER_FILES})
target_include_directories(console PUBLIC
	"${PROJECT_SOURCE_DIR}/include"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/hdr>"
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>")
set_property(TARGET console PROPERTY CXX_STANDARD 20)
set_property(TARGET console PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
set_property(TARGET console PROPERTY CXX_VISIBILITY_PRESET hidden)
set_property(TARGET console PROPERTY SOVERSION ${PROJECT_VERSION_MAJOR})
set_property(TARGET console PROPERTY VERSION ${PROJECT_VERSION})

# dependencies
target_link_libraries(console PUBLIC Threads::Threads)
if (WIN32)
	set(Boost_USE_STATIC_LIBS   ON)
	set(Boost_USE_STATIC_RUNTIME ON)
endif()

target_link_libraries(console PRIVATE Boost::system)
target_link_libraries(console PRIVATE OpenSSL::SSL)
target_link_libraries(console PRIVATE ZLIB::ZLIB)
target_link_libraries(console PRIVATE neolib$<$<CONFIG:Debug>:d>)


