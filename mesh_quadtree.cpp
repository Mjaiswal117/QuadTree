#include "Cell.hpp"
#include "GeometryBoundary.hpp"
#include "QuadTree.hpp"
#include "utils.hpp"

#include <array>
#include <cstdlib>
#include <string>
#include <vector>
#include <iostream>

int main(int argc, char *argv[]) {
  // Handle Input
  std::string msh_filename = std::string(PROJECT_DIR) + "/example/wing.msh";
  int base_subdivisions = 1;
  int refine_subdivisions = 8;
  std::string output_filename = "quadtree.vtk";

  // Check for command-line arguments
  if (argc > 1) {
    msh_filename = argv[1]; // First argument is the msh_filename
  }
  if (argc > 2) {
    base_subdivisions =
        std::atoi(argv[2]); // Second argument is base_subdivisions
  }
  if (argc > 3) {
    refine_subdivisions =
        std::atoi(argv[3]); // Third argument is refine_subdivisions
  }
  if (argc > 4) {
    output_filename = argv[4]; // Fourth argument is output_filename
  }

  // Output the values to confirm
  std::cout << "Filename: " << msh_filename << std::endl;
  std::cout << "Base subdivisions: " << base_subdivisions << std::endl;
  std::cout << "Refine subdivisions: " << refine_subdivisions << std::endl;

  // Read the mesh file
  std::vector<std::array<double, 2>> vertices;
  std::vector<std::array<int, 2>> connectivity;
  utils::readMshFile2d(msh_filename, vertices, connectivity);
  const GeometryBoundary boundary(vertices, connectivity);

  // Create a lambda function to identify if the cell needs to refined
  auto refine_boundary = utils::refineGeometryBoundary(boundary);

  // Create the root/intial domain cell
  const BoundingBox bbox = boundary.getBoundingBox();
  double length_x = bbox.xmax - bbox.xmin;
  double length_y = bbox.ymax - bbox.ymin;
  double length = 1.2 * std::max(length_x, length_y);
  Point2 center = {(bbox.xmin + bbox.xmax) / 2, (bbox.ymin + bbox.ymax) / 2};
  Cell cell(center, length, length);

  // Create the quadtree
  QuadTree qt(cell, base_subdivisions, refine_subdivisions, refine_boundary);

  // Dump the quadtree to a vtk file
  utils::dumpQuadTreeToVtk(qt, output_filename);

  return 0;
}