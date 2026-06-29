add_executable(triangle_example
    main.cpp
)

target_include_directories(triangle_example
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/../../include
)

target_link_libraries(triangle_example
    PRIVATE
        gfx
)

set_target_properties(triangle_example
    PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/examples
)
