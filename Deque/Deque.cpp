#include <algorithm>
#include <cmath>
#include <cstddef>
#include <iostream>
#include <iterator>
#include <vector>

template <typename T>
class Deque {
 private:
  const size_t kCapacity = 10;
  size_t size_;
  size_t first_block_;
  size_t last_block_;
  size_t first_elem_;
  size_t last_elem_;
  std::vector<T*> deque_;

 public:
  Deque() {
    deque_.resize(2);
    size_ = 0;
    first_block_ = 0;
    last_block_ = 0;
    first_elem_ = 0;
    last_elem_ = 0;
  }

  Deque(const Deque& deque2) { copy(deque2); }

  Deque(size_t count) { create(count); }

  Deque(size_t count, const T& value) {
    try {
      size_ = count;
      deque_.resize(ceil(count / double(kCapacity)) * 2);
      deque_.assign(deque_.size(), nullptr);
      int diff = ceil(count / double(kCapacity)) - 1;
      first_block_ = deque_.size() / 2;
      last_block_ = first_block_ + diff;
      first_elem_ = 0;
      last_elem_ =
          size_ % kCapacity == 0 ? kCapacity - 1 : size_ % kCapacity - 1;

      size_t begin = first_elem_;
      size_t end = kCapacity - 1;

      for (size_t i = 0; i < deque_.size(); ++i) {
        deque_[i] = reinterpret_cast<T*>(operator new[](sizeof(T) * kCapacity));
        if (i >= first_block_ && i <= last_block_) {
          end = (i == last_block_) ? last_elem_ : kCapacity - 1;
          for (size_t j = begin; j <= end; ++j) {
            new (&deque_[i][j]) T(value);
          }
        }
      }
    } catch (...) {
      clear();
      throw;
    }
  }

  ~Deque() {
    for (size_t i = 0; i < deque_.size(); ++i) {
      operator delete[](deque_[i]);
    }
  }

  void clear() {
    for (size_t i = 0; i < deque_.size(); ++i) {
      operator delete[](deque_[i]);
    }
    size_ = 0;
    first_block_ = 0;
    first_elem_ = 0;
    last_elem_ = 0;
    last_block_ = 0;
  }
  void create(size_t size) {
    clear();
    size_ = size;
    deque_.resize(ceil(size / double(kCapacity)) * 2);
    int diff = ceil(size / double(kCapacity)) - 1;
    first_block_ = deque_.size() / 2;
    last_block_ = first_block_ + diff;
    first_elem_ = 0;
    last_elem_ = size % kCapacity == 0 ? kCapacity - 1 : size % kCapacity - 1;

    for (size_t i = 0; i < deque_.size(); ++i) {
      deque_[i] = reinterpret_cast<T*>(operator new[](sizeof(T) * kCapacity));

      if constexpr (std::is_default_constructible_v<T> &&
                    std::is_constructible_v<T>) {
        for (size_t j = 0; j < kCapacity; ++j) {
          new (&deque_[i][j]) T();
        }
      } else {
      }
    }
  }

  void copy(const Deque& deque2) {
    clear();
    deque_.resize(deque2.deque_.size(), nullptr);
    size_ = deque2.size_;
    first_block_ = deque2.first_block_;
    last_block_ = deque2.last_block_;
    first_elem_ = deque2.first_elem_;
    last_elem_ = deque2.last_elem_;

    size_t begin = first_elem_;
    size_t end = kCapacity - 1;

    for (size_t i = 0; i < deque_.size(); ++i) {
      deque_[i] = reinterpret_cast<T*>(operator new[](sizeof(T) * kCapacity));
    }

    if (deque2.empty()) {
      return;
    }

    for (size_t i = first_block_; i < last_block_ + 1; ++i) {
      end = (i == last_block_) ? last_elem_ : kCapacity - 1;
      for (size_t k = begin; k <= end; ++k) {
        deque_[i][k] = deque2.deque_[i][k];
      }
      begin = 0;
    }
  }

  void resize(size_t size) {
    size_t begin = first_elem_;
    size_t end = kCapacity - 1;

    size_t diff = last_block_ - first_block_;

    deque_.resize(deque_.size() * size);

    for (size_t i = last_block_ + 1; i < deque_.size(); ++i) {
      if (deque_[i] == nullptr) {
        deque_[i] = reinterpret_cast<T*>(operator new[](sizeof(T) * kCapacity));
      }
    }
    for (size_t i = first_block_, j = (deque_.size() - 1) / 2;
         i < last_block_ + 1; ++i, ++j) {
      end = (i == last_block_) ? last_elem_ : kCapacity - 1;
      for (size_t k = begin; k <= end; ++k) {
        new (&deque_[j][k]) T(deque_[i][k]);
      }
      begin = 0;
    }
    first_block_ = (deque_.size() - 1) / 2;
    last_block_ = first_block_ + diff;
  }

  Deque<T>& operator=(const Deque<T>& other) {
    copy(other);
    return *this;
  }

  size_t size() const { return size_; }

  bool empty() const { return size_ == 0; }

  T& operator[](size_t idx) {
    size_t second_idx = idx;
    size_t first_idx = idx / double(kCapacity - 1) <= 1
                           ? first_block_
                           : ceil(idx / kCapacity) + first_block_;
    if (idx / double(kCapacity - 1) > 1) {
      second_idx = (idx - kCapacity + first_elem_) % kCapacity;
    }
    return deque_[first_idx][second_idx];
  }

  const T& operator[](size_t idx) const {
    size_t second_idx = idx;
    size_t first_idx = idx / double(kCapacity - 1) <= 1
                           ? first_block_
                           : ceil(idx / kCapacity) + first_block_;
    if (idx / double(kCapacity - 1) > 1) {
      second_idx = (idx - kCapacity + first_elem_) % kCapacity;
    }
    return deque_[first_idx][second_idx];
  }

  bool operator==(const Deque<T>& other) {
    if (size_ != other.size_) {
      return false;
    }
    for (int i = 0; i < size_; ++i) {
      if (deque_[i] != other.deque_[i]) {
        return false;
      }
    }
    return true;
  }

  T& at(size_t idx) {
    if (idx < size_) {
      size_t second_idx = idx;
      size_t first_idx = idx / double(kCapacity - 1) <= 1
                             ? first_block_
                             : ceil(idx / kCapacity) + first_block_;
      if (idx / double(kCapacity - 1) > 1) {
        second_idx = (idx - kCapacity + first_elem_) % kCapacity;
      }
      return deque_[first_idx][second_idx];
    }
    throw std::out_of_range("Out of range");
  }

  const T& at(size_t idx) const {
    if (idx < size_) {
      size_t second_idx = idx;
      size_t first_idx = idx / double(kCapacity - 1) <= 1
                             ? first_block_
                             : ceil(idx / kCapacity) + first_block_;
      if (idx / double(kCapacity - 1) > 1) {
        second_idx = (idx - kCapacity + first_elem_) % kCapacity;
      }
      return deque_[first_idx][second_idx];
    }
    throw std::out_of_range("Out of range");
  }

  void push_back(const T& element) {
    if (empty()) {
      create(1);
      size_ = 0;
      last_elem_ = kCapacity + 1;
    } else if (last_elem_ == kCapacity - 1) {
      if (last_block_ == deque_.size() - 1) {
        resize(2);
      }
      ++last_block_;
      last_elem_ = kCapacity + 1;
    }
    last_elem_ = (last_elem_ == kCapacity + 1) ? 0 : last_elem_ + 1;
    new (&deque_[last_block_][last_elem_]) T(element);
    ++size_;
  }

  void make_it_zero() {
    last_block_ = 0;
    last_elem_ = 0;
    first_block_ = 0;
    first_elem_ = 0;
  }

  void pop_back() {
    if (empty()) {
      return;
    }
    if (last_elem_ == 0) {
      last_elem_ = kCapacity - 1;
      --last_block_;
    } else {
      --last_elem_;
    }
    --size_;
    if (size_ == 0) {
      make_it_zero();
    }
  }

  void push_front(const T& element) {
    if (empty()) {
      clear();
      create(1);
      --size_;
      first_elem_ = 1;
    } else if (first_elem_ == 0) {
      if (first_block_ == 0) {
        resize(2);
      }
      --first_block_;
      first_elem_ = kCapacity;
    }
    --first_elem_;
    new (&deque_[first_block_][first_elem_]) T(element);
    ++size_;
  }

  void pop_front() {
    if (empty()) {
      return;
    }
    if (first_elem_ == kCapacity - 1) {
      first_elem_ = 0;
      ++first_block_;
    } else {
      ++first_elem_;
    }
    --size_;
    if (size_ == 0) {
      make_it_zero();
    }
  }

  template <bool IsConst>
  class Iterator : public std::iterator<
                       std::random_access_iterator_tag,
                       typename std::conditional<IsConst, const T, T>::type> {
   private:
    Deque<T>* start_ = nullptr;
    size_t index_ = 0;

   public:
    using difference_type = typename std::iterator<
        std::random_access_iterator_tag,
        typename std::conditional<IsConst, const T, T>::type>::difference_type;
    using pointer = typename std::iterator<
        std::random_access_iterator_tag,
        typename std::conditional<IsConst, const T, T>::type>::pointer;
    using reference = typename std::iterator<
        std::random_access_iterator_tag,
        typename std::conditional<IsConst, const T, T>::type>::reference;

    Iterator() = default;

    Iterator(const Iterator& iter2) {
      index_ = iter2.index_;
      start_ = iter2.start_;
    }

    Iterator(Iterator& iter2) {
      index_ = iter2.index_;
      start_ = iter2.start_;
    }

    Iterator(Deque<T>* other, int idx = 0) {
      start_ = other;
      index_ = idx;
    }

    void operator=(const Iterator& iter2) {
      index_ = iter2.index_;
      start_ = iter2.start_;
    }

    Iterator& operator++() {
      ++index_;
      return *this;
    }

    Iterator& operator--() {
      --index_;
      return *this;
    }

    Iterator operator++(int) {
      Iterator<IsConst> iter2(*this);
      ++index_;
      return iter2;
    }

    Iterator operator--(int) {
      Iterator<IsConst> iter2(*this);
      --index_;
      return iter2;
    }

    Iterator& operator+=(int size) {
      index_ += size;
      return *this;
    }

    Iterator operator+(int size) const {
      Iterator<IsConst> copy(*this);
      copy += size;
      return copy;
    }

    Iterator& operator-=(int size) {
      index_ -= size;
      return *this;
    }

    Iterator<IsConst> operator-(int size) const {
      Iterator<IsConst> copy(*this);
      copy -= size;
      return copy;
    }

    difference_type operator-(const Iterator& iter2) const {
      return index_ - iter2.index_;
    }

    difference_type operator+(const Iterator& iter2) const {
      return index_ + iter2.index_;
    }

    reference operator*() const {
      size_t second_idx = index_;
      size_t first_idx =
          index_ / double(start_->kCapacity - 1) <= 1
              ? start_->first_block_
              : ceil(index_ / start_->kCapacity) + start_->first_block_;
      if (index_ / double(start_->kCapacity - 1) > 1) {
        second_idx = (index_ - start_->kCapacity + start_->first_elem_) %
                     start_->kCapacity;
      }
      return *((*start_).deque_[first_idx] + second_idx);
    }

    pointer operator->() const {
      size_t second_idx = index_;
      size_t first_idx =
          index_ / double(start_->kCapacity - 1) <= 1
              ? start_->first_block_
              : ceil(index_ / start_->kCapacity) + start_->first_block_;
      if (index_ / double(start_->kCapacity - 1) > 1) {
        second_idx = (index_ - start_->kCapacity + start_->first_elem_) %
                     start_->kCapacity;
      }
      return start_->deque_[first_idx] + second_idx;
    }

    bool operator<(const Iterator& iter) const { return index_ < iter.index_; }

    bool operator>(const Iterator& iter) const { return index_ > iter.index_; }

    bool operator<=(const Iterator& iter) const {
      return index_ <= iter.index_;
    }

    bool operator>=(const Iterator& iter) const {
      return index_ >= iter.index_;
    }

    bool operator==(const Iterator& iter) const {
      return index_ == iter.index_;
    }

    bool operator!=(const Iterator& iter) const {
      return index_ != iter.index_;
    }
  };

  using iterator = Iterator<false>;
  using const_iterator = Iterator<true>;
  using reverse_iterator = std::reverse_iterator<Iterator<false>>;
  using const_reverse_iterator = std::reverse_iterator<Iterator<true>>;

  iterator begin() { return iterator(this, 0); }

  const_iterator cbegin() { return const_iterator(this, 0); }

  iterator end() { return iterator(this, size_); }

  const_iterator cend() { return const_iterator(this, size_); }

  reverse_iterator rbegin() { return reverse_iterator(iterator(this, size_)); }

  const_reverse_iterator crbegin() {
    return const_reverse_iterator(iterator(this, size_));
  }

  reverse_iterator rend() { return reverse_iterator(iterator(this, 0)); }

  const_reverse_iterator crend() {
    return const_reverse_iterator(iterator(this, 0));
  }

  void insert(iterator iter, const T& elem) {
    if (empty()) {
      clear();
      push_back(elem);
      return;
    }
    if (last_elem_ % kCapacity == kCapacity - 1) {
      if (last_block_ + 1 == deque_.size()) {
        resize(2);
      }
      ++last_block_;
    }
    ++size_;
    last_elem_ = (last_elem_ + 1) % kCapacity;

    for (auto iter_to = end() - 1; iter_to > iter && iter_to - 1 != 0;
         --iter_to) {
      *(iter_to) = *(iter_to - 1);
    }
    *iter = elem;
  }

  void erase(iterator iter) {
    if (empty()) {
      return;
    }
    while (iter < end() - 1) {
      *(iter) = *(iter + 1);
      ++iter;
    }
    --size_;
    if (last_elem_ % kCapacity == 0 && last_elem_ != 0) {
      last_elem_ = kCapacity - 1;
      --last_block_;
    } else {
      --last_elem_;
    }
  }
};