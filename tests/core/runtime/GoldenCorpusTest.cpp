#include <catch2/catch_test_macros.hpp>

#include "parser/Parser.hpp"
#include "runtime/Interpreter.hpp"
#include "runtime/Io.hpp"

#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>
#include <string_view>

namespace {

std::filesystem::path sourceRoot() {
    return std::filesystem::path(CESIL_SOURCE_DIR);
}

std::filesystem::path goldenDir() {
    return sourceRoot() / "testdata" / "parity" / "golden";
}

std::filesystem::path parityDir() {
    return sourceRoot() / "testdata" / "parity";
}

std::string readFile(const std::filesystem::path& path) {
    std::ifstream in(path);
    REQUIRE(in);
    std::ostringstream buffer;
    buffer << in.rdbuf();
    return buffer.str();
}

std::string normalizeNewlines(std::string text) {
    std::string out;
    out.reserve(text.size());
    for (std::size_t i = 0; i < text.size(); ++i) {
        if (text[i] == '\r') {
            if (i + 1 < text.size() && text[i + 1] == '\n') {
                continue;
            }
            out.push_back('\n');
            continue;
        }
        out.push_back(text[i]);
    }
    return out;
}

class CaptureIoHost final : public cesil::IoHost {
   public:
    int readInt() override { return 0; }
    void writeInt(int value) override { buffer_ << value; }
    void writeString(const std::string& text) override { buffer_ << text; }
    void writeLine() override { buffer_ << '\n'; }
    std::string captured() const { return buffer_.str(); }

   private:
    std::ostringstream buffer_;
};

// Same parse → load → run path used by the CLI (`src/cli/Main.cpp`).
cesil::RunResult runLikeCli(const std::string& source, CaptureIoHost& io) {
    cesil::ParseResult parsed = cesil::Parser{}.parse(source);
    if (!parsed.ok_) {
        cesil::RunResult failed;
        failed.ok_ = false;
        failed.diagnostics_ = parsed.diagnostics_;
        return failed;
    }
    cesil::Interpreter interpreter(io);
    interpreter.load(std::move(parsed.instructions_), std::move(parsed.data_),
                     std::move(parsed.labelIndices_));
    return interpreter.run();
}

void expectGolden(const std::filesystem::path& sourcePath,
                  const std::filesystem::path& expectedOutPath, bool expectOk) {
    const auto source = readFile(sourcePath);
    const auto expected = normalizeNewlines(readFile(expectedOutPath));
    CaptureIoHost io;
    const auto ran = runLikeCli(source, io);
    CHECK(ran.ok_ == expectOk);
    CHECK(normalizeNewlines(io.captured()) == expected);
}

}  // namespace

TEST_CASE("Golden corpus — happy paths", "[runtime][golden]") {
    SECTION("hello") {
        expectGolden(goldenDir() / "hello.ces", goldenDir() / "hello.out", true);
    }
    SECTION("total") {
        expectGolden(goldenDir() / "total.ces", goldenDir() / "total.out", true);
    }
    SECTION("print-out") {
        expectGolden(goldenDir() / "print-out.ces", goldenDir() / "print-out.out", true);
    }
    SECTION("arith-jizero (SUBTRACT MULTIPLY JIZERO)") {
        expectGolden(goldenDir() / "arith-jizero.ces", goldenDir() / "arith-jizero.out", true);
    }
}

TEST_CASE("Golden corpus — runtime error banners", "[runtime][golden]") {
    SECTION("no HALT") {
        expectGolden(parityDir() / "runtime-no-halt.ces", goldenDir() / "error-no-halt.out", false);
    }
    SECTION("IN exhaustion") {
        expectGolden(parityDir() / "runtime-in-exhaust.ces", goldenDir() / "error-in-exhaust.out",
                     false);
    }
    SECTION("division by zero") {
        expectGolden(parityDir() / "runtime-divzero.ces", goldenDir() / "error-divzero.out", false);
    }
}

TEST_CASE("Golden corpus — CLI path smoke", "[runtime][golden][cli]") {
    // Exercises the same Parser + Interpreter wiring as `cesil` run mode.
    const auto source = readFile(goldenDir() / "hello.ces");
    CaptureIoHost io;
    const auto ran = runLikeCli(source, io);
    REQUIRE(ran.ok_);
    CHECK(normalizeNewlines(io.captured()) == "Hello\n");
}
