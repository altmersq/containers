#ifndef S21_STACK_H
#define S21_STACK_H

#include <initializer_list>
#include <iostream>
#include <limits>

#include "s21_vector.h"

namespace s21 {
template <typename T>
class stack {
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
  size_type size_;

 public:
  stack() : head_(nullptr), size_(0) {}

  stack(std::initializer_list<value_type> il) : stack() {
    for (const auto& value : il) push(value);
  }

  stack(const stack& other) : stack() {
    stack<value_type> tmp;
    for (Node* node = other.head_; node; node = node->next) {
      tmp.push(node->data);
    }
    while (!tmp.empty()) {
      push(tmp.top());
      tmp.pop();
    }
  }

  stack(stack&& other) : head_(other.head_), size_(other.size_) {
    other.head_ = nullptr;
    other.size_ = 0;
  }

  ~stack() { clear(); }

  stack& operator=(const stack& other) {
    if (this != &other) {
      clear();
      for (Node* node = other.head_; node; node = node->next) push(node->data);
    }
    return *this;
  }

  stack& operator=(stack&& other) noexcept {
    if (this != &other) {
      clear();
      head_ = other.head_;
      size_ = other.size_;
      other.head_ = nullptr;
      other.size_ = 0;
    }
    return *this;
  }

  const_reference top() const { return head_->data; }

  bool empty() const { return size_ == 0; }
  size_type size() const { return size_; }

  void push(const_reference value) {
    head_ = new Node(value, head_);
    ++size_;
  }

  void pop() {
    if (head_) {
      Node* toDelete = head_;
      head_ = head_->next;
      delete toDelete;
      --size_;
    }
  }

  void clear() {
    while (!empty()) pop();
  }

  void swap(stack& other) {
    std::swap(head_, other.head_);
    std::swap(size_, other.size_);
  }

  // \/ BONUS PART \/ BONUS PART \/ BONUS PART \/ BONUS PART

  template <typename... Args>
  void insert_many_front(Args&&... args) {
    std::initializer_list<value_type> temp = {std::forward<Args>(args)...};
    for (auto it = temp.begin(); it != temp.end(); ++it) {
      push(*it);
    }
  }

  // /\ BONUS PART /\ BONUS PART /\ BONUS PART /\ BONUS PART
};

}  // namespace s21

#endif  // S21_STACK_H