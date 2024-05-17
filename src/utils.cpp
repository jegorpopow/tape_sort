#include "utils.hpp"
#include <fstream>
namespace utils {
void create_tape_file(std::string const &path,
                      std::vector<uint32_t> const &nums) {
    std::ofstream f(path);
    f.write(reinterpret_cast<const char *>(nums.data()),
            nums.size() * sizeof(nums.front()));
    f.close();
}

void create_and_clear_file(std::string const &path) {
    std::ofstream f(path);
    f.close();
}

std::vector<std::uint32_t> tape_dump(tape &tp) {
    tp.rewind(0);
    std::vector<std::uint32_t> result;
    while (!tp.is_last_cell()) {
        result.push_back(tp.read_advance());
    }
    return result;
}

void write_to_tape(std::vector<std::uint32_t> const &data, tape &tp) {
    for (std::uint32_t val : data) {
        tp.write_advance(val);
    }
}
}  // namespace utils