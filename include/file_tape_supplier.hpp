#ifndef FILE_TAPE_SUPPLIER_HPP_
#define FILE_TAPE_SUPPLIER_HPP_

#include <random>
#include <string>
#include "tape_supplier.hpp"

struct file_tape_supplier : tape_supplier {
private:
    std::string m_directory_path;
    std::mt19937 m_salt_gen;
    std::size_t m_counter;

public:
    explicit file_tape_supplier(std::string directory_path);
    std::unique_ptr<tape> new_tape() override;
};

#endif