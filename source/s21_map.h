#ifndef S21_map_H
#define S21_map_H

#include "rb_tree.h"
#include "s21_vector.h"

namespace s21 {
template <typename Key, typename Value>
class map {
 public:
  using key_type = Key;
  using mapped_type = Value;
  using value_type = std::pair<const Key, Value>;
  using reference = value_type &;
  using const_reference = const value_type &;
  using tree_type = rb_tree<key_type, mapped_type>;
  using iterator = typename tree_type::iterator;
  using const_iterator = typename tree_type::const_iterator;
  using size_type = size_t;

 private:
  tree_type *tree_;

 public:
  map() : tree_(new tree_type()) {}
  map(std::initializer_list<value_type> const &items) : map() {
    for (auto item : items) {
      bool insterted;
      tree_->map_insert(item.first, item.second, insterted);
    }
  }
  map(const map &other) : tree_(nullptr) {
    if (other.tree_) {
      tree_ = new tree_type(*other.tree_);
    }
  }
  map(map &&s) : tree_(s.tree_) { s.tree_ = nullptr; }
  ~map() { delete tree_; }
  map &operator=(map &&s) noexcept {
    if (this != &s) {
      delete tree_;
      tree_ = s.tree_;
      s.tree_ = nullptr;
    }
    return *this;
  }

  map &operator=(const map &s) {
    if (this != &s) {
      delete tree_;
      tree_ = new tree_type(*(s.tree_));
    }
    return *this;
  }

  mapped_type &at(const key_type &key) { return tree_->at_map(key); }
  mapped_type &operator[](const key_type &key) {
    if (tree_->find(key)) {
      return at(key);
    } else {
      bool inserted;
      std::pair<iterator, bool> res =
          tree_->map_insert(key, mapped_type{}, inserted);
      return at((*res.first));
    }
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
    tree_->map_insert(value.first, value.second, inserted);
    return std::make_pair(iterator(tree_->find(value.first)), inserted);
  }
  std::pair<iterator, bool> insert(const Key &key, const Value &obj) {
    bool inserted;
    tree_->map_insert(key, obj, inserted);
    return std::make_pair(iterator(tree_->find(key)), inserted);
  }
  std::pair<iterator, bool> insert_or_assign(const Key &key, const Value &obj) {
    bool inserted;
    if (iterator(tree_->find(key)) != end()) {
      at(key) = obj;
      inserted = true;
    } else {
      tree_->map_insert(key, obj, inserted);
    }
    return std::make_pair(iterator(tree_->find(key)), inserted);
  }
  void erase(iterator pos) {
    if (pos == end()) {
      return;
    }
    tree_->remove(tree_->get_key(tree_->find(*pos)));
  }
  void swap(map &other) { std::swap(tree_, other.tree_); }
  void merge(map &other) {
    for (auto &pair : other) {
      if (!(tree_->find(*(&pair)))) {
        bool inserted;
        tree_->map_insert(*(&pair), other.at(pair), inserted);
      }
    }
    other.clear();
  }

  bool contains(const Key &key) { return tree_->find(key) ? true : false; }

  template <typename... Args>
  s21::vector<std::pair<iterator, bool>> insert_many(Args &&...args) {
    s21::vector<std::pair<iterator, bool>> results;
    (results.push_back(insert(std::forward<Args>(args))), ...);
    return results;
  }

  void print() {
    for (auto &pair : *tree_) {
      std::cout << pair << " : " << at(pair) << std::endl;
    }
  }
};
};  // namespace s21

#endif