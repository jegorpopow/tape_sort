#ifndef FILE_TAPE_HPP_
#define FILE_TAPE_HPP_

#include <fstream>
#include <string>
#include "tape.hpp"

struct file_tape : tape {
private:
    std::fstream m_file;
    std::size_t m_offset;

public:
    explicit file_tape(std::string const &path);
    [[nodiscard]] std::uint32_t read() override;
    void write(std::uint32_t value) override;
    void shift_right() override;
    void shift_left() override;
    bool is_last_cell() override;
    std::uint32_t read_advance() override;
    void write_advance(std::uint32_t value) override;
    std::size_t get_offset() override;
    void rewind(std::size_t offset) override;
};

#endif