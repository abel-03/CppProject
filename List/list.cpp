#pragma once

#include <initializer_list>
#include <iostream>
#include <memory>

template <typename T, typename Allocator = std::allocator<T>>
class List {
 private:
  struct Node {
    T value = T();
    Node* prev = nullptr;
    Node* next = nullptr;
    Node() = default;
    Node(const T& value) : value(value) {}
  };

  Node* head_ = nullptr;
  Node* Fake_node_ = nullptr;
  Node* tail_ = nullptr;
  size_t size_ = 0;

  using alloc_type =
      typename std::allocator_traits<Allocator>::template rebind_alloc<Node>;
  using alloc_traits = std::allocator_traits<alloc_type>;

  alloc_type alloc_ = Allocator();

 public:
  using value_type = T;
  using allocator_type = Allocator;

  alloc_type get_allocator() { return alloc_; }

  List() = default;

  List(size_t count, const T& value, const Allocator& alloc = Allocator())
      : alloc_(alloc) {
    try {
      fake_node();
      head_ = alloc_traits::allocate(alloc_, 1);
      alloc_traits::construct(alloc_, head_, value);
      head_->next = Fake_node_;
      head_->prev = Fake_node_;
      tail_ = head_;
      Fake_node_->next = head_;
      Fake_node_->prev = tail_;
      ++size_;

      for (size_t i = 0; i < count - 1; ++i) {
        Node* new_node = alloc_traits::allocate(alloc_, 1);
        alloc_traits::construct(alloc_, new_node, value);
        new_node->prev = tail_;
        new_node->next = Fake_node_;
        tail_->next = new_node;
        tail_ = new_node;
        Fake_node_->prev = tail_;
        ++size_;
      }
    } catch (...) {
      delete_list();
      throw;
    }
  }

  explicit List(size_t count, const Allocator& alloc = Allocator())
      : alloc_(alloc) {
    if (count == 0) {
      return;
    }
    fake_node();
    head_ = alloc_traits::allocate(alloc_, 1);
    try {
      alloc_traits::construct(alloc_, head_);
    } catch (...) {
      alloc_traits::deallocate(alloc_, head_, 1);
      alloc_traits::deallocate(alloc_, Fake_node_, 1);
      throw;
    }
    head_->next = Fake_node_;
    head_->prev = Fake_node_;
    tail_ = head_;
    Fake_node_->next = head_;
    Fake_node_->prev = tail_;
    ++size_;

    for (size_t i = 1; i < count; ++i) {
      Node* new_node = alloc_traits::allocate(alloc_, 1);
      try {
        alloc_traits::construct(alloc_, new_node);
      } catch (...) {
        alloc_traits::deallocate(alloc_, new_node, 1);
        delete_list();
        throw;
      }
      new_node->prev = tail_;
      new_node->next = Fake_node_;
      tail_->next = new_node;
      tail_ = new_node;
      Fake_node_->prev = tail_;
      ++size_;
    }
  }

  List(const List& other)
      : alloc_(std::allocator_traits<Allocator>::
                   select_on_container_copy_construction(other.alloc_)) {
    Node* current = other.head_;
    for (size_t i = 0; i < other.size_; ++i) {
      push_back(current->value);
      current = current->next;
    }
  }

  List(std::initializer_list<T> init, const Allocator& alloc = Allocator())
      : alloc_(alloc) {
    for (auto iter = init.begin(); iter != init.end(); ++iter) {
      push_back(*iter);
    }
  }

  ~List() { delete_list(); }

  void fake_node() { Fake_node_ = alloc_traits::allocate(alloc_, 1); }

  void delete_list() {
    if (size_ == 0) {
      return;
    }
    Node* current = head_->next;
    for (Node* node = current->prev; current != Fake_node_;
         node = current->prev) {
      alloc_traits::destroy(alloc_, node);  //
      alloc_traits::deallocate(alloc_, node, 1);
      current = current->next;
    }
    alloc_traits::destroy(alloc_, Fake_node_->prev);
    alloc_traits::deallocate(alloc_, Fake_node_->prev, 1);
    alloc_traits::deallocate(alloc_, Fake_node_, 1);
    size_ = 0;
  }

  template <bool IsConst>
  class Iterator : public std::iterator<
                       std::bidirectional_iterator_tag,
                       typename std::conditional<IsConst, const T, T>::type> {
   private:
    Node* start_ = nullptr;

   public:
    using difference_type = typename std::iterator<
        std::bidirectional_iterator_tag,
        typename std::conditional<IsConst, const T, T>::type>::difference_type;
    using pointer = typename std::iterator<
        std::bidirectional_iterator_tag,
        typename std::conditional<IsConst, const T, T>::type>::pointer;
    using reference = typename std::iterator<
        std::bidirectional_iterator_tag,
        typename std::conditional<IsConst, const T, T>::type>::reference;

    Iterator() = default;

    Iterator(const Iterator& iter2) { start_ = iter2.start_; }

    Iterator(Iterator& iter2) { start_ = iter2.start_; }

    Iterator(Node* start2) { start_ = start2; }

    void operator=(const Iterator& iter2) { start_ = iter2.start_; }

    Iterator& operator++() {
      start_ = start_->next;
      return *this;
    }

    Iterator& operator--() {
      start_ = start_->prev;
      return *this;
    }

    Iterator operator++(int) {
      Iterator<IsConst> iter2(*this);
      start_ = start_->next;
      return iter2;
    }

    Iterator operator--(int) {
      Iterator<IsConst> iter2(*this);
      start_ = start_->prev;
      return iter2;
    }

    reference operator*() const { return start_->value; }

    pointer operator->() const { return &(start_->value); }

    bool operator==(const Iterator& iter) const {
      return start_ == iter.start_;
    }

    bool operator!=(const Iterator& iter) const {
      return start_ != iter.start_;
    }
  };

  using iterator = Iterator<false>;
  using const_iterator = Iterator<true>;
  using reverse_iterator = std::reverse_iterator<Iterator<false>>;
  using const_reverse_iterator = std::reverse_iterator<Iterator<true>>;

  iterator begin() { return iterator(head_); }

  iterator begin() const { return iterator(head_); }

  iterator end() { return iterator(Fake_node_); }

  iterator end() const { return iterator(Fake_node_); }

  const_iterator cbegin() { return const_iterator(head_); }

  const_iterator cend() { return const_iterator(Fake_node_); }

  reverse_iterator rbegin() { return reverse_iterator(iterator(Fake_node_)); }

  reverse_iterator rend() { return reverse_iterator(iterator(head_)); }

  const_reverse_iterator crbegin() {
    return const_reverse_iterator(iterator(Fake_node_));
  }

  const_reverse_iterator crend() {
    return const_reverse_iterator(iterator(head_));
  }

  List& operator=(const List& other) {
    if (std::allocator_traits<
            Allocator>::propagate_on_container_copy_assignment::value) {
      alloc_ = other.alloc_;
    }

    Node* current = other.head_;
    Node* this_node = head_;
    size_t iter = 0;
    while (iter < std::min(other.size_, size_)) {
      this_node->value = current->value;
      current = current->next;
      this_node = this_node->next;
      ++iter;
    }

    if (other.size_ < size_) {
      while (iter < size_) {
        pop_back();
      }
    } else {
      for (size_t i = size_; i < other.size_; ++i) {
        try {
          push_back(current->value);
        } catch (...) {
          throw;
        }
        current = current->next;
      }
    }

    return *this;
  }

  T& front() { return head_->value; }
  const T& front() const { return head_->value; }
  T& back() { return tail_->value; }
  const T& back() const { return tail_->value; }

  bool empty() const { return size_ == 0; }
  size_t size() const { return size_; }

  void push_back(const T& value) {
    Node* new_node = alloc_traits::allocate(alloc_, 1);
    try {
      alloc_traits::construct(alloc_, new_node, value);
    } catch (...) {
      alloc_traits::deallocate(alloc_, new_node, 1);
      delete_list();
      throw;
    }
    if (empty()) {
      fake_node();
      head_ = new_node;
      head_->next = Fake_node_;
      head_->prev = Fake_node_;
      tail_ = head_;
      Fake_node_->next = head_;
      Fake_node_->prev = tail_;
      ++size_;
    } else {
      tail_->next = new_node;
      new_node->prev = tail_;
      new_node->next = Fake_node_;
      Fake_node_->prev = new_node;
      tail_ = new_node;
      ++size_;
    }
  }

  void push_back(T&& value) {
    Node* new_node = alloc_traits::allocate(alloc_, 1);
    try {
      alloc_traits::construct(alloc_, new_node, std::move(value));
    } catch (...) {
      alloc_traits::deallocate(alloc_, new_node, 1);
      delete_list();
      throw;
    }
    if (empty()) {
      fake_node();
      head_ = new_node;
      head_->next = Fake_node_;
      head_->prev = Fake_node_;
      tail_ = head_;
      Fake_node_->next = head_;
      Fake_node_->prev = tail_;
      ++size_;
    } else {
      tail_->next = new_node;
      new_node->prev = tail_;
      new_node->next = Fake_node_;
      Fake_node_->prev = new_node;
      tail_ = new_node;
      ++size_;
    }
  }

  void push_front(const T& value) {
    Node* new_node = alloc_traits::allocate(alloc_, 1);
    try {
      alloc_traits::construct(alloc_, new_node, value);
    } catch (...) {
      alloc_traits::deallocate(alloc_, new_node, 1);
      delete_list();
      throw;
    }
    if (empty()) {
      fake_node();
      head_ = new_node;
      head_->next = Fake_node_;
      head_->prev = Fake_node_;
      Fake_node_->next = head_;
      Fake_node_->prev = head_;
      ++size_;
    } else {
      head_->prev = new_node;
      new_node->prev = Fake_node_;
      new_node->next = head_;
      Fake_node_->next = new_node;
      head_ = new_node;
      ++size_;
    }
  }

  void push_front(T&& value) {
    Node* new_node = alloc_traits::allocate(alloc_, 1);
    try {
      alloc_traits::construct(alloc_, new_node, std::move(value));
    } catch (...) {
      alloc_traits::deallocate(alloc_, new_node, 1);
      delete_list();
      throw;
    }
    if (empty()) {
      fake_node();
      head_ = new_node;
      head_->next = Fake_node_;
      head_->prev = Fake_node_;
      tail_ = head_;
      Fake_node_->next = head_;
      Fake_node_->prev = tail_;
      ++size_;
    } else {
      head_->prev = new_node;
      new_node->prev = Fake_node_;
      new_node->next = head_;
      Fake_node_->next = new_node;
      head_ = new_node;
      ++size_;
    }
  }

  void pop_back() {
    tail_ = tail_->prev;
    Fake_node_->prev = tail_;
    alloc_traits::destroy(alloc_, tail_->next);
    alloc_traits::deallocate(alloc_, tail_->next, 1);
    tail_->next = Fake_node_;
    --size_;
  }

  void pop_front() {
    head_ = head_->next;
    Fake_node_->next = head_;
    alloc_traits::destroy(alloc_, head_->prev);
    alloc_traits::deallocate(alloc_, head_->prev, 1);
    head_->prev = Fake_node_;
    --size_;
  }
};
