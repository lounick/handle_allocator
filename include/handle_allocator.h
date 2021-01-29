#pragma once

#include <cstddef>
#include <memory>
#include <optional>
#include <stack>
#include <vector>

#include "handle.h"

namespace handle_allocator {
template <typename T>
struct AllocatorElement {
  Handle handle;
  T data;
};

template <typename T>
class HandleAllocator {
 public:
  // max_size = 0 -> growing array
  HandleAllocator(std::size_t max_size = 0) : max_size_(max_size) {
    if (max_size_ > 0) {
      data_.reserve(max_size_);
      FillEmptyData();
    }
  }

  // Moving the data the user must give up
  std::optional<Handle> Insert(T&& data) {
    if (max_size_ > 0) {
      if (free_elements_.empty()) {
        return {};
      }
      std::size_t index = free_elements_.top();
      free_elements_.pop();
      data_.at(index).data = data;
      return std::optional<Handle>{data_.at(index).handle};
    }
    AllocatorElement<T> element;
    element.handle.index = data_.size();
    element.handle.pattern = 0;
    element.data = data;
    try {
      data_.push_back(element);
    } catch (const std::exception& e) {
      // Couldn't push back
      return {};
    }
    FillEmptyData();
  }

  std::optional<const T&> GetConst(const Handle& handle) const {}
  std::optional<T&> Get(const Handle& handle) {}
  bool Delete(const Handle& handle) {}

 private:
  void FillEmptyData() {
    while (data_.size() < data_.capacity()) {
      data_.push_back(AllocatorElement());
      std::size_t index = data_.size() - 1;
      data_.back().handle.index = index;
      data_.back().handle.pattern = 0;
      free_elements_.push(index);
    }
  }

  std::size_t max_size_;
  std::vector<AllocatorElement<T>> data_;
  std::stack<size_t> free_elements_;
};
}  // namespace handle_allocator