add_executable(cesil_tests
    lexer/lexer_test.cpp
    parser/parser_test.cpp
    parser/semantics_test.cpp
    runtime/smoke_test.cpp
)

target_link_libraries(cesil_tests PRIVATE cesil-core Catch2::Catch2WithMain)
target_compile_features(cesil_tests PRIVATE cxx_std_20)

add_test(NAME cesil_tests COMMAND cesil_tests)
