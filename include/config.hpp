#ifndef CONFIG_HPP_
#define CONFIG_HPP_

#include <optional>
#include <utility>

struct config {
    std::size_t ram_size;
    int read_delay;
    int write_delay;
    int shift_delay;
    int rewind_delay;
};

std::optional<config> parse_config(std::string const &path);

#endif
