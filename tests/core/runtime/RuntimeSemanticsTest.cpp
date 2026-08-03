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

std::filesystem::path parityDir() {
    return std::filesystem::path(CESIL_SOURCE_DIR) / "testdata" / "parity";
}

std::string readFixture(std::string_view name) {
    const auto path = parityDir() / std::string(name);
    std::ifstream in(path);
    REQUIRE(in);
    std::ostringstream buffer;
    buffer << in.rdbuf();
    return buffer.str();
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

cesil::RunResult runParsed(cesil::ParseResult parsed, CaptureIoHost& io) {
    cesil::Interpreter interpreter(io);
    interpreter.load(std::move(parsed.instructions_), std::move(parsed.data_),
                     std::move(parsed.labelIndices_));
    return interpreter.run();
}

}  // namespace

TEST_CASE("Runtime semantics — unset variable reads as zero", "[runtime][parity]") {
    const auto parsed = cesil::Parser{}.parse(readFixture("probe-unset-var.ces"));
    REQUIRE(parsed.ok_);
    CaptureIoHost io;
    const auto ran = runParsed(parsed, io);
    REQUIRE(ran.ok_);
    CHECK(io.captured() == "0\n");
}

TEST_CASE("Runtime semantics — PRINT/OUT adjacency", "[runtime][parity]") {
    const auto parsed = cesil::Parser{}.parse(readFixture("smoke-print-out.ces"));
    REQUIRE(parsed.ok_);
    CaptureIoHost io;
    const auto ran = runParsed(parsed, io);
    REQUIRE(ran.ok_);
    CHECK(io.captured() == "Hi1\n");
}

TEST_CASE("Runtime semantics — no HALT at end of program", "[runtime][parity]") {
    const auto parsed = cesil::Parser{}.parse(readFixture("runtime-no-halt.ces"));
    REQUIRE(parsed.ok_);
    CaptureIoHost io;
    const auto ran = runParsed(parsed, io);
    CHECK_FALSE(ran.ok_);
    CHECK(io.captured().find("** ERROR: No HALT at end of program") != std::string::npos);
    CHECK(io.captured().find("5\n") != std::string::npos);
}

TEST_CASE("Runtime semantics — Jacobs runtime banners", "[runtime][parity]") {
    SECTION("IN exhaustion") {
        const auto parsed = cesil::Parser{}.parse(readFixture("runtime-in-exhaust.ces"));
        REQUIRE(parsed.ok_);
        CaptureIoHost io;
        const auto ran = runParsed(parsed, io);
        CHECK_FALSE(ran.ok_);
        CHECK(io.captured().find("** ERROR: Attempt to read more data than was provided") !=
              std::string::npos);
    }

    SECTION("division by zero") {
        const auto parsed = cesil::Parser{}.parse(readFixture("runtime-divzero.ces"));
        REQUIRE(parsed.ok_);
        CaptureIoHost io;
        const auto ran = runParsed(parsed, io);
        CHECK_FALSE(ran.ok_);
        CHECK(io.captured().find("** ERROR: Attempted division by zero") != std::string::npos);
    }
}

TEST_CASE("Runtime semantics — DIVIDE toward zero", "[runtime][parity]") {
    const auto parsed = cesil::Parser{}.parse(readFixture("runtime-divide-neg.ces"));
    REQUIRE(parsed.ok_);
    CaptureIoHost io;
    const auto ran = runParsed(parsed, io);
    REQUIRE(ran.ok_);
    CHECK(io.captured() == "-3\n");
}

TEST_CASE("Runtime semantics — OUT multi-digit and negative", "[runtime][parity]") {
    const auto parsed = cesil::Parser{}.parse(readFixture("runtime-outdigits.ces"));
    REQUIRE(parsed.ok_);
    CaptureIoHost io;
    const auto ran = runParsed(parsed, io);
    REQUIRE(ran.ok_);
    CHECK(io.captured() == "n=42 m=-3\n");
}

TEST_CASE("Runtime semantics — label length max six", "[runtime][parity]") {
    const auto parsed = cesil::Parser{}.parse(readFixture("runtime-long-label.ces"));
    CHECK_FALSE(parsed.ok_);
    bool sawLabelDiag = false;
    for (const auto& d : parsed.diagnostics_) {
        if (d.message_.find("label") != std::string::npos ||
            d.message_.find("jump target") != std::string::npos) {
            sawLabelDiag = true;
            break;
        }
    }
    CHECK(sawLabelDiag);
}

TEST_CASE("Runtime semantics — overflow uses host int", "[runtime][parity]") {
    const auto parsed = cesil::Parser{}.parse(readFixture("runtime-overflow.ces"));
    REQUIRE(parsed.ok_);
    CaptureIoHost io;
    const auto ran = runParsed(parsed, io);
    REQUIRE(ran.ok_);
    // 8388607 * 2 = 16777214 on host int (beyond classic 24-bit max magnitude).
    CHECK(io.captured() == "16777214\n");
}
