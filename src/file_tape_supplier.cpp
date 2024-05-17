#include "file_tape_supplier.hpp"
#include <ctime>
#include <fstream>
#include "file_tape.hpp"

file_tape_supplier::file_tape_supplier(std::string directory_path)
    : m_directory_path(directory_path),
      m_salt_gen(std::time(nullptr)),
      m_counter(0) {
}

std::unique_ptr<tape> file_tape_supplier::new_tape() {
    std::string new_tape_path = m_directory_path + std::to_string(m_counter) +
                                std::to_string(m_salt_gen());
    m_counter++;
    // creating new file
    std::ofstream new_tape_file(new_tape_path);
    new_tape_file.close();

    return std::make_unique<file_tape>(new_tape_path);
}
