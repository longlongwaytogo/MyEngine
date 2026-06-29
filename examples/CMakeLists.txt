add_executable(triangle
    triangle/main.cpp
)

target_link_libraries(triangle
    PRIVATE
        gfx
)

add_executable(model_viewer
    model_viewer/main.cpp
)

target_link_libraries(model_viewer
    PRIVATE
        gfx
)
