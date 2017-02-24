#include "proxy.h"

#include "Eigen/SVD"

namespace proxy {
	double fit_curve(const std::vector<size_t> & sizes,
		const std::vector<TimeIterationsData> & times,
		proxy_runtime_t curve)
	{
		// Eigen is the linear algebra library used here for convenience
		using namespace Eigen;

		MatrixXd matrix(sizes.size(), 2);
		VectorXd rhs(sizes.size());
		for (size_t i = 0; i < sizes.size(); ++i) {
			matrix(i, 0) = 1;
			matrix(i, 1) = runtime_function[curve](sizes[i]);
			rhs(i) = times[i].timePerCall;
		}

		VectorXd soln = matrix.jacobiSvd(ComputeThinU | ComputeThinV).solve(rhs);

		double residual = 0;
		for (size_t i = 0; i < sizes.size(); ++i) {
			double value = soln(0) + soln(1) * runtime_function[curve](sizes[i]);
			residual += (value - times[i].timePerCall)
						* (value - times[i].timePerCall);
		}
		return std::sqrt(residual / sizes.size());
	}
}
