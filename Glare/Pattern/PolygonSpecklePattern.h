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
	arma::mat x_;		// ɢ�� x ����
	arma::mat y_;		// ɢ�� y ����
	double r_;			// ��������Բ�뾶

	double ratio_;		// ����εĿ�����
	arma::mat theta_;	// ����ε���ת��
	arma::imat edges_;	// ����εı���
};
