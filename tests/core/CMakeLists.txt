add_executable(cesil_tests
    lexer/LexerTest.cpp
    parser/ParserTest.cpp
    parser/SemanticsTest.cpp
    runtime/SmokeTest.cpp
)

target_link_libraries(cesil_tests PRIVATE cesil-core Catch2::Catch2WithMain)
target_compile_features(cesil_tests PRIVATE cxx_std_20)

add_test(NAME cesil_tests COMMAND cesil_tests)
