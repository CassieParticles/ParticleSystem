# CMakeList.txt : CMake project for ParticleSystem, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)


project ("ParticleSystem")

list(APPEND Game "src/Particles/ParticleManager.cpp")
list(APPEND Game "src/Particles/ParticleRenderer.cpp")
list(APPEND Game "src/Particles/ParticleEmitter.cpp")

list(APPEND Game "src/Main.cpp")

# Add source to this project's executable.
add_executable (ParticleSystem ${Game})

add_subdirectory(D3DFramework)

target_link_libraries(ParticleSystem PUBLIC D3D11Framework)

#Copy shaders into the output directory, to prevent them being copied manually
add_custom_target(copyShaders ALL
COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_LIST_DIR}/shaders" "${CMAKE_BINARY_DIR}/shaders"
COMMENT "Copied shaders to output directory" VERBATIM
)

#Copy assets into the output directory, to prevent them being copied manually
add_custom_target(copyAssets ALL
COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_LIST_DIR}/assets" "${CMAKE_BINARY_DIR}/assets"
COMMENT "Copied assets to output directory" VERBATIM
)

add_dependencies(ParticleSystem copyShaders)
add_dependencies(ParticleSystem copyAssets)

# TODO: Add tests and install targets if needed.
