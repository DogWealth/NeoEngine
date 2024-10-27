add_executable(TestBed src/main.cpp
)
add_definitions(-DNIMPORT -DDEBUG)
target_link_libraries(TestBed PUBLIC NeoEngine)
#target_include_directories(TestBed PUBLIC ../NeoEngine/Engine/src)