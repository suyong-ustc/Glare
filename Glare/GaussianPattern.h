#pragma once

#include <armadillo>

class GaussianPattern
{

public:
	GaussianPattern(const double& diameter, const double& coverage, const double& variation, const int& height, const int& width, const double& err = 1e-5);
	~GaussianPattern();

	void buildCacheImage();

	arma::mat Values(const int& width, const int& height) const;
	arma::mat Values(const arma::mat& img_x, const arma::mat& img_y) const;

	arma::mat EstimateValues(const int& width, const int& height) const;
	arma::mat EstimateValues(const arma::mat& img_x, const arma::mat& img_y) const;

	arma::mat x() const { return x_; }
	arma::mat y() const { return y_; }
	double radius() const { return radius_; }

private:

	double radius_;		// É¢°ß°ë¾¶
	arma::mat x_;		// É¢°ß x ×ø±ê
	arma::mat y_;		// É¢°ß y ×ø±ê

	double r2_;			// 1 / r^2
	double trunction_;	// ½Ø¶Ï°ë¾¶

	int cache_xmin_;
	int cache_xmax_;
	int cache_ymin_;
	int cache_ymax_;
	arma::mat cache_;

};
