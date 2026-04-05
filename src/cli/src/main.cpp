#include <cesil/diagnostic.hpp>
#include <cesil/interpreter/interpreter.hpp>
#include <cesil/io.hpp>
#include <cesil/parser/parser.hpp>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <string_view>

namespace {

/// Console-backed \ref cesil::IoHost for the CLI.
class ConsoleIoHost final : public cesil::IoHost {
   public:
    int read_int() override {
        int value = 0;
        if (!(std::cin >> value)) {
            return 0;
        }
        return value;
    }

    void write_int(int value) override { std::cout << value; }

    void write_string(const std::string& text) override { std::cout << text; }

    void write_line() override { std::cout << '\n'; }
};

void print_usage(const char* argv0) {
    std::cerr << "Usage: " << argv0 << " run <file>\n";
}

bool read_file(const std::string& path, std::string& out, std::string& error) {
    std::ifstream in(path, std::ios::binary);
    if (!in) {
        error = "failed to open file: " + path;
        return false;
    }
    std::ostringstream buffer;
    buffer << in.rdbuf();
    out = buffer.str();
    return true;
}

}  // namespace

int main(int argc, char* argv[]) {
    if (argc != 3 || std::string_view(argv[1]) != "run") {
        print_usage(argc > 0 ? argv[0] : "cesil");
        return 1;
    }

    const std::string path = argv[2];
    std::string source;
    std::string io_error;
    if (!read_file(path, source, io_error)) {
        std::cerr << "error: " << io_error << '\n';
        return 1;
    }

    cesil::Parser parser;
    cesil::ParseResult parsed = parser.parse(source);
    for (const auto& d : parsed.diagnostics) {
        cesil::print_diagnostic(std::cerr, d);
    }
    if (!parsed.ok) {
        return 1;
    }

    ConsoleIoHost io;
    cesil::Interpreter interpreter(io);
    interpreter.load(std::move(parsed.instructions), std::move(parsed.data),
                     std::move(parsed.label_indices));

    const cesil::RunResult ran = interpreter.run();
    for (const auto& d : ran.diagnostics) {
        cesil::print_diagnostic(std::cerr, d);
    }
    if (!ran.ok) {
        return 1;
    }

    return 0;
}
