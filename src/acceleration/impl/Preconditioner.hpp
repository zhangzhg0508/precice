#pragma once

#include <Eigen/Core>
#include <vector>

#include "cplscheme/SharedPointer.hpp"
#include "logging/LogMacros.hpp"
#include "logging/Logger.hpp"
#include "utils/assertion.hpp"

namespace precice {
namespace acceleration {
namespace impl {

/**
 * @brief Interface for preconditioner variants that can be applied to quasi-Newton acceleration schemes.
 *
 * Preconditioning (formerly also known as scaling) improves the balance between several sub-vectors for parallel or multi coupling.
 *
 * apply() applies the weighting, i.e. transforms from physical values to balanced values.
 * revert() reverts the weighting, i.e. transforms from balanced values back to physical values.
 * update() updates the preconditioner, after every FSI iteration (though some variants might only be updated after a complete time window)
 */
class Preconditioner {
public:
  Preconditioner(int maxNonConstTimeWindows)
      : _maxNonConstTimeWindows(maxNonConstTimeWindows)
  {
  }

  /// Destructor, empty.
  virtual ~Preconditioner() {}

  /**
   * @brief initialize the preconditioner
   * @param size of the pp system (e.g. rows of V)
   */
  virtual void initialize(std::vector<size_t> &svs)
  {
    PRECICE_TRACE();

    PRECICE_ASSERT(_weights.empty());
    _subVectorSizes = svs;

    size_t N = 0;
    for (auto elem : _subVectorSizes) {
      N += elem;
    }
    // cannot do this already in the constructor as the size is unknown at that point
    _weights.resize(N, 1.0);
    _invWeights.resize(N, 1.0);
  }

  /**
   * @brief Apply preconditioner to matrix
   * @param transpose: false = from left, true = from right
   */
  void apply(Eigen::MatrixXd &M, bool transpose)
  {
    PRECICE_TRACE();
    if (transpose) {
      PRECICE_ASSERT(M.cols() == (int) _weights.size(), M.cols(), _weights.size());
      for (int i = 0; i < M.cols(); i++) {
        for (int j = 0; j < M.rows(); j++) {
          M(j, i) *= _weights[i];
        }
      }
    } else {
      PRECICE_ASSERT(M.rows() == (int) _weights.size(), M.rows(), (int) _weights.size());
      for (int i = 0; i < M.cols(); i++) {
        for (int j = 0; j < M.rows(); j++) {
          M(j, i) *= _weights[j];
        }
      }
    }
  }

  /**
   * @brief Apply inverse preconditioner to matrix
   * @param transpose: false = from left, true = from right
   */
  void revert(Eigen::MatrixXd &M, bool transpose)
  {
    PRECICE_TRACE();
    //PRECICE_ASSERT(_needsGlobalWeights);
    if (transpose) {
      PRECICE_ASSERT(M.cols() == (int) _invWeights.size());
      for (int i = 0; i < M.cols(); i++) {
        for (int j = 0; j < M.rows(); j++) {
          M(j, i) *= _invWeights[i];
        }
      }
    } else {
      PRECICE_ASSERT(M.rows() == (int) _invWeights.size(), M.rows(), (int) _invWeights.size());
      for (int i = 0; i < M.cols(); i++) {
        for (int j = 0; j < M.rows(); j++) {
          M(j, i) *= _invWeights[j];
        }
      }
    }
  }

  /// To transform physical values to balanced values. Matrix version
  void apply(Eigen::MatrixXd &M)
  {
    PRECICE_TRACE();
    PRECICE_ASSERT(M.rows() == (int) _weights.size(), M.rows(), (int) _weights.size());

    // scale matrix M
    for (int i = 0; i < M.cols(); i++) {
      for (int j = 0; j < M.rows(); j++) {
        M(j, i) *= _weights[j];
      }
    }
  }

    /// To transform physical values to balanced values. Matrix version
  void applySVD(Eigen::MatrixXd &M)
  {
    PRECICE_TRACE();
    //PRECICE_ASSERT(M.rows() == (int) _weights.size(), M.rows(), (int) _weights.size());

    // scale matrix M
    for (int i = 0; i < M.cols(); i++) {
      for (int j = 0; j < M.rows(); j++) {
        M(j, i) *= _weights_SVD[j];
      }
    }
  }

  /// To transform physical values to balanced values. Vector version
  void apply(Eigen::VectorXd &v)
  {
    PRECICE_TRACE();

    PRECICE_ASSERT(v.size() == (int) _weights.size());

    // scale residual
    for (int j = 0; j < v.size(); j++) {
      v[j] *= _weights[j];
    }
  }

  /// To transform balanced values back to physical values. Matrix version
  void revert(Eigen::MatrixXd &M)
  {
    PRECICE_TRACE();

    PRECICE_ASSERT(M.rows() == (int) _weights.size());

    // scale matrix M
    for (int i = 0; i < M.cols(); i++) {
      for (int j = 0; j < M.rows(); j++) {
        M(j, i) *= _invWeights[j];
      }
    }
  }

    /// To transform balanced values back to physical values. Matrix version
  void revertSVD(Eigen::MatrixXd &M)
  {
    PRECICE_TRACE();

    //PRECICE_ASSERT(M.rows() == (int) _weights.size());

    // scale matrix M
    for (int i = 0; i < M.cols(); i++) {
      for (int j = 0; j < M.rows(); j++) {
        M(j, i) *= _invWeights_SVD[j];
      }
    }
  }

  /// To transform balanced values back to physical values. Vector version
  void revert(Eigen::VectorXd &v)
  {
    PRECICE_TRACE();

    PRECICE_ASSERT(v.size() == (int) _weights.size());

    // scale residual
    for (int j = 0; j < v.size(); j++) {
      v[j] *= _invWeights[j];
    }
  }

  /**
   * @brief Update the scaling after every FSI iteration and require a new QR decomposition (if necessary)
   *
   * @param[in] timeWindowComplete True if this FSI iteration also completed a time window
   */
  void update(bool timeWindowComplete, const Eigen::VectorXd &oldValues, const Eigen::VectorXd &res)
  {
    PRECICE_TRACE(_nbNonConstTimeWindows, _frozen);

    // if number of allowed non-const time windows is exceeded, do not update weights
    if (_frozen)
      return;

    // increment number of time windows that has been scaled with changing preconditioning weights
    if (timeWindowComplete) {
      _nbNonConstTimeWindows++;
      if (_nbNonConstTimeWindows >= _maxNonConstTimeWindows && _maxNonConstTimeWindows > 0)
        _frozen = true;
    }

    // type specific update functionality
    _update_(timeWindowComplete, oldValues, res);
  }

  /// returns true if a QR decomposition from scratch is necessary
  bool requireNewQR()
  {
    PRECICE_TRACE(_requireNewQR);
    return _requireNewQR;
  }

  /// to tell the preconditioner that QR-decomposition has been recomputed
  void newQRfulfilled()
  {
    _requireNewQR = false;
  }

  /// to tell the filtering that the pre-scaling weights were updated
  bool areWeightsUpdated()
  {
    return _areWeightsUpdated;
  }

  /// to tell that the QR decomposition has been reset due to updated pre-scaling weights
  void updatedWeightsReset()
  {
    _areWeightsUpdated = false;
  }

  bool svdWeightsSet()
  {
    return _resetSVD;
  }

  void svdReset()
  {
    _resetSVD = false;
  }

  std::vector<double> &getWeights()
  {
    return _weights;
  }

  bool isConst()
  {
    return _frozen;
  }

  void setSVDWeights()
  {
    //PRECICE_TRACE();
    //PRECICE_INFO(" SVD weights[0] = {}", _weights_SVD[0]);
    //PRECICE_INFO(" SVD inv_weights[0] = {}", _invWeights_SVD[0]);
    _weights_SVD = _weights;
    _invWeights_SVD = _invWeights;

  }

protected:
  /// Weights used to scale the matrix V and the residual
  std::vector<double> _weights;

  std::vector<double> _weights_SVD;

  /// Inverse weights (for efficiency reasons)
  std::vector<double> _invWeights;

  std::vector<double> _invWeights_SVD;

  /// Sizes of each sub-vector, i.e. each coupling data
  std::vector<size_t> _subVectorSizes;

  /** @brief maximum number of non-const time windows, i.e., after this number of time windows,
   *  the preconditioner is frozen with the current weights and becomes a constant preconditioner
   */
  int _maxNonConstTimeWindows;

  /// Counts the number of completed time windows with a non-const weighting
  int _nbNonConstTimeWindows = 0;

  /// True if a QR decomposition from scratch is necessary
  bool _requireNewQR = false;

  /// @brief True if pre-scaling weights were updated in the iteration, and a QR decomposition is required
  bool _areWeightsUpdated = false;

  /// @brief True if pre-scaling weights were updated in the iteration, and an SVD reset is required
  bool _resetSVD = false;

  /// True if _nbNonConstTimeWindows >= _maxNonConstTimeWindows, i.e., preconditioner is not updated any more.
  bool _frozen = false;

  /**
   * @brief Update the scaling after every FSI iteration and require a new QR decomposition (if necessary)
   *
   * @param[in] timeWindowComplete True if this FSI iteration also completed a time windows
   */
  virtual void _update_(bool timeWindowComplete, const Eigen::VectorXd &oldValues, const Eigen::VectorXd &res) = 0;

private:
  logging::Logger _log{"acceleration::Preconditioner"};
};

} // namespace impl
} // namespace acceleration
} // namespace precice
