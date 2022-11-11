#include "InverseMap.h"
#define LARGE_NEGTIVE_NUMBER -100000
using namespace arma;


InverseMap::InverseMap(QObject *parent)
	: QObject(parent)
{

}


InverseMap::~InverseMap()
{

}


/***********************************************************************************************************************/
/***********************************************    平移变形    *********************************************************/
/***********************************************************************************************************************/


void InverseMap::InverseMap_TranslationDeformation(const double& u0, const mat& deform_x, const mat& deform_y, 
	mat& refer_x, mat& refer_y)
{
	refer_x = deform_x - u0;
	refer_y = deform_y;
}



/***********************************************************************************************************************/
/***********************************************    拉伸变形    *********************************************************/
/***********************************************************************************************************************/

void InverseMap::InverseMap_ElongtationDeformation(const double& a, const double& x0, const mat& deform_x, const mat& deform_y,
	mat& refer_x, mat& refer_y)
{
	refer_x = (deform_x + a * x0) / (1 + a);
	refer_y = deform_y;
}



/***********************************************************************************************************************/
/***********************************************    正弦变形    *********************************************************/
/***********************************************************************************************************************/

rowvec InverseMap::EstimateInitialPosition_SinusoidalDeformation(const double& a, const double T, const double& b, const int width)
{
	// 初始化
	rowvec deform_x_at_refer = LARGE_NEGTIVE_NUMBER * ones<rowvec>(width);


	// 寻找变形图点在参考图位置的大概范围
	const double xp_min = 0;
	const double xp_max = width - 1;

	const int delta_pad = 10;

	int pad = 0;
	int x_min = -pad;
	int x_max = pad + width - 1;

	while (x_min + SinusoidalDisplacement(a, T, b, x_min) > xp_min ||
		   x_max + SinusoidalDisplacement(a, T, b, x_max) < xp_max)
	{
		pad += delta_pad;
		x_min = -pad;
		x_max = pad + width - 1;
	}


	// 参考图网格点在变形图位置
	const rowvec refer_x = regspace<rowvec>(x_min, x_max);
	const rowvec refer_x_at_deform = refer_x + SinusoidalDisplacement(a, T, b, refer_x);


	// 估计参考图每一个像素点在参考图中的位置
	double xl, xr;
	for (int c = 0; c < width; ++c)
	{
		for (int i = 0; i < refer_x.n_elem - 1; ++i)
		{
			xl = refer_x_at_deform(i);
			xr = refer_x_at_deform(i + 1);
			if (c >= xl && c < xr)
			{
				deform_x_at_refer(c) = refer_x(i) + (c - xl) / (xr - xl);
				const double a = deform_x_at_refer(c);
				break;
			}
		}
	}

	return deform_x_at_refer;

}



rowvec InverseMap::InverseMap_SinusoidalDeformation(const double& a, const double T, const double& b, const int width)
{
	// 估计初值
	rowvec deform_x_at_refer_estimate = EstimateInitialPosition_SinusoidalDeformation(a, T, b, width);

	// 使用牛顿迭代法
	rowvec deform_x_at_refer = zeros<rowvec>(width);

	for (int i = 0; i < width; ++i)
	{
		double x = deform_x_at_refer_estimate(i);	// 初值

		// 迭代
		double dx = 1;
		double f, fp;
		while (abs(dx) > 1e-4)
		{
			f = x + a * sin(2 * datum::pi * x / T + b) - i;
			fp = 1 + 2 * datum::pi * a / T * cos(2 * datum::pi * x / T + b);
			dx = -f / fp;
			x = x + dx;
		}

		deform_x_at_refer(i) = x;
	}

	return deform_x_at_refer;
}



/***********************************************************************************************************************/
/***********************************************    高斯变形    *********************************************************/
/***********************************************************************************************************************/

rowvec InverseMap::EstimateInitialPosition_GaussianDeformation(const double& a, const double x0, const double& c, const int width)
{
	// 初始化
	rowvec deform_x_at_refer = LARGE_NEGTIVE_NUMBER * ones<rowvec>(width);


	// 寻找变形图点在参考图位置的大概范围
	const double xp_min = 0;
	const double xp_max = width - 1;

	const int delta_pad = 10;

	int pad = 0;
	int x_min = -pad;
	int x_max = pad + width - 1;

	while (x_min + GaussianDisplacement(a, x0, c, x_min) > xp_min ||
		   x_max + GaussianDisplacement(a, x0, c, x_max) < xp_max)
	{
		pad += delta_pad;
		x_min = -pad;
		x_max = pad + width - 1;
	}


	// 参考图网格点在变形图位置
	const rowvec refer_x = regspace<rowvec>(x_min, x_max);
	const rowvec refer_x_at_deform = refer_x + GaussianDisplacement(a, x0, c, refer_x);


	// 估计参考图每一个像素点在参考图中的位置
	double xl, xr;
	for (int c = 0; c < width; ++c)
	{
		for (int i = 0; i < refer_x.n_elem - 1; ++i)
		{
			xl = refer_x_at_deform(i);
			xr = refer_x_at_deform(i + 1);
			if (c >= xl && c < xr)
			{
				deform_x_at_refer(c) = refer_x(i) + (c - xl) / (xr - xl);
				break;
			}
		}
	}

	return deform_x_at_refer;
}



rowvec InverseMap::InverseMap_GaussianDeformation(const double& a, const double x0, const double& c, const int width)
{
	// 估计初值
	rowvec deform_x_at_refer_estimate = EstimateInitialPosition_GaussianDeformation(a, x0, c, width);

	// 使用牛顿迭代法
	rowvec deform_x_at_refer = zeros<rowvec>(width);

	for (int i = 0; i < width; ++i)
	{
		double x = deform_x_at_refer_estimate(i);	// 初值

		// 迭代
		double dx = 1;
		double f, fp, t;
		while (abs(dx) > 1e-4)
		{
			t = (x - x0) / c;
			f = x + a * exp(-t * t) - i;
			fp = 1 - 2 * a / c * t * exp(-t * t);
			dx = -f / fp;
			x = x + dx;
		}

		deform_x_at_refer(i) = x;
	}

	return deform_x_at_refer;


}



/***********************************************************************************************************************/
/***********************************************    PLC带变形    ********************************************************/
/***********************************************************************************************************************/

rowvec InverseMap::EstimateInitialPosition_PLCDeformation(const double& a, const double x0, const double& c, const int width)
{
	// 初始化
	rowvec deform_x_at_refer = LARGE_NEGTIVE_NUMBER * ones<rowvec>(width);


	// 寻找变形图点在参考图位置的大概范围
	const double xp_min = 0;
	const double xp_max = width - 1;

	const int delta_pad = 10;

	int pad = 0;
	int x_min = -pad;
	int x_max = pad + width - 1;

	while (x_min + PLCDisplacement(a, x0, c, x_min) > xp_min ||
		x_max + PLCDisplacement(a, x0, c, x_max) < xp_max)
	{
		pad += delta_pad;
		x_min = -pad;
		x_max = pad + width - 1;
	}


	// 参考图网格点在变形图位置
	const rowvec refer_x = regspace<rowvec>(x_min, x_max);
	const rowvec refer_x_at_deform = refer_x + PLCDisplacement(a, x0, c, refer_x);


	// 估计参考图每一个像素点在参考图中的位置
	double xl, xr;
	for (int c = 0; c < width; ++c)
	{
		for (int i = 0; i < refer_x.n_elem - 1; ++i)
		{
			xl = refer_x_at_deform(i);
			xr = refer_x_at_deform(i + 1);
			if (c >= xl && c < xr)
			{
				deform_x_at_refer(c) = refer_x(i) + (c - xl) / (xr - xl);
				break;
			}
		}
	}

	return deform_x_at_refer;

}




rowvec InverseMap::InverseMap_PLCDeformation(const double& a, const double x0, const double& c, const int width)
{
	// 估计初值
	rowvec deform_x_at_refer_estimate = InverseMap::EstimateInitialPosition_PLCDeformation(a, x0, c, width);

	// 使用牛顿迭代法
	rowvec deform_x_at_refer = zeros<rowvec>(width);

	for (int i = 0; i < width; ++i)
	{
		double x = deform_x_at_refer_estimate(i);	// 初值

		// 迭代
		double dx = 1;
		double f, fp, t;
		while (abs(dx) > 1e-4)
		{
			t = (x - x0) / c;
			f = x + 0.5 * a * c * sqrt(datum::pi) * (erf((x - x0) / c) + erf(x0 / c)) - i;
			fp = 1 + a * exp(-t * t);
			dx = -f / fp;
			x = x + dx;
		}

		deform_x_at_refer(i) = x;
	}

	return deform_x_at_refer;


}



/***********************************************************************************************************************/
/***********************************************     旋转变形    ********************************************************/
/***********************************************************************************************************************/


void InverseMap::InverseMap_RotationDeformation(const double& a, const double& angle, const double& x0, const double& y0, const mat& deform_x, const mat& deform_y,
	mat& refer_x, mat& refer_y)
{
	refer_x.zeros(size(deform_x));
	refer_y.zeros(size(deform_x));

	// 转化为弧度
	const double theta = angle * datum::pi / 180;

	//const double a1 = cos(theta);
	//const double b1 = -sin(theta);
	//const mat c1_mat = deform_x - x0 + x0 * cos(theta) - y0 * sin(theta);

	//const double a2 = sin(theta);
	//const double b2 = cos(theta);
	//const mat c2_mat = deform_y - y0 + x0 * sin(theta) + y0 * cos(theta);

	//for (int r = 0; r < deform_x.n_rows; ++r)
	//{
	//	for (int c = 0; c < deform_x.n_cols; ++c)
	//	{
	//		const double c1 = c1_mat(r, c);
	//		const double c2 = c2_mat(r, c);

	//		const double x = (b1 * c2 - b2 * c1) / (b1 * a2 - b2 * a1);
	//		const double y = (c1 * a2 - c2 * a1) / (b1 * a2 - b2 * a1);

	//		refer_x(r, c) = x;
	//		refer_y(r, c) = y;
	//	}

	//}

	const mat xp = deform_x - x0;
	const mat yp = deform_y - y0;

	refer_x = (xp * cos(theta) + yp * sin(theta)) / a + x0;
	refer_y = (-sin(theta) * xp + yp * cos(theta)) / a + y0;

}

