#pragma once

#include <QObject>
#include <QString>
#include <opencv2/core.hpp>
#include <armadillo>
#include "XianType.h"


class AssessSpecklePattern : public QObject
{
	Q_OBJECT

public:
	AssessSpecklePattern(QObject *parent);
	~AssessSpecklePattern();

	// 图像格式转换
	static arma::mat Uchar2ArmaMat(uchar* data, int h, int w);

	// 插值传递函数
	static double Sinc(const double& f);
	static double TransferFunction_Keys(const double& f);
	static double TransferFunction_BSpline3(const double& f);
	static double TransferFunction_BSpline5(const double& f);
	static double TransferFunction_BSpline7(const double& f);

	// 梯形法求近似积分
	static double TrapezoidIntegral(const cv::Mat& a);

	// 图像功率谱
	static cv::Mat PowerSpectrum(const cv::Mat& I);

	// 计算插值偏差核与灰度梯度平方和核
	static void Kernel_Keys(const cv::Size& sz, cv::Mat& bias_kernel, cv::Mat& sssig_kernel);
	static void Kernel_BSpline3(const cv::Size& sz, cv::Mat& bias_kernel, cv::Mat& sssig_kernel);
	static void Kernel_BSpline5(const cv::Size& sz, cv::Mat& bias_kernel, cv::Mat& sssig_kernel);
	static void Kernel_BSpline7(const cv::Size& sz, cv::Mat& bias_kernel, cv::Mat& sssig_kernel);

	// 估计插值偏差
	static double InterpolationBias(const cv::Mat& I, const InterpolationAlgorithm& interp_type);

	// Otsu法估计二值化阈值
	static double OtsuThreshold(cv::Mat ori_image, int total_level = 256);

	// 计算自相关曲线
	static arma::vec Normalize(const arma::mat& data);

	static void CalculateAutoCorrelation(const arma::mat& image, const int& subset_size, const int& maximal_translation,
		arma::vec& zncc_x, arma::vec& zncc_y);

	// 计算散斑尺寸
	static double CalculateSpeckleSize(const arma::vec& zncc_x, const arma::vec& zncc_y);

	// 计算图像灰度梯度
	static void CalculateImageGradients(const arma::mat& image, arma::mat& gradient_x, arma::mat& gradient_y);

	// 计算积分图像
	static arma::mat CalculateIntegralImage(const arma::mat& image);

};
