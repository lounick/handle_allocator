#pragma once

#include <handle_allocator/handle.h>

#include <cstddef>
#include <functional>
#include <memory>
#include <optional>
#include <stack>
#include <vector>

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

  std::optional<std::reference_wrapper<const T>> GetConst(
      const Handle& handle) const {
    if (!IsHandleValid(handle)) {
      return {}
    }

    return std::optional<std::reference_wrapper<const T>> {
      std::cref(data_.at(handle.index).data)
    }
  }

  std::optional<std::reference_wrapper<T>> Get(const Handle& handle) {
    if (!IsHandleValid(handle)) {
      return {}
    }

    return std::optional<std::reference_wrapper<T>> {
      std::ref(data_.at(handle.index).data)
    }
  }

  bool Delete(const Handle& handle) {
    if (!IsHandleValid(handle)) {
      return false;
    }

    data_.at(handle.index).handle.pattern += 1;
    free_elements_.push(handle.index);
  }

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

  bool IsHandleValid(const Handle& handle) {
    size_t index = handle.index;
    size_t pattern = handle.pattern;
    if (data_.size() < index || data_.at(index).handle.pattern != pattern) {
      return false;
    }
    return true;
  }

  std::size_t max_size_;
  std::vector<AllocatorElement<T>> data_;
  std::stack<size_t> free_elements_;
};
}  // namespace handle_allocator
