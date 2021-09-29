#pragma once

#include <algorithm>
#include <iostream>
#include <vector>

template <typename T>
typedef bool (*lessFunc)(T&, T&);

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

  void insert(BSNode<T>*& node, T data, lessFunc lessThan) {
    if (node) {
      if (lessThan(node->data, data))
        return insert(node->right, data, lessThan);
      else
        return insert(node->left, data, lessThan);
    } else {
      auto newnode = new BSNode<T>(data);
      node = newnode;
    }
  }

  void displayInOrder(BSNode<T>* node, std::vector<T>& points) {
    if (node) {
      displayInOrder(node->left, points);

      points.push_back(node->data);
      displayInOrder(node->right, points);
    }
  }
  void displayPreOrder(BSNode<T>* node) {
    if (node) {
      std::cout << (node->data) << "\n";
      displayPreOrder(node->left);
      displayPreOrder(node->right);
    }
  }
  void displayPostOrder(BSNode<T>* node) {
    if (node) {
      displayPostOrder(node->left);
      displayPostOrder(node->right);
      std::cout << (node->data) << "\n";
    }
  }

  void displayPretty(BSNode<T>* node, int level) {
    if (node) {
      int count = level;
      while (count--) {
        if (count)
          std::cout << "│ ";
        else
          std::cout << "├>";
      }
      std::cout << "(" << node->data << ")\n";
      displayPretty(node->left, level + 1);
      displayPretty(node->right, level + 1);
    }
  }

 public:
  BSTree() : root(nullptr) {}
  ~BSTree() {
    delete root;
    root = nullptr;
  }

  void insertBST(T data, lessFunc lessThan) {
    return insert(root, data, lessThan);
  }
  std::vector<T> rangeSearch(T lb, T ub, lessFunc lessThan) {
    std::vector<T> points;
    displayInOrder(root, points);
    auto itlb = std::lower_bound(points.begin(), points.end(), lb, lessThan);
    auto itub = std::upper_bound(itlb, points.end(), ub, lessThan);
    return std::vector<T>(itlb, itub);
  }
  void displayPreOrder() { return displayPreOrder(root); }
  void displayPostOrder() { return displayPostOrder(root); }
  void displayPretty() { return displayPretty(root, 0); }
};