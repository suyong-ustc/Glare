#pragma once

#include <armadillo>


class EllipseSpecklePattern
{

public:
	EllipseSpecklePattern(const double& diameter, const double& density, const double& variation, const double& eccentricity, const double& ratio,
		const int& height, const int& width);
	~EllipseSpecklePattern();

	arma::mat x() const { return x_; }
	arma::mat y() const { return y_; }
	double r() const { return r_; }
	double eccentricity() const { return ecc_; }
	double ratio() const { return ratio_; }
	arma::mat rotation() const { return theta_; }

private:
	arma::mat x_;		// É¢°ß x ×ø±ê
	arma::mat y_;		// É¢°ß y ×ø±ê
	double r_;			// É¢°ß°ë¾¶

	double ecc_;		// ÍÖÔ²µÄÆ«ĞÄÂÊ
	double ratio_;		// ÍÖÔ²µÄ¿ÕĞÄÂÊ
	arma::mat theta_;	// ÍÖÔ²µÄĞı×ª½Ç
};
