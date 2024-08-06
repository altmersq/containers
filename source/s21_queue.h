#ifndef S21_QUEUE_H
#define S21_QUEUE_H

#include <initializer_list>
#include <iostream>
#include <limits>

namespace s21 {

template <typename T>
class queue {
 public:
  using value_type = T;
  using reference = T&;
  using const_reference = const T&;
  using size_type = size_t;

 private:
  struct Node {
    value_type data;
    Node* next;
    Node(const value_type& val, Node* n = nullptr) : data(val), next(n) {}
    Node(const Node&) = delete;
    Node& operator=(const Node&) = delete;
  };

  Node* head_;
  Node* tail_;
  size_type size_;

 public:
  queue() : head_(nullptr), tail_(nullptr), size_(0) {}

  queue(std::initializer_list<value_type> il) : queue() {
    for (const auto& value : il) push(value);
  }

  queue(const queue& other) : queue() {
    for (Node* node = other.head_; node; node = node->next) push(node->data);
  }

  queue(queue&& other) noexcept
      : head_(other.head_), tail_(other.tail_), size_(other.size_) {
    other.head_ = other.tail_ = nullptr;
    other.size_ = 0;
  }

  ~queue() { clear(); }

  queue& operator=(const queue& other) {
    if (this != &other) {
      clear();
      for (Node* node = other.head_; node; node = node->next) push(node->data);
    }
    return *this;
  }

  queue& operator=(queue&& other) noexcept {
    if (this != &other) {
      clear();
      head_ = other.head_;
      tail_ = other.tail_;
      size_ = other.size_;
      other.head_ = other.tail_ = nullptr;
      other.size_ = 0;
    }
    return *this;
  }

  const_reference front() const { return head_->data; }
  const_reference back() const { return tail_->data; }

  bool empty() const { return size_ == 0; }
  size_type size() const { return size_; }

  void push(const_reference value) {
    Node* node = new Node(value);
    if (tail_) tail_->next = node;
    tail_ = node;
    if (!head_) head_ = node;
    ++size_;
  }

  void pop() {
    if (head_) {
      Node* toDelete = head_;
      head_ = head_->next;
      if (!head_) tail_ = nullptr;
      delete toDelete;
      --size_;
    }
  }

  void clear() {
    while (!empty()) pop();
  }

  void swap(queue& other) {
    std::swap(head_, other.head_);
    std::swap(tail_, other.tail_);
    std::swap(size_, other.size_);
  }

  // \/ BONUS PART \/ BONUS PART \/ BONUS PART \/ BONUS PART

  template <typename... Args>
  void insert_many_back(Args&&... args) {
    (void)std::initializer_list<int>{(push(std::forward<Args>(args)), 0)...};
  }

  // /\ BONUS PART /\ BONUS PART /\ BONUS PART /\ BONUS PART
};

}  // namespace s21

#endif  // S21_QUEUE_H
