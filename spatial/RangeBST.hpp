#pragma once

#include "SpatialBase.h"
#include "node.h"

namespace utec {
namespace spatial {

/**
 * RangeBST implementation
 */
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

template <typename Point>
class RangeBST : public SpatialBase<Point> {
 private:
  BSNode<Point>* root;

  void rangeSearch(const Point& min, const Point& max, BSNode<Point>* n,
                   std::vector<Point>& rpta) {
    if (n == nullptr) return;
    if (min <= n->data && n->data <= max) {
      rpta.push_back(n->data);
      rangeSearch(min, max, n->left, rpta);
      rangeSearch(min, max, n->right, rpta);
    } else if (n->data < min)
      rangeSearch(min, max, n->right, rpta);
    else if (max < n->data)
      rangeSearch(min, max, n->left, rpta);
  }

  void insertpriv(BSNode<Point>*& node, Point data) {
    if (node) {
      if (node->data < data)
        return insertpriv(node->right, data);
      else
        return insertpriv(node->left, data);
    } else {
      auto newnode = new BSNode<Point>(data);
      node = newnode;
    }
  }

 public:
  RangeBST() { root = nullptr; };
  void insert(const Point& new_point) override { insertpriv(root, new_point); }

  // El punto de referencia no necesariamente es parte del dataset
  Point nearest_neighbor(const Point& reference) override { return Point({0}); }
  std::vector<Point> range(const Point& min, const Point& max) override {
    std::vector<Point> rpta;
    rangeSearch(min, max, root, rpta);
    return rpta;
  };
};

}  // namespace spatial
}  // namespace utec
