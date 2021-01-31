#pragma once

#include <cstddef>

namespace handle_allocator {
struct Handle {
  std::size_t index;
  std::size_t pattern;
};

}  // namespace handle_allocator