#ifndef QUADTREE_H
#define QUADTREE_H

#include "Cell.hpp"
#include <functional>

class QuadTree {
public:
  // Initialize the QuadTree with a given depth
  QuadTree(const Cell cell, const int base_subdivisions,
           const int refine_subdivisions,
           std::function<bool(Cell)> &boundary_identifier)
      : mCell(cell), mBaseSubdivisions(base_subdivisions),
        mRefineSubdivisions(refine_subdivisions),
        mBoundaryIdentifier(boundary_identifier) {
    if (mBaseSubdivisions > 0 or
        (mRefineSubdivisions > 0 and mBoundaryIdentifier(mCell)))
      subdivide();
  }

  void iterateCells(const std::function<void(Cell)> &callback) const;

private:
  const Cell mCell;
  const int mBaseSubdivisions;
  const int mRefineSubdivisions;
  std::function<bool(Cell)> &mBoundaryIdentifier;

  QuadTree *tl = nullptr;
  QuadTree *tr = nullptr;
  QuadTree *bl = nullptr;
  QuadTree *br = nullptr;

  void subdivide();
  bool isLeafNode() const;
};

#endif // QUADTREE_H