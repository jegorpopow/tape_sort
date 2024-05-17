#include <algorithm>
#include <ctime>
#include <iostream>
#include <random>
#include <vector>
#include "doctest.h"
#include "file_tape.hpp"
#include "file_tape_supplier.hpp"
#include "sorter.hpp"
#include "utils.hpp"

namespace {
void sort_in_ram_and_check(config cfg,
                           std::vector<std::uint32_t> const &data,
                           std::string const &name) {
    file_tape_supplier supplier("tmp/");
    sorter srt(cfg);
    utils::create_tape_file("test_data/" + name, data);
    utils::create_and_clear_file("test_data/" + name + "_sorted");

    file_tape src("test_data/" + name);
    file_tape dst("test_data/" + name + "_sorted");

    srt.sort_in_ram(src, dst, supplier);
    std::vector<std::uint32_t> actual = utils::tape_dump(dst);
    CHECK(std::is_sorted(actual.begin(), actual.end()));

    std::vector<std::uint32_t> expected = data;
    std::sort(expected.begin(), expected.end());

    CHECK(actual == expected);
}

void merge_sort_and_check(config cfg,
                          std::vector<std::uint32_t> const &data,
                          std::string const &name,
                          std::size_t tapes_count) {
    file_tape_supplier supplier("tmp/");
    sorter srt(cfg);
    utils::create_tape_file("test_data/" + name, data);
    utils::create_and_clear_file("test_data/" + name + "_sorted");

    file_tape src("test_data/" + name);
    file_tape dst("test_data/" + name + "_sorted");

    srt.balanced_merge_sort(src, dst, supplier, tapes_count);
    std::vector<std::uint32_t> actual = utils::tape_dump(dst);
    CHECK(std::is_sorted(actual.begin(), actual.end()));

    std::vector<std::uint32_t> expected = data;
    std::sort(expected.begin(), expected.end());

    CHECK(actual.size() == expected.size());

    CHECK(actual == expected);
}

}  // namespace

TEST_CASE("naive sort small tape") {
    config small_cfg{2, 1, 1, 1, 1};
    std::vector<std::uint32_t> data{5, 6, 3, 8, 0, 7};
    file_tape_supplier supplier("tmp/");
    sorter srt(small_cfg);

    utils::create_tape_file("test_data/small_tape", data);
    utils::create_and_clear_file("test_data/small_tape_sorted");

    file_tape small_tape("test_data/small_tape");
    file_tape small_tape_sorted("test_data/small_tape_sorted");

    srt.sort_in_ram(small_tape, small_tape_sorted, supplier);
    std::vector<std::uint32_t> actual = utils::tape_dump(small_tape_sorted);
    CHECK(std::is_sorted(actual.begin(), actual.end()));

    std::vector<std::uint32_t> expected = data;
    std::sort(expected.begin(), expected.end());
    CHECK(actual == expected);
}

TEST_CASE("naive sort big tape") {
    config small_cfg{10200, 1, 1, 1, 1};
    std::mt19937 rnd(std::time(nullptr));
    std::vector<std::uint32_t> data(10'000'000);
    std::generate(data.begin(), data.end(), rnd);
    file_tape_supplier supplier("tmp/");
    sorter srt(small_cfg);

    utils::create_tape_file("test_data/big_tape", data);
    utils::create_and_clear_file("test_data/big_tape_sorted");

    file_tape big_tape("test_data/big_tape");
    file_tape big_tape_sorted("test_data/big_tape_sorted");

    srt.sort_in_ram(big_tape, big_tape_sorted, supplier);
    std::vector<std::uint32_t> actual = utils::tape_dump(big_tape_sorted);
    CHECK(std::is_sorted(actual.begin(), actual.end()));

    std::vector<std::uint32_t> expected = data;
    std::sort(expected.begin(), expected.end());
    CHECK(actual.size() == expected.size());
    CHECK(actual == expected);
}

TEST_CASE("naive sort divided size") {
    config cfg{10, 1, 1, 1, 1};
    std::mt19937 rnd(std::time(nullptr));
    std::vector<std::uint32_t> data(1000);
    std::generate(data.begin(), data.end(), rnd);

    sort_in_ram_and_check(cfg, data, "naive_divided_size");
}

TEST_CASE("naive sort one block") {
    config cfg{100, 1, 1, 1, 1};
    std::mt19937 rnd(std::time(nullptr));
    std::vector<std::uint32_t> data(25);
    std::generate(data.begin(), data.end(), rnd);

    sort_in_ram_and_check(cfg, data, "naive_one_block");
}


TEST_CASE("balanced merge sort small test") {
    config cfg{3, 1, 1, 1, 1};
    std::mt19937 rnd(std::time(nullptr));
    std::vector<std::uint32_t> data(10);
    std::generate(data.begin(), data.end(),
                  [count = 10]() mutable { return count--; });
    merge_sort_and_check(cfg, data, "mergesort_small", 2);
}

TEST_CASE("balanced merge sort small random test") {
    config cfg{3, 1, 1, 1, 1};
    std::mt19937 rnd(std::time(nullptr));
    std::vector<std::uint32_t> data(10);
    std::generate(data.begin(), data.end(),
                  rnd);
    merge_sort_and_check(cfg, data, "mergesort_small_random", 2);
}

TEST_CASE("balanced merge sort one block") {
    config cfg{100, 1, 1, 1, 1};
    std::mt19937 rnd(std::time(nullptr));
    std::vector<std::uint32_t> data(25);
    std::generate(data.begin(), data.end(),
                  rnd);
    merge_sort_and_check(cfg, data, "mergesort_small_random", 2);
}

TEST_CASE("balanced merge sort divided size") {
    config cfg{10, 1, 1, 1, 1};
    std::mt19937 rnd(std::time(nullptr));
    std::vector<std::uint32_t> data(1000);
    std::generate(data.begin(), data.end(),
                  rnd);
    merge_sort_and_check(cfg, data, "mergesort_small_random", 3);
}

TEST_CASE("balanced merge sort stress test") {
    config cfg{128, 1, 1, 1, 1};
    std::mt19937 rnd(std::time(nullptr));
    std::vector<std::uint32_t> data(100'000);
    std::generate(data.begin(), data.end(), rnd);
    merge_sort_and_check(cfg, data, "mergesort_stress", 10);
}
