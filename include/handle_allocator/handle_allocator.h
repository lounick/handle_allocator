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
  std::shared_ptr<T> data;
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
      data_.at(index).data = std::make_shared<T>(data);
      return std::optional<Handle>{data_.at(index).handle};
    }
    AllocatorElement<T> element;
    element.handle.index = data_.size();
    element.handle.pattern = 0;
    element.data = std::make_shared<T>(data);
    try {
      data_.push_back(element);
    } catch (const std::exception& e) {
      // Couldn't push back
      return {};
    }
    FillEmptyData();
    return std::optional<Handle>{data_.back().handle};
  }

  std::optional<std::weak_ptr<const T>> GetConst(const Handle& handle) const {
    if (!IsHandleValid(handle)) {
      return {};
    }

    return std::optional<std::weak_ptr<const T>>{
        std::const_pointer_cast<const T>(data_.at(handle.index).data)};
  }

  std::optional<std::weak_ptr<T>> Get(const Handle& handle) {
    if (!IsHandleValid(handle)) {
      return {};
    }

    return std::optional<std::weak_ptr<T>>{data_.at(handle.index).data};
  }

  bool Delete(const Handle& handle) {
    if (!IsHandleValid(handle)) {
      return false;
    }

    // Change the pattern to invalidate handles
    data_.at(handle.index).handle.pattern += 1;
    // Now free the object hopefully others don't keep it alive
    data_.at(handle.index).data = nullptr;
    free_elements_.push(handle.index);
    return true;
  }

 private:
  void FillEmptyData() {
    while (data_.size() < data_.capacity()) {
      data_.push_back(AllocatorElement<T>());
      std::size_t index = data_.size() - 1;
      data_.back().handle.index = index;
      data_.back().handle.pattern = 0;
      data_.back().data = nullptr;
      free_elements_.push(index);
    }
  }

  bool IsHandleValid(const Handle& handle) const {
    size_t index = handle.index;
    size_t pattern = handle.pattern;
    if (data_.size() <= index || data_.at(index).handle.pattern != pattern) {
      return false;
    }
    return true;
  }

  std::size_t max_size_;
  std::vector<AllocatorElement<T>> data_;
  std::stack<size_t> free_elements_;
};
}  // namespace handle_allocator
