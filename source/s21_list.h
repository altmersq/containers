#ifndef S21_LIST_H
#define S21_LIST_H

#include <initializer_list>
#include <iostream>
#include <limits>

namespace s21 {

template <typename T>
class list {
 public:
  using value_type = T;
  using reference = T&;
  using const_reference = const T&;
  using size_type = size_t;

  struct Node {
    value_type data;
    Node* prev;
    Node* next;
    Node(const value_type& val, Node* p = nullptr, Node* n = nullptr)
        : data(val), prev(p), next(n) {}
  };

 private:
  Node* head_;
  Node* tail_;
  size_type size_;

 public:
  list() : head_(nullptr), tail_(nullptr), size_(0) {}
  list(size_type n, const value_type& value = value_type()) : list() {
    while (n--) push_back(value);
  }
  list(std::initializer_list<value_type> il) : list() {
    for (const auto& value : il) push_back(value);
  }
  list(const list& other) : list() {
    for (Node* node = other.head_; node; node = node->next)
      push_back(node->data);
  }
  list(list&& other)
      : head_(other.head_), tail_(other.tail_), size_(other.size_) {
    other.head_ = other.tail_ = nullptr;
    other.size_ = 0;
  }
  ~list() { clear(); }

  list& operator=(const list& other) {
    if (this != &other) {
      clear();
      for (Node* node = other.head_; node; node = node->next)
        push_back(node->data);
    }
    return *this;
  }
  list& operator=(list&& other) noexcept {
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

  void clear() {
    while (!empty()) pop_front();
  }

  const_reference front() const { return head_->data; }
  const_reference back() const { return tail_->data; }

  bool empty() const { return size_ == 0; }
  size_type size() const { return size_; }
  size_type max_size() const {
    return std::numeric_limits<size_type>::max() / sizeof(Node);
  }

  class iterator {
   public:
    Node* ptr_;
    iterator(Node* ptr = nullptr) : ptr_(ptr) {}
    iterator& operator++() {
      ptr_ = ptr_->next;
      return *this;
    }
    iterator operator++(int) {
      iterator tmp = *this;
      ++(*this);
      return tmp;
    }
    iterator& operator--() {
      ptr_ = ptr_->prev;
      return *this;
    }
    iterator operator--(int) {
      iterator tmp = *this;
      --(*this);
      return tmp;
    }
    bool operator==(const iterator& other) const { return ptr_ == other.ptr_; }
    bool operator!=(const iterator& other) const { return ptr_ != other.ptr_; }
    reference operator*() const { return ptr_->data; }
    Node* getNode() const { return ptr_; }
  };

  class const_iterator {
   public:
    const Node* ptr_;
    const_iterator(const Node* ptr = nullptr) : ptr_(ptr) {}
    const_iterator(const iterator& it) : ptr_(it.ptr_) {}
    const_iterator& operator++() {
      ptr_ = ptr_->next;
      return *this;
    }
    const_iterator operator++(int) {
      const_iterator tmp = *this;
      ++(*this);
      return tmp;
    }
    const_iterator& operator--() {
      ptr_ = ptr_->prev;
      return *this;
    }
    const_iterator operator--(int) {
      const_iterator tmp = *this;
      --(*this);
      return tmp;
    }
    bool operator==(const const_iterator& other) const {
      return ptr_ == other.ptr_;
    }
    bool operator!=(const const_iterator& other) const {
      return ptr_ != other.ptr_;
    }
    const_reference operator*() const { return ptr_->data; }
    const Node* getNode() const { return ptr_; }
  };

  iterator begin() { return iterator(head_); }
  iterator end() { return iterator(nullptr); }
  const_iterator cbegin() const { return const_iterator(head_); }
  const_iterator cend() const { return const_iterator(nullptr); }

  void push_back(const_reference value) {
    Node* node = new Node(value, tail_, nullptr);
    if (tail_) tail_->next = node;
    tail_ = node;
    if (!head_) head_ = node;
    ++size_;
  }

  void pop_back() {
    if (tail_) {
      Node* toDelete = tail_;
      tail_ = tail_->prev;
      if (tail_)
        tail_->next = nullptr;
      else
        head_ = nullptr;
      delete toDelete;
      --size_;
    }
  }

  void push_front(const_reference value) {
    Node* node = new Node(value, nullptr, head_);
    if (head_) head_->prev = node; 
    head_ = node;
    if (!tail_) tail_ = node;
    ++size_;
  }

  void pop_front() {
    if (head_) {
      Node* toDelete = head_;
      head_ = head_->next;
      if (head_)
        head_->prev = nullptr;
      else
        tail_ = nullptr;
      delete toDelete;
      --size_;
    }
  }

  // \/ BONUS PART \/ BONUS PART \/ BONUS PART \/ BONUS PART

  template <typename... Args>
  void insert_many_back(Args&&... args) {
    (void)std::initializer_list<int>{
        (push_back(std::forward<Args>(args)), 0)...};
  }

  template <typename... Args>
  void insert_many_front(Args&&... args) {
    std::initializer_list<value_type> temp = {std::forward<Args>(args)...};
    for (auto it = temp.end(); it != temp.begin();) {
      --it;
      push_front(*it);
    }
  }

  template <typename... Args>
  iterator insert_many(const_iterator pos, Args&&... args) {
    Node* current = const_cast<Node*>(pos.getNode());
    Node* prev = current ? current->prev : nullptr;
    (void)std::initializer_list<int>{
        (insert_node_before(current, std::forward<Args>(args)), 0)...};
    if (prev)
      return iterator(prev->next);
    else
      return iterator(head_);
  }

 private:
  void insert_node_before(Node* node, const_reference value) {
    Node* newNode = new Node(value);
    if (node) {
      newNode->next = node;
      newNode->prev = node->prev;
      if (node->prev) node->prev->next = newNode;
      node->prev = newNode;
    } else {
      if (tail_) {
        tail_->next = newNode;
        newNode->prev = tail_;
        tail_ = newNode;
      } else {
        head_ = tail_ = newNode;
      }
    }
    ++size_;
  }

  // /\ BONUS PART /\ BONUS PART /\ BONUS PART /\ BONUS PART
 public:
  iterator insert(iterator pos, const_reference value) {
    if (pos == end()) {
      push_back(value);
      return iterator(tail_);
    } else {
      Node* nodePos = pos.getNode();
      Node* newNode = new Node(value, nodePos->prev, nodePos);
      if (nodePos->prev)
        nodePos->prev->next = newNode;
      else
        head_ = newNode;
      nodePos->prev = newNode;
      ++size_;
      return iterator(newNode);
    }
  }

  void erase(iterator pos) {
    if (pos == end()) return;
    Node* nodePos = pos.getNode();
    if (nodePos->prev) nodePos->prev->next = nodePos->next;
    if (nodePos->next) nodePos->next->prev = nodePos->prev;
    if (nodePos == head_) head_ = nodePos->next;
    if (nodePos == tail_) tail_ = nodePos->prev;
    delete nodePos;
    --size_;
  }

  void swap(list& other) {
    std::swap(head_, other.head_);
    std::swap(tail_, other.tail_);
    std::swap(size_, other.size_);
  }

  void reverse() {
    Node* current = head_;
    Node* temp = nullptr;
    while (current != nullptr) {
      temp = current->prev;
      current->prev = current->next;
      current->next = temp;
      current = current->prev;
    }
    temp = head_;
    head_ = tail_;
    tail_ = temp;
  }

  void unique() {
    Node* current = head_;
    while (current != nullptr && current->next != nullptr) {
      if (current->data == current->next->data) {
        erase(iterator(current->next));
      } else {
        current = current->next;
      }
    }
  }

  void sort() {
    if (head_ == nullptr || head_->next == nullptr) return;
    bool swapped = true;
    while (swapped) {
      swapped = false;
      for (Node* current = head_;
           current != nullptr && current->next != nullptr;
           current = current->next) {
        if (current->data > current->next->data) {
          std::swap(current->data, current->next->data);
          swapped = true;
        }
      }
    }
  }

  void merge(list& other) {
    iterator it = begin();
    while (!other.empty() && it != end()) {
      if (*it > other.front()) {
        insert(it, other.front());
        other.pop_front();
      } else {
        ++it;
      }
    }
    while (!other.empty()) {
      push_back(other.front());
      other.pop_front();
    }
  }

  void splice(const_iterator pos, list& other) {
    if (pos == cend()) {
      if (!other.empty()) {
        if (tail_) {
          tail_->next = other.head_;
          other.head_->prev = tail_;
        } else {
          head_ = other.head_;
        }
        tail_ = other.tail_;
        size_ += other.size_;
        other.head_ = nullptr;
        other.tail_ = nullptr;
        other.size_ = 0;
      }
    } else {
      Node* nodePos = const_cast<Node*>(pos.getNode());
      if (nodePos == head_) {
        if (other.head_) {
          other.tail_->next = head_;
          head_->prev = other.tail_;
          head_ = other.head_;
          size_ += other.size_;
          other.head_ = nullptr;
          other.tail_ = nullptr;
          other.size_ = 0;
        }
      } else {
        if (other.head_) {
          Node* prev = nodePos->prev;
          prev->next = other.head_;
          other.head_->prev = prev;
          other.tail_->next = nodePos;
          nodePos->prev = other.tail_;
          size_ += other.size_;
          other.head_ = nullptr;
          other.tail_ = nullptr;
          other.size_ = 0;
        }
      }
    }
  }
};

}  // namespace s21

#endif  // S21_LIST_H
