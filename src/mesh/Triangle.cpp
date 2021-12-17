#include "Triangle.hpp"
#include <Eigen/Core>
#include <Eigen/Dense>
#include <Eigen/Geometry>
#include <algorithm>
#include <boost/concept/assert.hpp>
#include <boost/range/concepts.hpp>
#include "math/differences.hpp"
#include "math/geometry.hpp"
#include "mesh/Edge.hpp"
#include "mesh/Vertex.hpp"
#include "utils/EigenIO.hpp"

namespace precice {
namespace mesh {

BOOST_CONCEPT_ASSERT((boost::RandomAccessIteratorConcept<Triangle::iterator>) );
BOOST_CONCEPT_ASSERT((boost::RandomAccessIteratorConcept<Triangle::const_iterator>) );
BOOST_CONCEPT_ASSERT((boost::RandomAccessRangeConcept<Triangle>) );
BOOST_CONCEPT_ASSERT((boost::RandomAccessRangeConcept<const Triangle>) );

Triangle::Triangle(
    Vertex &vertexOne,
    Vertex &vertexTwo,
    Vertex &vertexThree,
    int     id)
    : _vertices({&vertexOne, &vertexTwo, &vertexThree}),
      _id(id)
{
  PRECICE_ASSERT(vertexOne.getDimensions() == vertexTwo.getDimensions());
  PRECICE_ASSERT(vertexOne.getDimensions() == vertexThree.getDimensions());
  PRECICE_ASSERT(vertexOne.getID() != vertexTwo.getID());
  PRECICE_ASSERT(vertexOne.getID() != vertexThree.getID());
}

Triangle::Triangle(
    Edge &edgeOne,
    Edge &edgeTwo,
    Edge &edgeThree,
    int   id)
    : _vertices(),
      _id(id)
{
  PRECICE_ASSERT(edgeOne.getDimensions() == edgeTwo.getDimensions(),
                 edgeOne.getDimensions(), edgeTwo.getDimensions());
  PRECICE_ASSERT(edgeTwo.getDimensions() == edgeThree.getDimensions(),
                 edgeTwo.getDimensions(), edgeThree.getDimensions());
  PRECICE_ASSERT(edgeOne.getDimensions() == 3);

  // Use the first and the second vertex from the first edge.
  Vertex &v0   = edgeOne.vertex(0);
  Vertex &v1   = edgeOne.vertex(1);
  _vertices[0] = &v0;
  _vertices[1] = &v1;

  // Find the third vertex using the second edge.
  Vertex &e2v0 = edgeTwo.vertex(0);
  Vertex &e2v1 = edgeTwo.vertex(1);
  if (e2v0 == v0) {
    _vertices[2] = &e2v1;
  } else if (e2v0 == v1) {
    _vertices[2] = &e2v1;
  } else if (e2v1 == v0) {
    _vertices[2] = &e2v0;
  } else {
    PRECICE_ASSERT(e2v1 == v1, "Edges one and two are not connected");
    _vertices[2] = &e2v0;
  }
}

double Triangle::getArea() const
{
  return math::geometry::triangleArea(vertex(0).getCoords(), vertex(1).getCoords(), vertex(2).getCoords());
}

Eigen::VectorXd Triangle::computeNormal() const
{
  PRECICE_ASSERT(getDimensions() == 3);
  Eigen::Vector3d v01{vertex(1).getCoords() - vertex(0).getCoords()};
  Eigen::Vector3d v02{vertex(2).getCoords() - vertex(0).getCoords()};
  return v01.cross(v02).normalized();
}

int Triangle::getDimensions() const
{
  return _vertices[0]->getDimensions();
}

const Eigen::VectorXd Triangle::getCenter() const
{
  return (vertex(0).getCoords() + vertex(1).getCoords() + vertex(2).getCoords()) / 3.0;
}

double Triangle::getEnclosingRadius() const
{
  auto center = getCenter();
  return std::max({(center - vertex(0).getCoords()).norm(),
                   (center - vertex(1).getCoords()).norm(),
                   (center - vertex(2).getCoords()).norm()});
}

bool Triangle::operator==(const Triangle &other) const
{
  return std::is_permutation(_vertices.begin(), _vertices.end(), other._vertices.begin(),
                             [](const Vertex *u, const Vertex *v) { return *u == *v; });
}

bool Triangle::operator!=(const Triangle &other) const
{
  return !(*this == other);
}

std::ostream &operator<<(std::ostream &os, const Triangle &t)
{
  using utils::eigenio::wkt;
  return os << "POLYGON (("
            << t.vertex(0).getCoords().transpose().format(wkt()) << ", "
            << t.vertex(1).getCoords().transpose().format(wkt()) << ", "
            << t.vertex(2).getCoords().transpose().format(wkt()) << ", "
            << t.vertex(0).getCoords().transpose().format(wkt()) << "))";
}

} // namespace mesh
} // namespace precice
