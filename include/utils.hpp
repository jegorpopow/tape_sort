#ifndef UTILS_HPP_
#define UTILS_HPP_

#include <ctime>
#include <random>
#include <string>
#include <vector>
#include "tape.hpp"

namespace utils {
void create_tape_file(std::string const &path,
                      std::vector<std::uint32_t> const &nums);
void create_and_clear_file(std::string const &path);
std::vector<std::uint32_t> tape_dump(tape &tp);
void write_to_tape(std::vector<std::uint32_t> const &data, tape &tape);
}  // namespace utils

#endif
