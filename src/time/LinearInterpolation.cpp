#include "LinearInterpolation.hpp"
#include "utils/assertion.hpp"

namespace precice
{
namespace time
{

void LinearInterpolation::apply(
    Eigen::VectorXd &values,
    const Eigen::VectorXd &newValues,
    const Eigen::VectorXd &oldValues,
    double computedTimestepPart,
    double windowSize)
{
  TRACE();
  assertion(computedTimestepPart>0.0);
  assertion(computedTimestepPart<=windowSize);
  double coeff = computedTimestepPart / windowSize;
  values = coeff * newValues + (1 - coeff) * oldValues;
}
}
} // namespace precice, time
