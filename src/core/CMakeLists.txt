add_library(cesil-core
    lexer/lexer.cpp
    lexer/token_utils.cpp
    parser/parser.cpp
    parser/instruction_rules.cpp
    parser/validator.cpp
    runtime/opcode.cpp
    runtime/interpreter.cpp
    errors/diagnostic.cpp
)

target_include_directories(cesil-core
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_features(cesil-core PUBLIC cxx_std_20)
