#
# Set the binary executable name
#
set(project_BIN ${PROJECT_NAME})

#
# Find all executables
#
file(GLOB project_SRCS *.cpp *.h)

#
# Add executables to project
#
add_executable(${project_BIN} ${project_SRCS})

target_link_libraries(${project_BIN}
    PRIVATE io2d
    PUBLIC pugixml
)

if( ${CMAKE_SYSTEM_NAME} MATCHES "Linux" )
    target_link_libraries(${project_BIN} PUBLIC pthread)
endif()

if(MSVC)
	target_compile_options(${project_BIN} PUBLIC /D_SILENCE_CXX17_ALLOCATOR_VOID_DEPRECATION_WARNING /wd4459)
endif()
