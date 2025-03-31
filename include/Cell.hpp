#ifndef CELL_H
#define CELL_H

#include <GeometryBoundary.hpp>
#include <functional>

class Cell {
public:
  // Constructor to initialize the cell
  Cell(const double xmin, const double xmax, const double ymin,
       const double ymax)
      : mXMin(xmin), mXMax(xmax), mYMin(ymin), mYMax(ymax) {}
  Cell(const Point2 &center, double length_x, double length_y)
      : mXMin(center.x - length_x / 2), mXMax(center.x + length_x / 2),
        mYMin(center.y - length_y / 2), mYMax(center.y + length_y / 2) {}

  // Accessor methods
  double Xmin() const { return mXMin; }
  double Xmax() const { return mXMax; }
  double Ymin() const { return mYMin; }
  double Ymax() const { return mYMax; }

  // Iterator to interate over the corner points of the cell
  void
  iterateCorners(const std::function<void(double, double)> &callback) const {
    callback(mXMin, mYMin);
    callback(mXMax, mYMin);
    callback(mXMax, mYMax);
    callback(mXMin, mYMax);
  }

  void
  iterateEdges(const std::function<void(const LineSegment &)> &callback) const {
    callback({{mXMin, mYMin}, {mXMax, mYMin}});
    callback({{mXMax, mYMin}, {mXMax, mYMax}});
    callback({{mXMax, mYMax}, {mXMin, mYMax}});
    callback({{mXMin, mYMax}, {mXMin, mYMin}});
  }

private:
  double mXMin, mXMax, mYMin, mYMax;
};

#endif // CELL_H