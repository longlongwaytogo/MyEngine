add_executable(model_viewer_example
    main.cpp
)

target_include_directories(model_viewer_example
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/../../include
)

target_link_libraries(model_viewer_example
    PRIVATE
        gfx
)

set_target_properties(model_viewer_example
    PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/examples
)
