#ifndef S21_MULTISET_H
#define S21_MULTISET_H

#include "../../source/s21_vector.h"
#include "rb_tree_multi.h"

namespace s21 {
template <typename Key>
class multiset {
 public:
  using key_type = Key;
  using value_type = Key;
  using reference = value_type&;
  using const_reference = const value_type&;
  using tree_type = rb_tree_multi<key_type, value_type>;
  using iterator = typename tree_type::iterator;
  using const_iterator = typename tree_type::const_iterator;
  using size_type = size_t;

 private:
  tree_type* tree_;

 public:
  multiset() : tree_(new tree_type()) {}
  multiset(std::initializer_list<value_type> const& items) : multiset() {
    for (auto item : items) {
      bool inserted;
      tree_->insert(item, inserted);
    }
  }
  multiset(const multiset& other) : tree_(nullptr) {
    if (other.tree_) {
      tree_ = new tree_type(*other.tree_);
    }
  }
  multiset(multiset&& s) : tree_(s.tree_) { s.tree_ = nullptr; }
  ~multiset() { delete tree_; }
  multiset& operator=(multiset&& other) noexcept {
    if (this != &other) {
      delete tree_;
      tree_ = other.tree_;
      other.tree_ = nullptr;
    }
    return *this;
  }

  multiset& operator=(const multiset& other) {
    if (this != &other) {
      delete tree_;
      tree_ = new tree_type(*(other.tree_));
    }
    return *this;
  }

  iterator begin() { return tree_->begin(); }
  iterator end() { return tree_->end(); }
  const_iterator cbegin() { return tree_->cbegin(); }
  const_iterator cend() { return tree_->cend(); }

  bool empty() const { return tree_->size() == 0; }
  size_type size() { return tree_->size(); }
  size_type max_size() { return tree_->max_size(); }

  void clear() noexcept {
    if (tree_) {
      delete tree_;
      tree_ = new tree_type();
    }
  }
  std::pair<iterator, bool> insert(const value_type& value) {
    bool inserted;
    tree_->insert(value, inserted);
    return std::make_pair(iterator(tree_->find(value)), inserted);
  }
  void erase(iterator pos) {
    if (pos == end()) {
      return;
    }
    tree_->remove(*pos);
  }
  void swap(multiset& other) { std::swap(tree_, other.tree_); }
  void merge(multiset& other) {
    tree_->merge(*other.tree_);
    other.clear();
  }

  size_t count(const Key& key) {
    size_t counter = 0;
    for (auto it = find(key); it != end() && *it == key; ++it) {
      ++counter;
    }
    return counter;
  }

  std::pair<iterator, iterator> equal_range(const Key& key) {
    iterator first = find(key);
    if (first == end()) {
      return std::make_pair(end(), end());
    }
    iterator last = first;
    while (last != end() && *last == key) {
      ++last;
    }
    return std::make_pair(first, last);
  }

  iterator lower_bound(const Key& key) {
    iterator it_find = find(key);
    if (it_find != end()) {
      return it_find;
    }
    for (iterator it = begin(); it != end(); ++it) {
      if (*it >= key) {
        return it;
      }
    }
    return end();
  }

  iterator upper_bound(const Key& key) {
    iterator it_find = find(key);
    if (it_find != end()) {
      ++it_find;
      return it_find;
    } else {
      for (iterator it = begin(); it != end(); ++it) {
        if (*it > key) {
          return it;
        }
      }
    }
    return end();
  }

  iterator find(const Key& key) {
    for (iterator it = begin(); it != end(); ++it) {
      if (*it == key) {
        return it;
      }
    }
    return end();
  }
  bool contains(const Key& key) { return tree_->find(key) ? true : false; }

  template <typename... Args>
  s21::vector<std::pair<iterator, bool>> insert_many(Args&&... args) {
    s21::vector<std::pair<iterator, bool>> results;
    (results.push_back(insert(std::forward<Args>(args))), ...);
    return results;
  }
};
}  // namespace s21

#endif
