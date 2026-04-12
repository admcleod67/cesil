add_executable(cesil-cli
        Main.cpp
)

target_link_libraries(cesil-cli PRIVATE cesil-core)
target_compile_features(cesil-cli PRIVATE cxx_std_20)

set_target_properties(cesil-cli PROPERTIES
    OUTPUT_NAME cesil
)
