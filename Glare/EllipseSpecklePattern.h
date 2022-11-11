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
	arma::mat x_;		// ɢ�� x ����
	arma::mat y_;		// ɢ�� y ����
	double r_;			// ɢ�߰뾶

	double ecc_;		// ��Բ��ƫ����
	double ratio_;		// ��Բ�Ŀ�����
	arma::mat theta_;	// ��Բ����ת��
};
