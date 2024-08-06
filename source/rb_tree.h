#ifndef RED_BLACK_TREE
#define RED_BLACK_TREE

#include <iostream>
#include <iterator>
#include <limits>
#include <string>

namespace s21 {
template <typename Key, typename Value>
class rb_tree {
 private:
  enum Color { RED, BLACK };

  struct Node {
    Key key;
    Value value;
    Node* parent;
    Node* left;
    Node* right;
    Color color;

    Node(const Node& other)
        : key(other.key),
          value(other.value),
          parent(nullptr),
          left(nullptr),
          right(nullptr),
          color(other.color) {}

    Node& operator=(const Node& other) {
      if (this != &other) {
        key = other.key;
        value = other.value;
        parent = nullptr;
        left = nullptr;
        right = nullptr;
        color = other.color;
      }
      return *this;
    }

    Node(const Key& k, const Value& v, Node* p = nullptr, Node* l = nullptr,
         Node* r = nullptr, Color c = RED)
        : key(k), value(v), parent(p), left(l), right(r), color(c) {}
  };

  Node* root_;

  void destroy_tree(Node* node) {
    if (node) {
      destroy_tree(node->left);
      destroy_tree(node->right);
      delete node;
    }
  }

  Node* copyTree(Node* srcNode, Node* parent) {
    if (!srcNode) {
      return nullptr;
    }

    Node* newNode = new Node(srcNode->key, srcNode->value, parent);
    newNode->color = srcNode->color;
    newNode->left = copyTree(srcNode->left, newNode);
    newNode->right = copyTree(srcNode->right, newNode);

    return newNode;
  }

  rb_tree& operator=(const rb_tree& other) {
    if (this != &other) {
      destroy_tree(root_);
      root_ = copyTree(other.root_, nullptr);
    }
    return *this;
  }

  void rotateLeft(Node* x) {
    if (x == nullptr || x->right == nullptr) {
      return;
    }
    Node* y = x->right;
    x->right = y->left;
    if (y->left != nullptr) y->left->parent = x;
    y->parent = x->parent;
    if (x->parent == nullptr)
      root_ = y;
    else if (x == x->parent->left)
      x->parent->left = y;
    else
      x->parent->right = y;
    y->left = x;
    x->parent = y;
  }

  void rotateRight(Node* y) {
    if (y == nullptr || y->right == nullptr) {
      return;
    }
    Node* x = y->left;
    y->left = x->right;
    if (x->right != nullptr) x->right->parent = y;
    x->parent = y->parent;
    if (y->parent == nullptr)
      root_ = x;
    else if (y == y->parent->right)
      y->parent->right = x;
    else
      y->parent->left = x;
    x->right = y;
    y->parent = x;
  }

  void insertFixup(Node* newNode) {
    while (newNode != nullptr && newNode != root_ &&
           newNode->parent->color == Color::RED) {
      if (newNode->parent == newNode->parent->parent->left) {
        Node* uncle = newNode->parent->parent->right;
        if (uncle != nullptr && uncle->color == Color::RED) {
          newNode->parent->color = Color::BLACK;
          uncle->color = Color::BLACK;
          newNode->parent->parent->color = Color::RED;
          newNode = newNode->parent->parent;
        } else {
          if (newNode == newNode->parent->right) {
            newNode = newNode->parent;
            rotateLeft(newNode);
          }
          newNode->parent->color = Color::BLACK;
          newNode->parent->parent->color = Color::RED;
          rotateRight(newNode->parent->parent);
        }
      } else {
        Node* uncle = newNode->parent->parent->left;
        if (uncle != nullptr && uncle->color == Color::RED) {
          newNode->parent->color = Color::BLACK;
          uncle->color = Color::BLACK;
          newNode->parent->parent->color = Color::RED;
          newNode = newNode->parent->parent;
        } else {
          if (newNode == newNode->parent->left) {
            newNode = newNode->parent;
            rotateRight(newNode);
          }
          newNode->parent->color = Color::BLACK;
          newNode->parent->parent->color = Color::RED;
          rotateLeft(newNode->parent->parent);
        }
      }
    }
    root_->color = Color::BLACK;
  }

  void insert(Node*& node, Node* parent, const Key& key, const Value& value,
              Node*& inserted_node, bool& inserted) {
    if (node == nullptr) {
      node = new Node(key, value);
      node->parent = parent;
      if (parent != nullptr) {
        if (key < parent->key)
          parent->left = node;
        else
          parent->right = node;
      }
      insertFixup(node);
      inserted_node = node;
      inserted = true;
    } else {
      if (key < node->key)
        insert(node->left, node, key, value, inserted_node, inserted);
      else if (key > node->key)
        insert(node->right, node, key, value, inserted_node, inserted);
      else {
        node->value = value;
        inserted_node = node;
        inserted = false;
      }
    }
  }

  void remove(Node*& root, const Key& key) {
    if (root == nullptr) return;

    if (key < root->key)
      remove(root->left, key);
    else if (key > root->key)
      remove(root->right, key);
    else {
      if (root->left == nullptr) {
        Node* temp = root->right;
        delete root;
        root = temp;
      } else if (root->right == nullptr) {
        Node* temp = root->left;
        delete root;
        root = temp;
      } else {
        Node* temp = minValueNode(root->right);
        root->key = temp->key;
        remove(root->right, temp->key);
      }
      if (root != nullptr) {
        balance(root, root->parent);
      }
    }
  }

  Node* minValueNode(Node* node) {
    Node* current = node;
    while (current->left != nullptr) {
      current = current->left;
    }
    return current;
  }

  Color getColor(Node* node) const {
    return (node != nullptr) ? node->color : BLACK;
  }

  void balance(Node*& node, Node* parent) {
    while (node != root_ &&
           (node == nullptr || getColor(node) == Color::BLACK)) {
      if (node == parent->left) {
        Node* sibling = parent->right;
        if (getColor(sibling) == Color::RED) {
          sibling->color = Color::BLACK;
          parent->color = Color::RED;
          rotateLeft(parent);
          sibling = parent->right;
        }
        if ((sibling->left == nullptr ||
             getColor(sibling->left) == Color::BLACK) &&
            (sibling->right == nullptr ||
             getColor(sibling->right) == Color::BLACK)) {
          sibling->color = Color::RED;
          node = parent;
          parent = parent->parent;
        } else {
          if (sibling->right == nullptr ||
              getColor(sibling->right) == Color::BLACK) {
            sibling->left->color = Color::BLACK;
            sibling->color = Color::RED;
            rotateRight(sibling);
            sibling = parent->right;
          }
          sibling->color = getColor(parent);
          parent->color = Color::BLACK;
          sibling->right->color = Color::BLACK;
          rotateLeft(parent);
          node = root_;
        }
      } else {
        Node* sibling = parent->left;
        if (getColor(sibling) == Color::RED) {
          sibling->color = Color::BLACK;
          parent->color = Color::RED;
          rotateRight(parent);
          sibling = parent->left;
        }
        if ((sibling->right == nullptr ||
             getColor(sibling->right) == Color::BLACK) &&
            (sibling->left == nullptr ||
             getColor(sibling->left) == Color::BLACK)) {
          sibling->color = Color::RED;
          node = parent;
          parent = parent->parent;
        } else {
          if (sibling->left == nullptr ||
              getColor(sibling->left) == Color::BLACK) {
            sibling->right->color = Color::BLACK;
            sibling->color = Color::RED;
            rotateLeft(sibling);
            sibling = parent->left;
          }
          sibling->color = getColor(parent);
          parent->color = Color::BLACK;
          sibling->left->color = Color::BLACK;
          rotateRight(parent);
          node = root_;
        }
      }
    }
    if (node != nullptr) node->color = Color::BLACK;
  }

  Node* find(Node* node, const Key& key) {
    Node* current = node;
    while (current != nullptr) {
      if (key == current->key) {
        return current;
      } else if (key < current->key) {
        current = current->left;
      } else {
        current = current->right;
      }
    }
    return nullptr;
  }

  size_t size(Node* node) {
    return node == nullptr ? 0 : size(node->left) + size(node->right) + 1;
  }

 public:
  class iterator {
   private:
    Node* current_;

   public:
    iterator(Node* ptr) : current_(ptr) {}

    friend bool operator==(const iterator& a, const iterator& b) noexcept {
      return a.current_ == b.current_;
    }

    friend bool operator!=(const iterator& a, const iterator& b) noexcept {
      return a.current_ != b.current_;
    }

    iterator& operator++() {
      if (current_ == nullptr) return *this;

      if (current_->right != nullptr) {
        current_ = current_->right;
        while (current_->left != nullptr) {
          current_ = current_->left;
        }
      } else {
        Node* parent = current_->parent;
        while (parent != nullptr && current_ == parent->right) {
          current_ = parent;
          parent = parent->parent;
        }
        current_ = parent;
      }
      return *this;
    }

    iterator& operator--() {
      if (current_ == nullptr) return *this;

      if (current_->left != nullptr) {
        current_ = current_->left;
        while (current_->right != nullptr) {
          current_ = current_->right;
        }
      } else {
        Node* parent = current_->parent;
        while (parent != nullptr && current_ == parent->left) {
          current_ = parent;
          parent = parent->parent;
        }
        current_ = parent;
      }
      return *this;
    }

    iterator operator--(int) {
      iterator tmp = *this;
      --(*this);
      return tmp;
    }

    // Оператор разыменования
    const Key& operator*() { return current_->key; }
    Key* operator->() { return &current_->key; }
  };

 public:
  class const_iterator {
   private:
    const Node* current_;

   public:
    using key_type = Key;
    const_iterator(const Node* ptr) : current_(ptr) {}

    friend bool operator==(const const_iterator& a,
                           const const_iterator& b) noexcept {
      return a.current_ == b.current_;
    }

    friend bool operator!=(const const_iterator& a,
                           const const_iterator& b) noexcept {
      return a.current_ == b.current_;
    }

    const_iterator& operator++() const {
      if (current_ == nullptr) return *this;

      if (current_->right != nullptr) {
        current_ = current_->right;
        while (current_->left != nullptr) {
          current_ = current_->left;
        }
      } else {
        Node* parent = current_->parent;
        while (parent != nullptr && current_ == parent->right) {
          current_ = parent;
          parent = parent->parent;
        }
        current_ = parent;
      }
      return *this;
    }

    const_iterator operator--(int) const {
      const_iterator tmp = *this;
      --(*this);
      return tmp;
    }

    // Оператор разыменования
    const Key& operator*() const noexcept { return current_->key; }
  };

  iterator begin() {
    if (root_ == nullptr) {
      return end();
    } else {
      Node* leftmost = root_;
      while (leftmost->left != nullptr) {
        leftmost = leftmost->left;
      }
      return iterator(leftmost);
    }
  }
  iterator end() { return iterator(nullptr); }

  const_iterator cbegin() const {
    if (root_ == nullptr) {
      return end();
    } else {
      Node* leftmost = root_;
      while (leftmost->left != nullptr) {
        leftmost = leftmost->left;
      }
      return iterator(leftmost);
    }
  }
  const_iterator cend() const { return iterator(nullptr); }

  rb_tree() : root_(nullptr) {}

  ~rb_tree() { destroy_tree(root_); }

  rb_tree(const rb_tree& other) : root_(nullptr) {
    root_ = copyTree(other.root_, nullptr);
  }

  std::pair<iterator, bool> insert(const Key& key, bool& inserted) {
    Node* inserted_node = nullptr;
    insert(root_, nullptr, key, Value(), inserted_node, inserted);
    return std::make_pair(iterator(inserted_node), inserted);
  }

  void remove(const Key& key) { remove(root_, key); }

  Node* find(const Key& key) { return find(root_, key); }

  void merge(rb_tree& other) {
    for (const auto& elem : other) {
      bool inserted;
      insert(elem, inserted);
    }
  }

  size_t size() { return size(root_); }

  size_t max_size() {
    return std::numeric_limits<std::size_t>::max() / sizeof(root_) / 2;
  }

  const Key& get_key(const Node* node) const { return node->key; }

  Node* getRoot() const { return root_; }

  void print_in_line(Node* root) {
    if (root == nullptr) return;

    print_in_line(root->left);
    std::cout << root->key << ": ";
    // std::cout << root->value << "; ";
    print_in_line(root->right);
  }

  std::pair<iterator, bool> map_insert(const Key& key, const Value& val,
                                       bool& inserted) {
    Node* inserted_node = nullptr;
    insert(root_, nullptr, key, val, inserted_node, inserted);
    return std::make_pair(iterator(inserted_node), inserted);
  }

  Value& at_map(const Key& key) {
    Node* node = find(key);
    if (node == end()) {
      throw std::out_of_range("Key not found");
    }
    return node->value;
  }
};
};  // namespace s21

#endif