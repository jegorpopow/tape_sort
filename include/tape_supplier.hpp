#ifndef TAPE_SUPPLIER_HPP_
#define TAPE_SUPPLIER_HPP_

#include <memory>
#include "tape.hpp"

struct tape_supplier {
    virtual std::unique_ptr<tape> new_tape() = 0;
    virtual ~tape_supplier() = default;
};

#endif