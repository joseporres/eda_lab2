#pragma once

#include "BST.hpp"
#include "SpatialBase.h"

namespace utec {
namespace spatial {

/**
 * RangeBST implementation
 */
template <typename Point>
class RangeBST : public BSTree<Point> {
 private:
  static bool comparePoint(Point a, Point b) {
    return a.distance(Point()) < b.distance(Point());
  }

 public:
  RangeBST(){};
  void insert(Point& new_point) { this->insertBST(new_point, comparePoint); }

  // El punto de referencia no necesariamente es parte del dataset
  Point nearest_neighbor(Point& reference) { return Point({0}); }

  std::vector<Point> range(Point& min, Point& max) {
    return this->rangeSearch(min, max, comparePoint);
  };
};

}  // namespace spatial
}  // namespace utec
