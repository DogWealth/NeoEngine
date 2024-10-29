file(GLOB_RECURSE SRC src/*.h src/*.cpp)

add_executable(TestBed ${SRC})
add_definitions(-DNIMPORT -DDEBUG)
target_link_libraries(TestBed PUBLIC NeoEngine)
#target_include_directories(TestBed PUBLIC ../NeoEngine/Engine/src)