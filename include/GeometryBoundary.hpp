#ifndef GEOMETRY_BOUNDARY_H
#define GEOMETRY_BOUNDARY_H
#include <array>
#include <vector>

struct Point2 {
  double x, y;
};

struct LineSegment {
  Point2 start, end;
};

struct BoundingBox {
  double xmin, xmax, ymin, ymax;
};

class GeometryBoundary {
public:
  GeometryBoundary(const std::vector<LineSegment> &lines)
      : mLineSegments(lines) {}
  GeometryBoundary(const std::vector<std::array<double, 2>> &vertices,
                   const std::vector<std::array<int, 2>> &connectivity);

  bool intersect(const LineSegment &line) const;

  const BoundingBox getBoundingBox() const;

private:
  std::vector<LineSegment> mLineSegments;
};

#endif // GEOMETRY_BOUNDARY_H