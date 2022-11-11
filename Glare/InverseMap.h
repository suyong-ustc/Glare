#pragma once

#include <QObject>
#include <armadillo>


class InverseMap : public QObject
{

public:
	InverseMap(QObject *parent);
	~InverseMap();

	/***********************************************************************************************************************/
	/***********************************************    平移变形    *********************************************************/
	/***********************************************************************************************************************/

	static void InverseMap_TranslationDeformation(const double& u0, const arma::mat& deform_x, const arma::mat& deform_y,
		arma::mat& refer_x, arma::mat& refer_y);


	/***********************************************************************************************************************/
	/***********************************************    拉伸变形    *********************************************************/
	/***********************************************************************************************************************/

	static void InverseMap_ElongtationDeformation(const double& a, const double& x0, const arma::mat& deform_x, const arma::mat& deform_y,
		arma::mat& refer_x, arma::mat& refer_y);



	/***********************************************************************************************************************/
	/***********************************************    正弦变形    *********************************************************/
	/***********************************************************************************************************************/

	static double SinusoidalDisplacement(const double& a, const double T, const double& b, const double& x)
	{
		return a * sin(2 * arma::datum::pi * x / T + b);
	}


	static arma::rowvec SinusoidalDisplacement(const double& a, const double T, const double& b, const arma::rowvec& x)
	{
		return a * arma::sin(2 * arma::datum::pi * x / T + b);
	}

	// 估计正弦位移
	static arma::rowvec EstimateInitialPosition_SinusoidalDeformation(const double& a, const double T, const double& b, const int width);

	// 使用牛顿迭代法确定正弦变形的反映射
	static arma::rowvec InverseMap_SinusoidalDeformation(const double& a, const double T, const double& b, const int width);

	/***********************************************************************************************************************/
	/***********************************************    高斯变形    *********************************************************/
	/***********************************************************************************************************************/

	static double GaussianDisplacement(const double& a, const double x0, const double& c, const double& x)
	{
		const double t = (x - x0) / c;
		return a * exp(-t * t);
	}

	static arma::rowvec GaussianDisplacement(const double& a, const double x0, const double& c, const arma::rowvec& x)
	{
		const arma::rowvec t = (x - x0) / c;
		return a * exp(-t % t);
	}

	static arma::rowvec EstimateInitialPosition_GaussianDeformation(const double& a, const double x0, const double& c, const int width);

	static arma::rowvec InverseMap_GaussianDeformation(const double& a, const double x0, const double& c, const int width);

	/***********************************************************************************************************************/
	/***********************************************    PLC带变形    ********************************************************/
	/***********************************************************************************************************************/


	static double PLCDisplacement(const double& a, const double x0, const double& c, const double& x)
	{
		return 0.5 * a * c * sqrt(arma::datum::pi) * (erf((x - x0) / c) + erf(x0 / c));
	}

	static arma::rowvec PLCDisplacement(const double& a, const double x0, const double& c, const arma::rowvec& x)
	{
		return 0.5 * a * c * sqrt(arma::datum::pi) * (erf((x - x0) / c) + erf(x0 / c));
	}


	static arma::rowvec EstimateInitialPosition_PLCDeformation(const double& a, const double x0, const double& c, const int width);

	static arma::rowvec InverseMap_PLCDeformation(const double& a, const double x0, const double& c, const int width);



	/***********************************************************************************************************************/
	/***********************************************    旋转变形    *********************************************************/
	/***********************************************************************************************************************/

	static void InverseMap_RotationDeformation(const double& a, const double& angle, const double& x0, const double& y0, const arma::mat& deform_x, const arma::mat& deform_y,
		arma::mat& refer_x, arma::mat& refer_y);



};
