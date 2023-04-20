#pragma once

#include <cstddef>
#include <vector>

class RingBuffer {
  std::vector<int> queue;  
  size_t MaxSize;

 public:
  explicit RingBuffer(size_t capacity) : MaxSize(capacity) {}

  size_t Size() const { return queue.size(); }

  bool Empty() const { return queue.size() == 0; }

  bool TryPush(int element) {
    if (queue.size() == MaxSize) {
      return false;
    }
    queue.push_back(element);
    return true;
  }
  bool TryPop(int* element) {
    if (queue.size() == 0) {
      return false;
    }
    *element = queue[0];
    for (int i = 0; i < queue.size() - 1; i++) {
      queue[i] = queue[i + 1];
    }
    queue.pop_back();
    return true;
  }
};