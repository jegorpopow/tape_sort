#include "sorter.hpp"
#include <algorithm>
#include <iostream>
#include <queue>
#include <utility>
#include <vector>
#include "utils.hpp"

sorter::sorter(config config_) : m_config(config_) {
}

void sorter::sort_in_ram(tape &src, tape &dest, tape_supplier &tape_supplier) {
    // split the tape to pieces, that fits RAM, sort separately, write to apart
    // tapes

    std::vector<std::unique_ptr<tape>> tapes;

    while (!src.is_last_cell()) {
        std::vector<std::uint32_t> data;
        while (!src.is_last_cell() && data.size() < m_config.ram_size) {
            data.push_back(src.read_advance());
        }

        std::sort(data.begin(), data.end());
        tapes.push_back(tape_supplier.new_tape());
        utils::write_to_tape(data, *tapes.back());
    }

    for (auto &tape : tapes) {
        tape->rewind(0);
    }

    // Merge all tapes into dest
    using pq_elem = std::pair<std::uint32_t, std::size_t>;
    std::priority_queue<pq_elem, std::vector<pq_elem>, std::greater<pq_elem>>
        queue;

    for (std::size_t i = 0; i < tapes.size(); i++) {
        if (!tapes[i]->is_last_cell()) {
            std::uint32_t val = tapes[i]->read_advance();
            queue.emplace(val, i);
        }
    }

    while (!queue.empty()) {
        auto [value, tape_index] = queue.top();
        queue.pop();
        dest.write_advance(value);

        if (!tapes[tape_index]->is_last_cell()) {
            queue.emplace(tapes[tape_index]->read_advance(), tape_index);
        }
    }
}

namespace {
void polyphase_merge(std::vector<std::unique_ptr<tape>> &lhs,
                     std::vector<std::unique_ptr<tape>> &rhs,
                     std::vector<std::size_t> &lhs_size,
                     std::vector<std::size_t> &rhs_size,
                     std::size_t block_size) {
    for (auto &tp : lhs) {
        tp->rewind(0);
    }

    for (auto &tp : rhs) {
        tp->rewind(0);
    }

    std::fill(rhs_size.begin(), rhs_size.end(), 0);
    std::size_t current_block = 0;

    std::vector<std::size_t> global_offset(lhs.size(), 0);

    while (!lhs[0]->is_last_cell()) {
        using pq_elem = std::pair<std::uint32_t, std::size_t>;
        std::priority_queue<pq_elem, std::vector<pq_elem>,
                            std::greater<pq_elem>>
            queue;

        for (std::size_t i = 0; i < lhs.size(); i++) {
            if (global_offset[i] < lhs_size[i]) {
                queue.emplace(lhs[i]->read_advance(), i);
            }
        }

        std::vector<std::size_t> elements_taken(lhs.size(), 0);

        while (!queue.empty()) {
            auto [value, tape_index] = queue.top();
            queue.pop();

            rhs[current_block]->write_advance(value);
            rhs_size[current_block]++;

            elements_taken[tape_index]++;
            global_offset[tape_index]++;

            if (global_offset[tape_index] < lhs_size[tape_index] &&
                elements_taken[tape_index] < block_size) {
                queue.emplace(lhs[tape_index]->read_advance(), tape_index);
            }
        }

        current_block = (current_block + 1) % rhs.size();
    }
}
}  // namespace

void sorter::balanced_merge_sort(tape &src,
                                 tape &dest,
                                 tape_supplier &tape_supplier,
                                 std::size_t tapes_count) {
    // distribute values to blocks of size M, write them on tapes_count * 2
    // additional tapes

    std::vector<std::unique_ptr<tape>> first_half;
    std::vector<std::unique_ptr<tape>> second_half;

    std::vector<std::size_t> first_half_size(tapes_count, 0);
    std::vector<std::size_t> second_half_size(tapes_count, 0);

    for (std::size_t i = 0; i < tapes_count; i++) {
        first_half.push_back(tape_supplier.new_tape());
        second_half.push_back(tape_supplier.new_tape());
    }

    std::size_t current_tape = 0;
    std::size_t total_size = 0;
    std::size_t block_size = m_config.ram_size;

    while (!src.is_last_cell()) {
        std::vector<std::uint32_t> data;
        while (!src.is_last_cell() && data.size() < m_config.ram_size) {
            data.push_back(src.read_advance());
        }

        total_size += data.size();
        std::sort(data.begin(), data.end());
        utils::write_to_tape(data, *first_half[current_tape]);
        first_half_size[current_tape] += data.size();
        current_tape = (current_tape + 1) % tapes_count;
    }

    // now we merge `tapes_count` blocks of size `block_size` and write the
    // result on a tape from another half
    while (block_size < total_size) {
        polyphase_merge(first_half, second_half, first_half_size,
                        second_half_size, block_size);
        std::swap(first_half, second_half);
        std::swap(first_half_size, second_half_size);
        block_size *= tapes_count;
    }

    first_half.front()->rewind(0);
    for (std::size_t i = 0; i < total_size; i++) {
        dest.write_advance(first_half.front()->read_advance());
    }
}
