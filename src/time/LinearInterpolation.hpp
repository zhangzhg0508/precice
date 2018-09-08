#pragma once

#include "Interpolation.hpp"
#include <Eigen/Core>
#include "logging/Logger.hpp"

namespace precice
{
namespace time
{

/**
 * @brief
 *
 * long
 */
class LinearInterpolation : public Interpolation
{
public:

  LinearInterpolation(){};

  virtual ~LinearInterpolation(){};


  virtual void apply(
    Eigen::VectorXd &values,
    const Eigen::VectorXd &newValues,
    const Eigen::VectorXd &oldValues,
    double computedTimestepPart,
    double windowSize);

private:

  logging::Logger _log{"time::LinearInterpolation"};

};
}
} // namespace precice, time
