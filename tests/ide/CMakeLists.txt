qt_add_executable(cesil_ide_tests
    DiagnosticUtilsTest.cpp
    ../../src/ide/DiagnosticUtils.cpp
    ../../src/ide/DiagnosticUtils.hpp
)

target_include_directories(cesil_ide_tests PRIVATE
    "${CMAKE_SOURCE_DIR}/src/ide"
)

target_link_libraries(cesil_ide_tests PRIVATE
    Qt6::Test
    Qt6::Widgets
    cesil-core
)

target_compile_features(cesil_ide_tests PRIVATE cxx_std_20)

add_test(NAME cesil_ide_tests COMMAND cesil_ide_tests)

qt_add_executable(cesil_ide_edit_actions_tests
    SourceEditActionsTest.cpp
    ../../src/ide/SourceEditActions.cpp
    ../../src/ide/SourceEditActions.hpp
)

target_include_directories(cesil_ide_edit_actions_tests PRIVATE
    "${CMAKE_SOURCE_DIR}/src/ide"
)

target_link_libraries(cesil_ide_edit_actions_tests PRIVATE
    Qt6::Test
    Qt6::Widgets
)

target_compile_features(cesil_ide_edit_actions_tests PRIVATE cxx_std_20)

add_test(NAME cesil_ide_edit_actions_tests COMMAND cesil_ide_edit_actions_tests)
