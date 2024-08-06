#ifndef S21_ARRAY_H
#define S21_ARRAY_H

#include <initializer_list>
#include <iostream>
#include <stdexcept>

namespace s21 {

template <typename T, std::size_t N>
class array {
 public:
  using value_type = T;
  using reference = T&;
  using const_reference = const T&;
  using iterator = T*;
  using const_iterator = const T*;
  using size_type = std::size_t;

 private:
  value_type data_[N];

 public:
  array() = default;

  array(std::initializer_list<value_type> il) {
    if (il.size() > N)
      throw std::out_of_range("Initializer list size exceeds array capacity");
    std::copy(il.begin(), il.end(), data_);
  }

  array(const array& other) { std::copy(other.data_, other.data_ + N, data_); }

  array(array&& other) noexcept {
    std::move(other.data_, other.data_ + N, data_);
  }

  ~array() = default;

  array& operator=(const array& other) {
    if (this != &other) {
      std::copy(other.data_, other.data_ + N, data_);
    }
    return *this;
  }

  array& operator=(array&& other) noexcept {
    if (this != &other) {
      std::move(other.data_, other.data_ + N, data_);
    }
    return *this;
  }

  reference at(size_type pos) {
    if (pos >= N) throw std::out_of_range("Index out of range");
    return data_[pos];
  }

  reference operator[](size_type pos) { return data_[pos]; }

  const_reference operator[](size_type pos) const { return data_[pos]; }

  const_reference front() const { return data_[0]; }

  const_reference back() const { return data_[N - 1]; }

  iterator data() { return data_; }

  const_iterator data() const { return data_; }

  iterator begin() { return data_; }

  const_iterator begin() const { return data_; }

  iterator end() { return data_ + N; }

  const_iterator end() const { return data_ + N; }

  bool empty() const { return N == 0; }

  size_type size() const { return N; }

  size_type max_size() const { return N; }

  void swap(array& other) {
    for (size_type i = 0; i < N; ++i) {
      std::swap(data_[i], other.data_[i]);
    }
  }

  void fill(const_reference value) { std::fill(data_, data_ + N, value); }
};

}  // namespace s21

#endif  // S21_ARRAY_H
