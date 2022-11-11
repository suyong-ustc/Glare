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

	// ͼ���ʽת��
	static arma::mat Uchar2ArmaMat(uchar* data, int h, int w);

	// ��ֵ���ݺ���
	static double Sinc(const double& f);
	static double TransferFunction_Keys(const double& f);
	static double TransferFunction_BSpline3(const double& f);
	static double TransferFunction_BSpline5(const double& f);
	static double TransferFunction_BSpline7(const double& f);

	// ���η�����ƻ���
	static double TrapezoidIntegral(const cv::Mat& a);

	// ͼ������
	static cv::Mat PowerSpectrum(const cv::Mat& I);

	// �����ֵƫ�����Ҷ��ݶ�ƽ���ͺ�
	static void Kernel_Keys(const cv::Size& sz, cv::Mat& bias_kernel, cv::Mat& sssig_kernel);
	static void Kernel_BSpline3(const cv::Size& sz, cv::Mat& bias_kernel, cv::Mat& sssig_kernel);
	static void Kernel_BSpline5(const cv::Size& sz, cv::Mat& bias_kernel, cv::Mat& sssig_kernel);
	static void Kernel_BSpline7(const cv::Size& sz, cv::Mat& bias_kernel, cv::Mat& sssig_kernel);

	// ���Ʋ�ֵƫ��
	static double InterpolationBias(const cv::Mat& I, const InterpolationAlgorithm& interp_type);

	// Otsu�����ƶ�ֵ����ֵ
	static double OtsuThreshold(cv::Mat ori_image, int total_level = 256);

	// �������������
	static arma::vec Normalize(const arma::mat& data);

	static void CalculateAutoCorrelation(const arma::mat& image, const int& subset_size, const int& maximal_translation,
		arma::vec& zncc_x, arma::vec& zncc_y);

	// ����ɢ�߳ߴ�
	static double CalculateSpeckleSize(const arma::vec& zncc_x, const arma::vec& zncc_y);

	// ����ͼ��Ҷ��ݶ�
	static void CalculateImageGradients(const arma::mat& image, arma::mat& gradient_x, arma::mat& gradient_y);

	// �������ͼ��
	static arma::mat CalculateIntegralImage(const arma::mat& image);

};
