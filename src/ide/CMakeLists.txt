qt_add_executable(cesil-ide
        Main.cpp
        MainWindow.cpp
        MainWindow.hpp
)

target_link_libraries(cesil-ide PRIVATE Qt6::Widgets cesil-core)
target_compile_features(cesil-ide PRIVATE cxx_std_20)

set_target_properties(cesil-ide PROPERTIES
    OUTPUT_NAME cesil-ide
)
