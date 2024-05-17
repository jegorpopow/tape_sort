#ifndef ABSTRACT_TAPE_HPP_
#define ABSTRACT_TAPE_HPP_

#include <cstdint>

/* Interface of `tape` storage */

struct tape {
    [[nodiscard]] virtual std::uint32_t read() = 0;
    virtual void write(std::uint32_t value) = 0;
    virtual bool shift_right() = 0;
    virtual bool shift_left() = 0;
    virtual void rewind(std::size_t offset) = 0;
    virtual bool is_last_cell() = 0;
    virtual std::size_t get_offset() = 0;
    virtual std::uint32_t read_advance() {
        // It may have more efficient overloads in derived classes
        std::uint32_t result = read();
        shift_right();
        return result;
    }
    virtual void write_advance(std::uint32_t value) {
        // It may have more efficient overloads in derived classes
        write(value);
        shift_right();
    }
    virtual ~tape() = default;
};

#endif