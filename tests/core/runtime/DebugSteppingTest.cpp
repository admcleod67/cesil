#include <catch2/catch_test_macros.hpp>

#include "parser/Parser.hpp"
#include "runtime/Hooks.hpp"
#include "runtime/Interpreter.hpp"
#include "runtime/Io.hpp"

#include <cstddef>
#include <sstream>
#include <string>

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

}  // namespace

TEST_CASE("Debug stepping — step through LOAD/OUT/LINE/HALT", "[runtime][debug]") {
    const auto parsed = cesil::Parser{}.parse("LOAD +7\nOUT\nLINE\nHALT\n");
    REQUIRE(parsed.ok_);
    REQUIRE(parsed.instructions_.size() == 4);

    CaptureIoHost io;
    cesil::Interpreter interpreter(io);
    interpreter.load(std::move(parsed.instructions_), {}, std::move(parsed.labelIndices_));

    CHECK(interpreter.programCounter() == 0);
    CHECK(interpreter.sourceLineAtPc() == 1);
    CHECK(interpreter.accumulator() == 0);

    auto stepped = interpreter.step();
    REQUIRE(stepped.ok_);
    CHECK_FALSE(stepped.stoppedAtBreakpoint_);
    CHECK(interpreter.accumulator() == 7);
    CHECK(interpreter.programCounter() == 1);
    CHECK(interpreter.sourceLineAtPc() == 2);

    stepped = interpreter.step();
    REQUIRE(stepped.ok_);
    CHECK(io.captured() == "7");
    CHECK(interpreter.programCounter() == 2);

    stepped = interpreter.step();
    REQUIRE(stepped.ok_);
    CHECK(io.captured() == "7\n");
    CHECK(interpreter.programCounter() == 3);
    CHECK(interpreter.sourceLineAtPc() == 4);

    stepped = interpreter.step();
    REQUIRE(stepped.ok_);
    CHECK(interpreter.programCounter() == 4);
    CHECK(interpreter.sourceLineAtPc() == 0);
}

TEST_CASE("Debug stepping — reset restores execution state", "[runtime][debug]") {
    const auto parsed = cesil::Parser{}.parse("LOAD +3\nSTORE X\nIN\nHALT\n%\n9\n*\n");
    REQUIRE(parsed.ok_);

    CaptureIoHost io;
    cesil::Interpreter interpreter(io);
    interpreter.load(std::move(parsed.instructions_), std::move(parsed.data_),
                     std::move(parsed.labelIndices_));

    REQUIRE(interpreter.step().ok_);
    REQUIRE(interpreter.step().ok_);
    REQUIRE(interpreter.step().ok_);
    CHECK(interpreter.accumulator() == 9);
    CHECK(interpreter.store().at("X") == 3);
    CHECK(interpreter.dataPointer() == 1);
    CHECK(interpreter.programCounter() == 3);

    interpreter.reset();
    CHECK(interpreter.accumulator() == 0);
    CHECK(interpreter.programCounter() == 0);
    CHECK(interpreter.store().empty());
    CHECK(interpreter.dataPointer() == 0);
    CHECK(interpreter.data().size() == 1);
    CHECK(interpreter.data()[0] == 9);
}

TEST_CASE("Debug stepping — shouldStop_ ends run cooperatively", "[runtime][debug]") {
    const auto parsed = cesil::Parser{}.parse("LOAD +1\nLOAD +2\nLOAD +3\nHALT\n");
    REQUIRE(parsed.ok_);

    CaptureIoHost io;
    std::size_t completed = 0;
    cesil::ExecutionHooks hooks;
    hooks.afterInstruction_ = [&](const cesil::Interpreter&) { ++completed; };
    hooks.shouldStop_ = [&]() { return completed >= 2; };

    cesil::Interpreter interpreter(io, hooks);
    interpreter.load(std::move(parsed.instructions_), {}, std::move(parsed.labelIndices_));

    const auto ran = interpreter.run();
    REQUIRE(ran.ok_);
    CHECK_FALSE(ran.stoppedAtBreakpoint_);
    CHECK(completed == 2);
    CHECK(interpreter.accumulator() == 2);
    CHECK(interpreter.programCounter() == 2);
}

TEST_CASE("Debug stepping — betweenInstructions_ runs during run", "[runtime][debug]") {
    const auto parsed = cesil::Parser{}.parse("LOAD +1\nLOAD +2\nHALT\n");
    REQUIRE(parsed.ok_);

    CaptureIoHost io;
    int betweenCount = 0;
    cesil::ExecutionHooks hooks;
    hooks.betweenInstructions_ = [&]() { ++betweenCount; };

    cesil::Interpreter interpreter(io, hooks);
    interpreter.load(std::move(parsed.instructions_), {}, std::move(parsed.labelIndices_));

    const auto ran = interpreter.run();
    REQUIRE(ran.ok_);
    // betweenInstructions_ after each completed non-halting instruction (LOAD, LOAD).
    CHECK(betweenCount == 2);
}

TEST_CASE("Debug stepping — data and dataPointer advance on IN via step", "[runtime][debug]") {
    const auto parsed = cesil::Parser{}.parse("IN\nHALT\n%\n42\n*\n");
    REQUIRE(parsed.ok_);

    CaptureIoHost io;
    cesil::Interpreter interpreter(io);
    interpreter.load(std::move(parsed.instructions_), std::move(parsed.data_),
                     std::move(parsed.labelIndices_));

    CHECK(interpreter.data().size() == 1);
    CHECK(interpreter.dataPointer() == 0);

    const auto stepped = interpreter.step();
    REQUIRE(stepped.ok_);
    CHECK(interpreter.accumulator() == 42);
    CHECK(interpreter.dataPointer() == 1);
}

TEST_CASE("Debug stepping — runtime error on step", "[runtime][debug]") {
    const auto parsed = cesil::Parser{}.parse("LOAD +1\nDIVIDE +0\nHALT\n");
    REQUIRE(parsed.ok_);

    CaptureIoHost io;
    cesil::Interpreter interpreter(io);
    interpreter.load(std::move(parsed.instructions_), {}, std::move(parsed.labelIndices_));

    REQUIRE(interpreter.step().ok_);
    const auto stepped = interpreter.step();
    CHECK_FALSE(stepped.ok_);
    CHECK(io.captured().find("** ERROR: Attempted division by zero") != std::string::npos);
}
