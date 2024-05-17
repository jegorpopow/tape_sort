#include "file_tape.hpp"
#include <iostream>

file_tape::file_tape(std::string const &path)
    : m_file(path,
             std::ios_base::binary | std::ios_base::in | std::ios_base::out),
      m_offset(0) {
}

std::uint32_t file_tape::read() {
    std::uint32_t result;
    m_file.read(reinterpret_cast<char *>(&result), sizeof(result));
    m_file.seekg(-sizeof(result), std::ios_base::cur);
    return result;
}

void file_tape::write(std::uint32_t value) {
    m_file.write(reinterpret_cast<char *>(&value), sizeof(value));
    m_file.seekg(-sizeof(value), std::ios_base::cur);
}

bool file_tape::shift_left() {
    if (m_offset == 0) {
        return false;
    }
    --m_offset;
    m_file.seekg(-sizeof(std::uint32_t), std::ios_base::cur);
    return true;
}

bool file_tape::shift_right() {
    if (is_last_cell()) {
        return false;
    }
    ++m_offset;
    m_file.seekg(sizeof(std::uint32_t), std::ios_base::cur);
    return true;
}

bool file_tape::is_last_cell() {
    return m_file.peek() == EOF;
}

std::uint32_t file_tape::read_advance() {
    std::uint32_t result;
    m_file.read(reinterpret_cast<char *>(&result), sizeof(result));

    m_offset++;

    return result;
}

std::size_t file_tape::get_offset() {
    return m_offset;
}

void file_tape::write_advance(std::uint32_t value) {
    m_file.write(reinterpret_cast<char *>(&value), sizeof(value));
    m_offset++;
}

void file_tape::rewind(std::size_t offset) {
    m_offset = offset;
    m_file.clear();
    m_file.seekg(sizeof(std::uint32_t) * m_offset, std::ios_base::beg);
    m_file.seekp(sizeof(std::uint32_t) * m_offset, std::ios_base::beg);
}
