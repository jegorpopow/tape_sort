#include "config.hpp"
#include <fstream>

std::optional<config> parse_config(std::string const &path) {
    std::fstream src(path);
    config result;

    if (src >> result.ram_size >> result.read_delay >> result.write_delay >>
        result.shift_delay >> result.rewind_delay) {
        return result;
    } else {
        return std::nullopt;
    }
}