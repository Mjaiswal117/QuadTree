#include "QuadTree.hpp"

void QuadTree::iterateCells(const std::function<void(Cell)> &callback) const {
  if (isLeafNode())
    callback(mCell);
  else {
    tl->iterateCells(callback);
    tr->iterateCells(callback);
    bl->iterateCells(callback);
    br->iterateCells(callback);
  }
}

void QuadTree::subdivide() {
  double x_mid = (mCell.Xmin() + mCell.Xmax()) / 2;
  double y_mid = (mCell.Ymin() + mCell.Ymax()) / 2;
  tl = new QuadTree(Cell(mCell.Xmin(), x_mid, y_mid, mCell.Ymax()),
                    mBaseSubdivisions - 1, mRefineSubdivisions - 1,
                    mBoundaryIdentifier);
  tr = new QuadTree(Cell(x_mid, mCell.Xmax(), y_mid, mCell.Ymax()),
                    mBaseSubdivisions - 1, mRefineSubdivisions - 1,
                    mBoundaryIdentifier);
  bl = new QuadTree(Cell(mCell.Xmin(), x_mid, mCell.Ymin(), y_mid),
                    mBaseSubdivisions - 1, mRefineSubdivisions - 1,
                    mBoundaryIdentifier);
  br = new QuadTree(Cell(x_mid, mCell.Xmax(), mCell.Ymin(), y_mid),
                    mBaseSubdivisions - 1, mRefineSubdivisions - 1,
                    mBoundaryIdentifier);
}

bool QuadTree::isLeafNode() const {
  return tl == nullptr && tr == nullptr && bl == nullptr && br == nullptr;
}
