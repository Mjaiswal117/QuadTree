#include "utils.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

void utils::readMshFile2d(const std::string &filename,
                          std::vector<std::array<double, 2>> &vertices,
                          std::vector<std::array<int, 2>> &connectivity) {
  std::ifstream file(filename);
  if (!file.is_open()) {
    throw std::runtime_error("Unable to open file: " + filename);
  }

  std::string line;
  bool readingNodes = false;
  bool readingElements = false;

  while (std::getline(file, line)) {
    if (line == "$Nodes") {
      readingNodes = true;
      continue;
    } else if (line == "$EndNodes") {
      readingNodes = false;
      continue;
    } else if (line == "$Elements") {
      readingElements = true;
      continue;
    } else if (line == "$EndElements") {
      readingElements = false;
      continue;
    }

    if (readingNodes) {
      std::istringstream iss(line);
      int id;
      double x, y, z;
      if (iss >> id >> x >> y >> z) {
        vertices.push_back({x, y});
      }
    }

    if (readingElements) {
      std::istringstream iss(line);
      int id, type, tags, tagValue, n0, n1, n2;
      if (iss >> id >> type >> tags >> tagValue >> n0 >> n1 >> n2) {
        if (type == 1) { // Type 1 corresponds to 1D elements
          connectivity.push_back(
              {n1 - 1, n2 - 1}); // Convert to 0-based indexing
        }
      }
    }
  }

  file.close();
}

void utils::dumpQuadTreeToVtk(const QuadTree &qt, const std::string &filename) {
  std::vector<Cell> cells;
  qt.iterateCells([&cells](const Cell &cell) { cells.push_back(cell); });

  // Open the output file
  std::ofstream vtkFile(filename);
  if (!vtkFile.is_open()) {
    std::cerr << "Failed to open the file: " << filename << std::endl;
    return;
  }

  // Write VTK header
  vtkFile << "# vtk DataFile Version 2.0\n";
  vtkFile << "Generated VTK from C++\n";
  vtkFile << "ASCII\n";
  vtkFile << "DATASET UNSTRUCTURED_GRID\n";

  // Write points
  vtkFile << "POINTS " << cells.size() * 4 << " float\n";
  for (const auto &cell : cells) {
    cell.iterateCorners(
        [&vtkFile](double x, double y) { vtkFile << x << " " << y << " 0\n"; });
  }

  // Write cells
  vtkFile << "CELLS " << cells.size() << " " << cells.size() * 5
          << "\n"; // 4 corners + 1 count
  int pointIndex = 0;
  for (size_t i = 0; i < cells.size(); ++i) {
    vtkFile << "4 " << pointIndex << " " << pointIndex + 1 << " "
            << pointIndex + 2 << " " << pointIndex + 3 << "\n";
    pointIndex += 4;
  }

  // Write cell types
  vtkFile << "CELL_TYPES " << cells.size() << "\n";
  for (size_t i = 0; i < cells.size(); ++i) {
    vtkFile << "9\n"; // VTK_QUAD
  }

  vtkFile.close();
  std::cout << "VTK file written to " << filename << std::endl;
}

std::function<bool(Cell)>
utils::refineGeometryBoundary(const GeometryBoundary &boundary) {
  return [&boundary](const Cell &cell) -> bool {
    bool is_cut = false;
    cell.iterateEdges([&boundary, &is_cut](const LineSegment &cell_edge) {
      if (boundary.intersect(cell_edge))
        is_cut = true;
    });
    return is_cut;
  };
}

std::function<bool(Cell)> utils::refineCircle(const std::vector<double> &center,
                                              double radius) {
  return [center, radius](const Cell &cell) -> bool {
    int num_inside_corners = 0;
    cell.iterateCorners(
        [&center, radius, &num_inside_corners](double x, double y) {
          if ((pow(x - center[0], 2) + pow(y - center[1], 2)) < pow(radius, 2))
            num_inside_corners++;
        });
    bool is_cut = num_inside_corners > 0 && num_inside_corners < 4;
    return is_cut;
  };
}

std::function<bool(Cell)> utils::refineSquare(const std::vector<double> &center,
                                              double side_length,
                                              double angle) {
  return [center, side_length, angle](const Cell &cell) -> bool {
    int num_inside_corners = 0;
    cell.iterateCorners(
        [&center, side_length, angle, &num_inside_corners](double x, double y) {
          double x_rot = (x - center[0]) * cos(angle) -
                         (y - center[1]) * sin(angle) + center[0];
          double y_rot = (x - center[0]) * sin(angle) +
                         (y - center[1]) * cos(angle) + center[1];
          if (abs(x_rot - center[0]) < side_length / 2 &&
              abs(y_rot - center[1]) < side_length / 2)
            num_inside_corners++;
        });
    bool is_cut = num_inside_corners > 0 && num_inside_corners < 4;
    return is_cut;
  };
}