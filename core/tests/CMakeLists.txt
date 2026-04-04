add_executable(cesil_tests
    smoke_test.cpp
    lexer_test.cpp
    parser_test.cpp
    semantics_test.cpp
)

target_link_libraries(cesil_tests PRIVATE cesil-core Catch2::Catch2WithMain)
target_compile_features(cesil_tests PRIVATE cxx_std_20)

add_test(NAME cesil_tests COMMAND cesil_tests)
