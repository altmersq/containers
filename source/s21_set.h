#ifndef S21_SET_H
#define S21_SET_H

#include "rb_tree.h"
#include "s21_vector.h"

namespace s21 {
template <typename Key>
class set {
 public:
  using key_type = Key;
  using value_type = Key;
  using reference = value_type &;
  using const_reference = const value_type &;
  using tree_type = rb_tree<key_type, std::less<key_type>>;
  using iterator = typename tree_type::iterator;
  using const_iterator = typename tree_type::const_iterator;
  using size_type = size_t;

 private:
  tree_type *tree_;

 public:
  set() : tree_(new tree_type()){};
  set(std::initializer_list<value_type> const &items) : set() {
    for (auto item : items) {
      bool insterted;
      tree_->insert(item, insterted);
    }
  }
  set(const set &other) : tree_(nullptr) {
    if (other.tree_) {
      tree_ = new tree_type(*other.tree_);
    }
  }
  set(set &&s) : tree_(s.tree_) { s.tree_ = nullptr; }
  ~set() { delete tree_; }
  set &operator=(set &&other) noexcept {
    if (this != &other) {
      delete tree_;
      tree_ = other.tree_;
      other.tree_ = nullptr;
    }
    return *this;
  }

  set &operator=(const set &other) {
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
  std::pair<iterator, bool> insert(const value_type &value) {
    bool inserted;
    tree_->insert(value, inserted);
    return std::make_pair(iterator(tree_->find(value)), inserted);
  }
  void erase(iterator pos) {
    if (pos == end()) {
      return;
    }
    tree_->remove(tree_->get_key(tree_->find(*pos)));
  }
  void swap(set &other) { std::swap(tree_, other.tree_); }
  void merge(set &other) {
    tree_->merge(*other.tree_);
    other.clear();
  }

  iterator find(const Key &key) { return tree_->find(key); }
  bool contains(const Key &key) { return tree_->find(key) ? true : false; }

  template <typename... Args>
  s21::vector<std::pair<iterator, bool>> insert_many(Args &&...args) {
    s21::vector<std::pair<iterator, bool>> results;
    (results.push_back(insert(std::forward<Args>(args))), ...);
    return results;
  }
};
};  // namespace s21

#endif