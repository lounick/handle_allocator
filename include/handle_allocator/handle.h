#pragma once

#include <cstddef>

namespace handle_allocator {
struct Handle {
  std::size_t index;
  std::size_t pattern;
  inline bool operator==(const Handle& rhs) {
    return this->index == rhs.index && this->pattern == rhs.pattern;
  }
};

}  // namespace handle_allocator