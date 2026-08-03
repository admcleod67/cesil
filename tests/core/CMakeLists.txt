add_executable(cesil_tests
    lexer/LexerTest.cpp
    parser/ParserTest.cpp
    parser/SemanticsTest.cpp
    parser/DiagnosticAccumulationTest.cpp
    parser/SourceCompatibilityTest.cpp
    runtime/SmokeTest.cpp
)

target_link_libraries(cesil_tests PRIVATE cesil-core Catch2::Catch2WithMain)
target_compile_features(cesil_tests PRIVATE cxx_std_20)
target_compile_definitions(cesil_tests PRIVATE
    CESIL_SOURCE_DIR="${CMAKE_SOURCE_DIR}"
)

add_test(NAME cesil_tests COMMAND cesil_tests)
