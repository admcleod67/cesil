#include <errors/Diagnostic.hpp>
#include <runtime/Interpreter.hpp>
#include <runtime/Io.hpp>
#include <parser/Parser.hpp>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <string_view>

namespace {

/// Console-backed \ref cesil::IoHost for the CLI.
class ConsoleIoHost final : public cesil::IoHost {
   public:
    int readInt() override {
        int value = 0;
        if (!(std::cin >> value)) {
            return 0;
        }
        return value;
    }

    void writeInt(int value) override { std::cout << value; }

    void writeString(const std::string& text) override { std::cout << text; }

    void writeLine() override { std::cout << '\n'; }
};

void printUsage(const char* argv0) {
    std::cerr << "Usage: " << argv0 << " run <file>\n"
              << "       " << argv0 << " --version\n";
}

bool readFile(const std::string& path, std::string& out, std::string& error) {
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
    if (argc == 2) {
        const std::string_view flag = argv[1];
        if (flag == "--version" || flag == "-v") {
            std::cout << "cesil " << CESIL_VERSION << '\n';
            return 0;
        }
    }

    if (argc != 3 || std::string_view(argv[1]) != "run") {
        printUsage(argc > 0 ? argv[0] : "cesil");
        return 1;
    }

    const std::string path = argv[2];
    std::string source;
    std::string ioError;
    if (!readFile(path, source, ioError)) {
        std::cerr << "error: " << ioError << '\n';
        return 1;
    }

    cesil::Parser parser;
    cesil::ParseResult parsed = parser.parse(source);
    for (const auto& d : parsed.diagnostics_) {
        cesil::printDiagnostic(std::cerr, d);
    }
    if (!parsed.ok_) {
        return 1;
    }

    ConsoleIoHost io;
    cesil::Interpreter interpreter(io);
    interpreter.load(std::move(parsed.instructions_), std::move(parsed.data_),
                     std::move(parsed.labelIndices_));

    const cesil::RunResult ran = interpreter.run();
    for (const auto& d : ran.diagnostics_) {
        cesil::printDiagnostic(std::cerr, d);
    }
    if (!ran.ok_) {
        return 1;
    }

    return 0;
}
