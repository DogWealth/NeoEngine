file(GLOB_RECURSE SRC src/*.h src/*.cpp)

add_library(NeoEngine SHARED ${SRC})

SET_TARGET_PROPERTIES(NeoEngine PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/TestBed)

add_definitions(-DNEO_EXPORT -DNEO_DEBUG -DNEO_PLATFORM_WINDOWS)
target_link_libraries(NeoEngine PUBLIC glfw Vulkan STB)
target_include_directories(NeoEngine PUBLIC ../3rdLibs/glm)

target_precompile_headers(NeoEngine PUBLIC src/pch.h)