add_library(cesil-core
    src/opcode.cpp
    src/diagnostic.cpp
        src/lexer/lexer.cpp
    src/lexer/token_utils.cpp
        src/parser/parser.cpp
    src/parser/instruction_rules.cpp
    src/semantics/validator.cpp
        src/interpreter/interpreter.cpp
)

target_include_directories(cesil-core
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_features(cesil-core PUBLIC cxx_std_20)
