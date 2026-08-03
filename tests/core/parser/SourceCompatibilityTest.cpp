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

}  // namespace

TEST_CASE("Source compatibility gate fixtures", "[parser][parity]") {
    cesil::Parser parser;

    SECTION("gate-unsigned-constant") {
        const auto r = parser.parse(readFixture("gate-unsigned-constant.ces"));
        CHECK(r.ok_);
        CHECK(r.diagnostics_.empty());
    }

    SECTION("gate-star-comment") {
        const auto r = parser.parse(readFixture("gate-star-comment.ces"));
        CHECK(r.ok_);
        CHECK(r.diagnostics_.empty());
    }

    SECTION("gate-paren-comment") {
        const auto r = parser.parse(readFixture("gate-paren-comment.ces"));
        CHECK(r.ok_);
        CHECK(r.diagnostics_.empty());
    }

    SECTION("gate-data-no-star") {
        const auto r = parser.parse(readFixture("gate-data-no-star.ces"));
        CHECK(r.ok_);
        REQUIRE(r.data_.size() == 2);
        CHECK(r.data_[0] == 1);
        CHECK(r.data_[1] == 2);
    }

    SECTION("gate-data-with-star") {
        const auto r = parser.parse(readFixture("gate-data-with-star.ces"));
        CHECK(r.ok_);
        REQUIRE(r.data_.size() == 2);
        CHECK(r.data_[0] == 1);
        CHECK(r.data_[1] == 2);
    }
}

TEST_CASE("Store names remain case-sensitive", "[parser][parity][runtime]") {
    const auto parsed = cesil::Parser{}.parse(readFixture("probe-case-fold.ces"));
    REQUIRE(parsed.ok_);

    CaptureIoHost io;
    cesil::Interpreter interpreter(io);
    interpreter.load(parsed.instructions_, parsed.data_, parsed.labelIndices_);
    const cesil::RunResult ran = interpreter.run();
    REQUIRE(ran.ok_);
    CHECK(io.captured() == "0\n");
}
