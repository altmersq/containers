#ifndef S21_VECTOR_H
#define S21_VECTOR_H

#include <initializer_list>
#include <iostream>
#include <limits>

namespace s21 {
template <typename T>
class vector {
 public:
  using value_type = T;
  using reference = T &;
  using const_reference = const T &;
  using iterator = T *;
  using const_iterator = T *;
  using size_type = size_t;

 private:
  T *data_;
  size_type capacity_;
  size_type size_;
  std::allocator<T> allocator_;

 public:
  vector() : data_(nullptr), capacity_(0), size_(0), allocator_() {}

  explicit vector(size_t n)
      : data_(allocator_.allocate(n)), capacity_(n), size_(n), allocator_() {
    std::uninitialized_fill_n(data_, n, T());
  }

  vector(std::initializer_list<T> const &items)
      : data_(allocator_.allocate(items.size())),
        capacity_(items.size()),
        size_(items.size()),
        allocator_() {
    std::uninitialized_copy(items.begin(), items.end(), data_);
  }

  vector(const vector &v)
      : data_(allocator_.allocate(v.capacity_)),
        capacity_(v.capacity_),
        size_(v.size_),
        allocator_(v.allocator_) {
    std::uninitialized_copy(v.data_, v.data_ + v.size_, data_);
  }

  vector(vector &&v) noexcept
      : data_(v.data_),
        capacity_(v.capacity_),
        size_(v.size_),
        allocator_(std::move(v.allocator_)) {
    v.data_ = nullptr;
    v.capacity_ = 0;
    v.size_ = 0;
  }

  ~vector() {
    clear();
    allocator_.deallocate(data_, capacity_);
  }

  vector &operator=(const vector &v) {
    if (this != &v) {
      clear();
      allocator_.deallocate(data_, capacity_);
      capacity_ = v.capacity_;
      size_ = v.size_;
      data_ = allocator_.allocate(capacity_);
      std::uninitialized_copy(v.data_, v.data_ + v.size_, data_);
    }
    return *this;
  }

  vector &operator=(vector &&v) noexcept {
    if (this != &v) {
      clear();
      allocator_.deallocate(data_, capacity_);
      data_ = v.data_;
      size_ = v.size_;
      capacity_ = v.capacity_;
      v.data_ = nullptr;
      v.size_ = 0;
      v.capacity_ = 0;
    }
    return *this;
  }

  reference at(size_type pos) {
    if (pos >= size_) {
      throw std::out_of_range("Index out of range");
    }
    return data_[pos];
  }

  reference operator[](size_type pos) { return data_[pos]; }

  const_reference front() const { return data_[0]; }

  const_reference back() const { return data_[size_ - 1]; }

  T *data() { return data_; }

  iterator begin() { return iterator(data_); }

  iterator end() { return iterator(data_ + size_); }

  const_iterator cbegin() { return const_iterator(data_); }

  const_iterator cend() { return const_iterator(data_ + size_); }

  bool empty() { return size_ == 0; }

  size_t size() { return size_; }

  size_type max_size() const {
    return std::numeric_limits<std::size_t>::max() / sizeof(value_type) / 2;
  }

  void reserve(size_type size) {
    if (size > max_size()) {
      throw std::out_of_range("Size can't be bigger than max size of vector");
    }
    if (size > capacity_) {
      T *new_data = allocator_.allocate(size);
      std::uninitialized_copy(data_, data_ + size_, new_data);
      for (size_t i = 0; i < size_; ++i) {
        allocator_.destroy(&data_[i]);
      }
      allocator_.deallocate(data_, capacity_);
      data_ = new_data;
      capacity_ = size;
    }
  }

  size_type capacity() const { return capacity_; }

  void shrink_to_fit() {
    if (capacity_ > size_) {
      T *new_data = allocator_.allocate(size_);
      std::uninitialized_copy(data_, data_ + size_, new_data);
      for (size_t i = 0; i < size_; ++i) {
        allocator_.destroy(&data_[i]);
      }
      allocator_.deallocate(data_, capacity_);
      data_ = new_data;
      capacity_ = size_;
    }
  }

  void clear() noexcept {
    for (size_t i = 0; i < size_; ++i) {
      allocator_.destroy(&data_[i]);
    }
    size_ = 0;
  }

  iterator insert(iterator pos, const_reference value) {
    if (pos >= begin() && pos <= end()) {
      size_type index = static_cast<size_type>(pos - begin());
      if (size_ == capacity_) {
        reserve(capacity_ + 1);
      }
      for (size_type i = size_; i > index; --i) {
        allocator_.construct(&data_[i], std::move(data_[i - 1]));
        allocator_.destroy(&data_[i - 1]);
      }
      allocator_.construct(&data_[index], value);
      ++size_;
      return begin() + index;
    } else {
      throw std::out_of_range("Invalid pos of value");
    }
  }

  void erase(iterator pos) {
    if (pos >= begin() && pos < end()) {
      size_type index = static_cast<size_type>(pos - begin());
      std::move(data_ + index + 1, data_ + size_, data_ + index);
      --size_;
      allocator_.destroy(data_ + size_);
    } else {
      throw std::out_of_range("Invalid iterator");
    }
  }

  void push_back(const T &value) {
    if (size_ == capacity_) {
      reserve(capacity_ == 0 ? 1 : 2 * capacity_);
    }
    allocator_.construct(data_ + size_, value);
    ++size_;
  }

  void pop_back() {
    if (size_ > 0) {
      --size_;
      allocator_.destroy(data_ + size_);
    }
  }

  void swap(vector &other) {
    std::swap(data_, other.data_);
    std::swap(size_, other.size_);
    std::swap(capacity_, other.capacity_);
  }

  template <typename... Args>
  iterator insert_many(const_iterator pos, Args &&...args) {
    size_t index = static_cast<size_t>(pos - cbegin());
    size_t num_elements = sizeof...(args);
    size_t new_size = size_ + num_elements;

    if (new_size > capacity_) {
      reserve(new_size);
    }

    iterator it = begin() + index;
    std::move_backward(it, end(), end() + num_elements);

    size_t i = 0;
    (void)std::initializer_list<int>{
        (data_[index + i++] = std::forward<Args>(args), 0)...};

    size_ = new_size;
    return it;
  }

  template <typename... Args>
  void insert_many_back(Args &&...args) {
    size_t new_size = size_ + sizeof...(args);
    if (new_size > capacity_) {
      reserve(new_size);
    }
    (push_back(std::forward<Args>(args)), ...);
  }

  void reallocate(size_t new_capacity) {
    T *new_data = allocator_.allocate(new_capacity);
    for (size_t i = 0; i < size_; ++i) {
      allocator_.construct(&new_data[i], std::move(data_[i]));
      allocator_.destroy(&data_[i]);
    }
    allocator_.deallocate(data_, capacity_);
    data_ = new_data;
    capacity_ = new_capacity;
  }

  void print() const {
    for (size_type i = 0; i < size_; ++i) {
      std::cout << data_[i] << " ";
    }
    std::cout << std::endl;
  }
};
};  // namespace s21

#endif