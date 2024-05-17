#include <algorithm>
#include <ctime>
#include <random>
#include <vector>
#include "doctest.h"
#include "file_tape.hpp"
#include "utils.hpp"

TEST_CASE("New tape") {
    utils::create_tape_file("tape1", std::vector<uint32_t>{1, 4, 6, 7, 8});
    file_tape tape("tape1");
    CHECK(tape.read() == 1);
    CHECK(tape.shift_right());
    CHECK(tape.read() == 4);
    CHECK(tape.shift_right());
    CHECK(tape.read() == 6);
    CHECK(tape.shift_right());
    CHECK(tape.read() == 7);
    CHECK(tape.shift_right());
    CHECK(tape.read() == 8);
    CHECK(tape.shift_right());
    CHECK(tape.is_last_cell());
    CHECK(tape.shift_left());
    CHECK(tape.shift_left());
    CHECK(tape.read() == 7);
    tape.write(15);
    CHECK(tape.read() == 15);
    CHECK(tape.read_advance() == 15);
    CHECK(tape.read() == 8);
}

TEST_CASE("file tape stress test") {
    std::mt19937 rnd(std::time(nullptr));
    std::vector<std::uint32_t> data(10'000);
    std::generate(data.begin(), data.end(), rnd);

    utils::create_tape_file("test_data/random_tape", data);
    file_tape tape("test_data/random_tape");

    std::size_t offset = 0;

    for (int t = 0; t < 10'000; t++) {
        int command = rnd() % 5;
        if (command == 0) {
            CHECK(data[offset] == tape.read());
        } else if (command == 1) {
            std::uint32_t val = rnd();
            data[offset] = val;
            tape.write(val);
        } else if (command == 2 && offset > 0) {
            offset--;
            tape.shift_left();
        } else if (command == 3 && offset < data.size() - 1) {
            offset++;
            tape.shift_right();
        } else {
            offset = rnd() % data.size();
            tape.rewind(offset);
        }
    }
}

TEST_CASE("read/write advaace / rewind") {
    utils::create_and_clear_file("test_data/fill");
    file_tape tp("test_data/fill");

    for (std::uint32_t i = 0; i < 50; i++) {
        tp.write_advance(i);
    }

    tp.rewind(0);
    CHECK(!tp.is_last_cell());
    std::uint32_t curr = 0;
    while(!tp.is_last_cell()) {
        CHECK(curr++ == tp.read_advance());
    }

    std::vector<std::uint32_t> expected(50);
    std::iota(expected.begin(), expected.end(), 0);

    tp.rewind(0);
    CHECK(!tp.is_last_cell());
    auto actual = utils::tape_dump(tp);

    CHECK(expected == actual);
}