#pragma once

#include <armadillo>

class PolygonSpecklePattern
{

public:
	PolygonSpecklePattern(const double& diameter, const double& density, const double& variation, const int& edge, const double& ratio,
		const int& height, const int& width);
	~PolygonSpecklePattern();

	arma::mat x() const { return x_; }
	arma::mat y() const { return y_; }
	double radius() const { return r_; }
	double ratio() const { return ratio_; }
	arma::mat rotation() const { return theta_; }
	arma::imat edges() const { return edges_; }

private:
	arma::mat x_;		// 散斑 x 坐标
	arma::mat y_;		// 散斑 y 坐标
	double r_;			// 多边形外接圆半径

	double ratio_;		// 多边形的空心率
	arma::mat theta_;	// 多边形的旋转角
	arma::imat edges_;	// 多边形的边数
};
