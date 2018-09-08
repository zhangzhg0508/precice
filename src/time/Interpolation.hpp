#pragma once

#include <Eigen/Core>
#include <map>

namespace precice
{
namespace time
{

/**
 * @brief
 *
 * long
 */
class Interpolation
{
public:
  /// Destructor, empty.
  virtual ~Interpolation() {}

  /**
   * @brief Performs convergence measurement.
   *
   * @param[inout] values Old iterate values.
   * @param[in] newValues New iterate values.
   */
  virtual void apply(
    Eigen::VectorXd &values,
    const Eigen::VectorXd &newValues,
    const Eigen::VectorXd &oldValues,
    double computedTimestepPart,
    double windowSize) = 0;

};

}
} // namespace precice, time
