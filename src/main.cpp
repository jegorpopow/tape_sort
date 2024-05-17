#include <cassert>
#include <iostream>
#include "config.hpp"
#include "file_tape.hpp"
#include "file_tape_supplier.hpp"
#include "sorter.hpp"

int main(int argc, char *argv[]) {
    if (argc <= 2) {
        std::cout << "usage: tape_sort <tape to sort> <destination> [optional "
                     ": algorithm 'naive' | 'merge'] [optional : config file]"
                  << std::endl;
        return 1;
    }

    std::string config_file_path = (argc > 4 ? argv[4] : ".tape-config");
    std::string algo = ((argc > 3 ? argv[3] : "naive"));

    std::optional<config> parsed_config = parse_config(config_file_path);

    if (!parsed_config.has_value()) {
        std::cout << "non existing or wrong formatted config file" << std::endl;
        return 1;
    }

    sorter tape_sorter(*parsed_config);
    file_tape_supplier supplier("tmp/");
    file_tape src(argv[1]);
    file_tape dest(argv[2]);

    if (algo == "naive") {
        tape_sorter.sort_in_ram(src, dest, supplier);
    } else if (algo == "merge") {
        tape_sorter.balanced_merge_sort(src, dest, supplier);
    } else {
        std::cout << "unknown algorithm" << std::endl;
    }

    return 0;
}