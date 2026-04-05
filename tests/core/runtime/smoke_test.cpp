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
    int read_int() override { return 0; }
    void write_int(int value) override { buffer_ << value; }
    void write_string(const std::string& text) override { buffer_ << text; }
    void write_line() override { buffer_ << '\n'; }
    std::string captured() const { return buffer_.str(); }

   private:
    std::ostringstream buffer_;
};

class NullIoHost final : public cesil::IoHost {
   public:
    int read_int() override { return 0; }
    void write_int(int) override {}
    void write_string(const std::string&) override {}
    void write_line() override {}
};

}  // namespace

TEST_CASE("Smoke tests and integration", "[smoke]") {
    cesil::Parser parser;

    SECTION("Empty and whitespace programs") {
        const cesil::ParseResult blank = parser.parse("");
        CHECK(blank.ok);
        CHECK(blank.instructions.empty());
        CHECK(blank.diagnostics.empty());

        const cesil::ParseResult spaces = parser.parse("   \n\t  ");
        CHECK(spaces.ok);
        CHECK(spaces.instructions.empty());
    }

    SECTION("Simple program execution") {
        cesil::ParseResult load_line = parser.parse("LOAD +1\nHALT\n");
        REQUIRE(load_line.ok);
        CHECK(load_line.instructions.size() == 2);

        NullIoHost null_io;
        cesil::Interpreter interpreter(null_io);
        interpreter.load(std::move(load_line.instructions), {}, std::move(load_line.label_indices));
        const cesil::RunResult ran = interpreter.run();
        CHECK(ran.ok);
        CHECK(interpreter.accumulator() == 1);
    }

    SECTION("Invalid program") {
        const cesil::ParseResult bad = parser.parse("NOTANINSTRUCTION");
        CHECK_FALSE(bad.ok);
        CHECK_FALSE(bad.diagnostics.empty());
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
        REQUIRE(total.ok);
        CaptureIoHost cap;
        cesil::Interpreter run_total(cap);
        run_total.load(std::move(total.instructions), std::move(total.data),
                       std::move(total.label_indices));
        const cesil::RunResult wiki_run = run_total.run();
        CHECK(wiki_run.ok);
        CHECK(cap.captured() == "The total is: 6\n");
    }
}
