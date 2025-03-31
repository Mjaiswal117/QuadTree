#include "GeometryBoundary.hpp"
#include <vector>

GeometryBoundary::GeometryBoundary(
    const std::vector<std::array<double, 2>> &vertices,
    const std::vector<std::array<int, 2>> &connectivity) {
  for (const auto &conn : connectivity) {
    mLineSegments.push_back({{vertices[conn[0]][0], vertices[conn[0]][1]},
                             {vertices[conn[1]][0], vertices[conn[1]][1]}});
  }
}

bool GeometryBoundary::intersect(const LineSegment &line) const {
  double x1 = line.start.x;
  double y1 = line.start.y;
  double x2 = line.end.x;
  double y2 = line.end.y;

  for (const auto &boundary_line : mLineSegments) {
    double x3 = boundary_line.start.x;
    double y3 = boundary_line.start.y;
    double x4 = boundary_line.end.x;
    double y4 = boundary_line.end.y;

    double denominator = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
    if (abs(denominator) <= 1e-9)
      continue;

    double t = ((x1 - x3) * (y3 - y4) - (y1 - y3) * (x3 - x4)) / denominator;
    double u = -((x1 - x2) * (y1 - y3) - (y1 - y2) * (x1 - x3)) / denominator;

    double tol = 1e-9;
    if (t >= (0 - tol) && t <= (1 + tol) && u >= (0 - tol) && u <= (1 + tol))
      return true;
  }
  return false;
}

const BoundingBox GeometryBoundary::getBoundingBox() const {
  double xmin = std::numeric_limits<double>::max();
  double xmax = -std::numeric_limits<double>::max();
  double ymin = std::numeric_limits<double>::max();
  double ymax = -std::numeric_limits<double>::max();

  for (const auto &line : mLineSegments) {
    xmin = std::min(xmin, std::min(line.start.x, line.end.x));
    xmax = std::max(xmax, std::max(line.start.x, line.end.x));
    ymin = std::min(ymin, std::min(line.start.y, line.end.y));
    ymax = std::max(ymax, std::max(line.start.y, line.end.y));
  }
  return {xmin, xmax, ymin, ymax};
}
