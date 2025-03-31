#include "QuadTree.hpp"
#include <string>


namespace utils
{
    // Read a 1D msh file and store the vertices and connectivity of the line segments
    void readMshFile2d(const std::string& filename, std::vector<std::array<double, 2>>& vertices, std::vector<std::array<int, 2>>& connectivity );

    // Dump the quadtree to a vtk file
    void dumpQuadTreeToVtk(const QuadTree& qt, const std::string& filename);

    std::function<bool(Cell)> refineGeometryBoundary(const GeometryBoundary& boundary);

    // Function to create a boundary refiner for a circle
    std::function<bool(Cell)> refineCircle(const std::vector<double> &center, double radius);

    // Function to create a boundary refiner for a square
    std::function<bool(Cell)> refineSquare(const std::vector<double>& center, double side_length, double angle);
}