#pragma once

#include <algorithm>
#include <vector>

//#include "SpatialBase.h"

template <typename T>
struct BSNode {
  T data;
  BSNode<T>* left;
  BSNode<T>* right;
  BSNode() : left(nullptr), right(nullptr) {}
  BSNode(T d) : data(d), left(nullptr), right(nullptr) {}

  ~BSNode() {
    delete left;
    left = nullptr;
    delete right;
    right = nullptr;
  }
};

template <typename T>
class BSTree {
  BSNode<T>* root;

  void insertpriv(BSNode<T>*& node, T data, bool (*lessThan)(T, T)) {
    if (node) {
      if (lessThan(node->data, data))
        return insertpriv(node->right, data, lessThan);
      else
        return insertpriv(node->left, data, lessThan);
    } else {
      auto newnode = new BSNode<T>(data);
      node = newnode;
    }
  }

  void getInOrder(BSNode<T>* node, std::vector<T>& points) {
    if (node) {
      getInOrder(node->left, points);

      points.push_back(node->data);
      getInOrder(node->right, points);
    }
  }

 public:
  BSTree() : root(nullptr) {}
  ~BSTree() {
    delete root;
    root = nullptr;
  }

  void insertBST(T data, bool (*lessThan)(T, T)) {
    return insertpriv(root, data, lessThan);
  }
  std::vector<T> rangeSearch(T lb, T ub, bool (*lessThan)(T, T)) {
    std::vector<T> points;
    getInOrder(root, points);
    auto itlb = std::lower_bound(points.begin(), points.end(), lb, lessThan);
    auto itub = std::upper_bound(points.begin(), points.end(), ub, lessThan);
    std::vector<T> result(itlb, itub);
    return result;
  }
};