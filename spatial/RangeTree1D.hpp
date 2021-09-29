#pragma once

#include "SpatialBase.h"
#include "node.h"

namespace utec {
namespace spatial {

/**
 * RangeTree1D implementation
 */
template <typename Point>
class RangeTree1D : public SpatialBase<Point> {
 private:
  NodeBT<Point> root;

  void calcHeight(NodeBT<Point> *&);
  int height(NodeBT<Point> *node);
  bool isBalanced(NodeBT<Point> *node);
  Point maxValue(NodeBT<Point> *node);
  int size(NodeBT<Point> *node);
  void remove(NodeBT<Point> *&node, Point value);

  int balancingFactor(NodeBT<Point> *node);
  void insert(NodeBT<Point> *&node, Point value);
  void balance(NodeBT<Point> *&node);
  void left_rota(NodeBT<Point> *&node);
  void right_rota(NodeBT<Point> *&node);

 public:
  void insert(const Point &value) override { insert(this->root, value); }

  // El punto de referencia no necesariamente es parte del dataset
  Point nearest_neighbor(const Point &reference) override { return Point({0}); }
  std::vector<Point> range(const Point &min, const Point &max) override {
    return {};
  };

 public:
  RangeTree1D() : root(nullptr) {}

  int height() { return height(this->root); }
  Point maxValue() { return maxValue(this->root); }
  bool isBalanced() { return isBalanced(this->root); }
  int size() { return size(this->root); }
  void remove(Point value) { remove(this->root, value); }
  ~RangeTree1D() {
    if (this->root != nullptr) {
      this->root->killSelf();
    }
  }
};

// auxiliar
template <typename Point>
void mySwap(Point &a, Point &b) {
  Point t = a;
  a = b;
  b = t;
}
int max(int &a, int &b) { return a > b ? a : b; }

template <typename Point>
void RangeTree1D<Point>::calcHeight(NodeBT<Point> *&node) {
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

template <typename Point>
bool RangeTree1D<Point>::isBalanced(NodeBT<Point> *node) {
  if (node == nullptr)
    return true;
  else
    return abs(height(node->left) - height(node->right)) <= 1 &&
           isBalanced(node->left) && isBalanced(node->right);
}

template <typename Point>
Point RangeTree1D<Point>::maxValue(NodeBT<Point> *node) {
  if (node == nullptr)
    throw("Pointhe tree is empty");
  else if (node->right == nullptr)
    return node->data;
  else
    return maxValue(node->right);
}

template <typename Point>
int RangeTree1D<Point>::size(NodeBT<Point> *node) {
  if (node == nullptr)
    return 0;
  else
    return 1 + size(node->left) + size(node->right);
}

template <typename Point>
void RangeTree1D<Point>::insert(NodeBT<Point> *&node, Point value) {
  if (node == nullptr)
    node = new NodeBT<Point>(value);
  else if (value < node->data) {
    insert(node->left, value);
    balance(node);
  } else {
    insert(node->right, value);
    balance(node);
  }
  // if the node doesn't add a level the height should remain the same v:
  calcHeight(node);
}

template <typename Point>
int RangeTree1D<Point>::height(NodeBT<Point> *node) {
  if (node == nullptr)
    return -1;
  else
    return node->height;
}

template <typename Point>
int RangeTree1D<Point>::balancingFactor(NodeBT<Point> *node) {
  return height(node->left) - height(node->right);
}

template <typename Point>
void RangeTree1D<Point>::balance(NodeBT<Point> *&node) {
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

template <typename Point>
void RangeTree1D<Point>::left_rota(NodeBT<Point> *&parent) {
  NodeBT<Point> *temp = parent->right;
  parent->right = temp->left;
  temp->left = parent;
  mySwap(parent->height, temp->height);
  calcHeight(parent);
  parent = temp;
  calcHeight(parent);
}

template <typename Point>
void RangeTree1D<Point>::right_rota(NodeBT<Point> *&parent) {
  NodeBT<Point> *temp = parent->left;
  parent->left = temp->right;
  temp->right = parent;
  mySwap(parent->height, temp->height);
  calcHeight(parent);
  parent = temp;
  calcHeight(parent);
}

template <typename Point>
void RangeTree1D<Point>::remove(NodeBT<Point> *&node, Point value) {
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
      NodeBT<Point> *temp = node;
      node = node->right;
      delete temp;
    } else if (node->right == nullptr) {
      NodeBT<Point> *temp = node;
      node = node->left;
      delete temp;
    } else {
      Point temp = maxValue(node->left);
      node->data = temp;
      remove(node->left, temp);
    }
  }
  // if the node doesn't add a level the height should remain the same v:
  // calcHeight(node);
  balance(node);
}

}  // namespace spatial
}  // namespace utec
