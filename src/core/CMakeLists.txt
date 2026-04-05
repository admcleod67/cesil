add_library(cesil-core
    lexer/Lexer.cpp
    lexer/TokenUtils.cpp
    parser/Parser.cpp
    parser/InstructionRules.cpp
    parser/Validator.cpp
    runtime/Opcode.cpp
    runtime/Interpreter.cpp
    errors/Diagnostic.cpp
)

target_include_directories(cesil-core
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_features(cesil-core PUBLIC cxx_std_20)
