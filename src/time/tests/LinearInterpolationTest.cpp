#include "testing/Testing.hpp"

#include "time/LinearInterpolation.hpp"


BOOST_AUTO_TEST_SUITE(TimeTests)
BOOST_AUTO_TEST_SUITE(LinearInterpolation)

BOOST_AUTO_TEST_CASE(SimpleTest)
{
  precice::time::LinearInterpolation interpolation;

  double windowSize = 0.1;

  Eigen::VectorXd values(3), newValues(3), oldValues(3), expected(3);
  values << 0.0, 0.0, 0.0;
  newValues << 1.0, 2.0, 3.0;
  oldValues << 1.0, 1.0, 1.0;

  double computedTimestepPart = 0.05;
  interpolation.apply(values, newValues, oldValues, computedTimestepPart, windowSize);
  expected = 0.5 * newValues + 0.5 * oldValues;
  BOOST_TEST(values == expected);

  computedTimestepPart = 0.1;
  interpolation.apply(values, newValues, oldValues, computedTimestepPart, windowSize);
  expected = newValues;
  BOOST_TEST(values == expected);
}

BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
