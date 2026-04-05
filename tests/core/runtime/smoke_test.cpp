#include <catch2/catch_test_macros.hpp>
#include "runtime/interpreter.hpp"
#include "runtime/io.hpp"
#include "parser/parser.hpp"

#include <sstream>
#include <string>
#include <vector>

namespace {

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

class NullIoHost final : public cesil::IoHost {
   public:
    int readInt() override { return 0; }
    void writeInt(int) override {}
    void writeString(const std::string&) override {}
    void writeLine() override {}
};

}  // namespace

TEST_CASE("Smoke tests and integration", "[smoke]") {
    cesil::Parser parser;

    SECTION("Empty and whitespace programs") {
        const cesil::ParseResult blank = parser.parse("");
        CHECK(blank.ok_);
        CHECK(blank.instructions_.empty());
        CHECK(blank.diagnostics_.empty());

        const cesil::ParseResult spaces = parser.parse("   \n\t  ");
        CHECK(spaces.ok_);
        CHECK(spaces.instructions_.empty());
    }

    SECTION("Simple program execution") {
        cesil::ParseResult loadLine = parser.parse("LOAD +1\nHALT\n");
        REQUIRE(loadLine.ok_);
        CHECK(loadLine.instructions_.size() == 2);

        NullIoHost nullIo;
        cesil::Interpreter interpreter(nullIo);
        interpreter.load(std::move(loadLine.instructions_), {}, std::move(loadLine.labelIndices_));
        const cesil::RunResult ran = interpreter.run();
        CHECK(ran.ok_);
        CHECK(interpreter.accumulator() == 1);
    }

    SECTION("Invalid program") {
        const cesil::ParseResult bad = parser.parse("NOTANINSTRUCTION");
        CHECK_FALSE(bad.ok_);
        CHECK_FALSE(bad.diagnostics_.empty());
    }

    SECTION("Wiki example - sum sequence") {
        const char* wiki = R"(
        LOAD    +0
LOOP    STORE   TOTAL
        IN
        JINEG   DONE
        ADD     TOTAL
        JUMP    LOOP

DONE    PRINT   "The total is: "
        LOAD    TOTAL
        OUT
        LINE
        HALT

%
1
2
3
-1
*
)";
        cesil::ParseResult total = parser.parse(wiki);
        REQUIRE(total.ok_);
        CaptureIoHost cap;
        cesil::Interpreter runTotal(cap);
        runTotal.load(std::move(total.instructions_), std::move(total.data_),
                      std::move(total.labelIndices_));
        const cesil::RunResult wikiRun = runTotal.run();
        CHECK(wikiRun.ok_);
        CHECK(cap.captured() == "The total is: 6\n");
    }
}
