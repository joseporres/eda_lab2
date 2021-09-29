#pragma once

#include <vector>

#include "SpatialBase.h"

namespace utec {
namespace spatial {

template <typename T>
struct NodeBT {
  T data;
  int height;
  NodeBT *left;
  NodeBT *right;
  NodeBT() : left(nullptr), right(nullptr), height(0) {}
  NodeBT(T value) : data(value), left(nullptr), right(nullptr), height(0) {}

  void killSelf() {
    if (left != nullptr) left->killSelf();
    if (right != nullptr) right->killSelf();
    delete this;
  }
};

// auxiliar
template <typename T>
void mySwap(T &a, T &b) {
  T t = a;
  a = b;
  b = t;
}
int max(int &a, int &b) { return a > b ? a : b; }

/**
 * RangeTree1D implementation
 */
template <typename T>
class RangeTree1D : public SpatialBase<T> {
 private:
  NodeBT<T> *root;

  void calcHeight(NodeBT<T> *&node) {
    if (!node) return;

    if (node->left && node->right)
      node->height = max(node->left->height, node->right->height) + 1;
    else if (node->left)
      node->height = node->left->height + 1;
    else if (node->right)
      node->height = node->right->height + 1;
    else
      node->height = 0;
  }
  int height(NodeBT<T> *node) {
    if (node == nullptr)
      return -1;
    else
      return node->height;
  }
  bool isBalanced(NodeBT<T> *node) {
    if (node == nullptr)
      return true;
    else
      return abs(height(node->left) - height(node->right)) <= 1 &&
             isBalanced(node->left) && isBalanced(node->right);
  }
  T minValue(NodeBT<T> *node) {
    if (node == nullptr)
      throw("The tree is empty");
    else if (node->left == nullptr)
      return node->data;
    else
      return minValue(node->left);
  }
  T maxValue(NodeBT<T> *node) {
    if (node == nullptr)
      throw("The tree is empty");
    else if (node->right == nullptr)
      return node->data;
    else
      return maxValue(node->right);
  }
  int size(NodeBT<T> *node) {
    if (node == nullptr)
      return 0;
    else
      return 1 + size(node->left) + size(node->right);
  }
  void remove(NodeBT<T> *&node, T value) {
    if (node == nullptr)
      return;
    else if (value < node->data)
      remove(node->left, value);
    else if (value > node->data)
      remove(node->right, value);
    else {
      if (node->left == nullptr && node->right == nullptr) {
        delete node;  // free
        node = nullptr;
      } else if (node->left == nullptr) {
        NodeBT<T> *temp = node;
        node = node->right;
        delete temp;
      } else if (node->right == nullptr) {
        NodeBT<T> *temp = node;
        node = node->left;
        delete temp;
      } else {
        T temp = maxValue(node->left);
        node->data = temp;
        remove(node->left, temp);
      }
    }
    // if the node doesn't add a level the height should remain the same v:
    // calcHeight(node);
    balance(node);
  }

  /*add for avl*/
  int balancingFactor(NodeBT<T> *node) {
    return height(node->left) - height(node->right);
  }
  void insert(NodeBT<T> *&node, const T &value, NodeBT<T> *parent) {
    if (node == nullptr) {
      node = new NodeBT<T>(value);
      if (parent) {
        if (value < parent->data) {
          node->left = new NodeBT<T>(value);
        } else {
          parent->left = new NodeBT<T>(parent->data);
        }
      }
    } else if (value < node->data) {  // to the left
      insert(node->left, value, node);
      balance(node);
    } else {  // to the right
      insert(node->right, value, node);
      balance(node);
    }
    // if the node doesn't add a level the height should remain the same v:
    calcHeight(node);
  }
  void balance(NodeBT<T> *&node) {
    if (!node) return;

    int hb = balancingFactor(node);
    if (hb >= 2) {  // cargado por la izquierda
      if (balancingFactor(node->left) >= 1)
        right_rota(node);  // rotacion simple
      else {               //<=-1, rotacion doble izquierda-derecha
        left_rota(node->left);
        right_rota(node);
      }
    } else if (hb <= -2) {  // cargado por la derecha
      if (balancingFactor(node->right) <= -1)
        left_rota(node);  // rotacion simple
      else {              //>=1, rotacion doble derecha-izquierda
        right_rota(node->right);
        left_rota(node);
      }
    }
  }
  void left_rota(NodeBT<T> *&parent) {
    NodeBT<T> *temp = parent->right;
    parent->right = temp->left;
    temp->left = parent;
    mySwap(parent->height, temp->height);
    calcHeight(parent);
    parent = temp;
    calcHeight(parent);
  }
  void right_rota(NodeBT<T> *&parent) {
    NodeBT<T> *temp = parent->left;
    parent->left = temp->right;
    temp->right = parent;
    mySwap(parent->height, temp->height);
    calcHeight(parent);
    parent = temp;
    calcHeight(parent);
  }

  NodeBT<T> *findSplitNode(const T &a, const T &b) {
    auto v = root;
    while (v->left && (b <= v->data || a > v->data)) {
      if (b <= v->data) {
        v = v->left;
      } else {
        v = v->right;
      }
    }
    return v;
  }

  void reportSubtree(NodeBT<T> *node, std::vector<T> &result) {
    if (node == nullptr) return;
    reportSubtree(node->left, result);
    if (!node->left) result.push_back(node->data);
    reportSubtree(node->right, result);
  }

 public:
  RangeTree1D() : root(nullptr) {}

  int height() { return height(this->root); }
  T minValue() { return minValue(this->root); }
  T maxValue() { return maxValue(this->root); }
  bool isBalanced() { return isBalanced(this->root); }
  int size() { return size(this->root); }
  void remove(T value) { remove(this->root, value); }
  ~RangeTree1D() {
    if (this->root != nullptr) {
      this->root->killSelf();
    }
  }

 public:
  void insert(const T &new_point) override {
    insert(this->root, new_point, nullptr);
  }

  // El punto de referencia no necesariamente es parte del dataset
  T nearest_neighbor(const T &reference) override { return T({0}); }
  std::vector<T> range(const T &min, const T &max) override {
    std::vector<T> result;
    auto vsplit = findSplitNode(min, max);
    auto v = vsplit;
    if (!v->left) {
      if (v->data >= min) result.push_back(v->data);
    } else {
      v = v->left;
      while (v->left) {
        if (min <= v->data) {
          reportSubtree(v->right, result);
          v = v->left;
        } else {
          v = v->right;
        }
      }
      if (v->data >= min) result.push_back(v->data);
    }
    v = vsplit->right;
    while (v->right) {
      if (max >= v->data) {
        reportSubtree(v->left, result);
        v = v->right;
      } else {
        v = v->left;
      }
    }
    if (v->data <= max) result.push_back(v->data);
    return result;
  };
};

}  // namespace spatial
}  // namespace utec

/* add for AVL*/
