#pragma once

#include <string>

namespace cesil {

/// Host-provided input/output for program execution (no iostream in the interface).
///
/// Front-ends (CLI, IDE, simulator) implement this to supply data and sinks.
class IoHost {
   public:
    virtual ~IoHost() = default;

    /// Read the next integer from program input.
    /// \return The value read; implementations may block.
    virtual int readInt() = 0;

    /// Write integer \p value to program output.
    virtual void writeInt(int value) = 0;

    /// Write \p text without an implicit newline.
    virtual void writeString(const std::string& text) = 0;

    /// Write a newline to program output.
    virtual void writeLine() = 0;
};

}  // namespace cesil
