#pragma once

#include "SpatialBase.h"
#include "BST.hpp"

namespace utec {
namespace spatial {

/**
 * RangeBST implementation
 */
template <typename Point>
class RangeBST : public SpatialBase<Point>, public BSTree<Point>{
 private:
  static bool comparePoint( Point& a,  Point& b){
    return a.distance()<b.distance();
  }
 public:
  RangeBST(){};
  void insert( Point& new_point) override { this->insertBST(new_point, comparePoint);}

  // El punto de referencia no necesariamente es parte del dataset
  Point nearest_neighbor( Point& reference) override { return Point({0}); }

  std::vector<Point> range( Point& min,  Point& max) override {
    return this->rangeSearch(min, max, comparePoint);
  };
};

}  // namespace spatial
}  // namespace utec
