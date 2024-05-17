#ifndef SORTER_HPP_
#define SORTER_HPP_

#include "tape_supplier.hpp"
#include "config.hpp"
#include "tape.hpp"

struct sorter {
private:
    config m_config;

public:
    explicit sorter(config config_);
    void sort_in_ram(tape &src,
                     tape &dest,
                     tape_supplier &tape_supplier);
    void balanced_merge_sort(tape &src,
                             tape &dest,
                             tape_supplier &tape_supplier,
                             std::size_t tapes_count = 16);
};

#endif