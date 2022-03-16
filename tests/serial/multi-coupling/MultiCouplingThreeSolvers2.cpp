#ifndef PRECICE_NO_MPI

#include <fstream>
#include <string>
#include "helpers.hpp"
#include "testing/Testing.hpp"

BOOST_AUTO_TEST_SUITE(PreciceTests)
BOOST_AUTO_TEST_SUITE(Serial)
BOOST_AUTO_TEST_SUITE(MultiCoupling)
BOOST_AUTO_TEST_CASE(MultiCouplingThreeSolvers2)
{
  PRECICE_TEST("SolverA"_on(1_rank), "SolverB"_on(1_rank), "SolverC"_on(1_rank));
  const std::string configFile = context.config();
  multiCouplingThreeSolvers(configFile, context);
}

BOOST_AUTO_TEST_SUITE_END() // MultiCoupling
BOOST_AUTO_TEST_SUITE_END() // Serial
BOOST_AUTO_TEST_SUITE_END() // PreciceTests

#endif // PRECICE_NO_MPI
