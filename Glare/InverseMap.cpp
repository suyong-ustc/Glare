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
/***********************************************    ƽ�Ʊ���    *********************************************************/
/***********************************************************************************************************************/


void InverseMap::InverseMap_TranslationDeformation(const double& u0, const mat& deform_x, const mat& deform_y, 
	mat& refer_x, mat& refer_y)
{
	refer_x = deform_x - u0;
	refer_y = deform_y;
}



/***********************************************************************************************************************/
/***********************************************    �������    *********************************************************/
/***********************************************************************************************************************/

void InverseMap::InverseMap_ElongtationDeformation(const double& a, const double& x0, const mat& deform_x, const mat& deform_y,
	mat& refer_x, mat& refer_y)
{
	refer_x = (deform_x + a * x0) / (1 + a);
	refer_y = deform_y;
}



/***********************************************************************************************************************/
/***********************************************    ���ұ���    *********************************************************/
/***********************************************************************************************************************/

rowvec InverseMap::EstimateInitialPosition_SinusoidalDeformation(const double& a, const double T, const double& b, const int width)
{
	// ��ʼ��
	rowvec deform_x_at_refer = LARGE_NEGTIVE_NUMBER * ones<rowvec>(width);


	// Ѱ�ұ���ͼ���ڲο�ͼλ�õĴ�ŷ�Χ
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


	// �ο�ͼ������ڱ���ͼλ��
	const rowvec refer_x = regspace<rowvec>(x_min, x_max);
	const rowvec refer_x_at_deform = refer_x + SinusoidalDisplacement(a, T, b, refer_x);


	// ���Ʋο�ͼÿһ�����ص��ڲο�ͼ�е�λ��
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
	// ���Ƴ�ֵ
	rowvec deform_x_at_refer_estimate = EstimateInitialPosition_SinusoidalDeformation(a, T, b, width);

	// ʹ��ţ�ٵ�����
	rowvec deform_x_at_refer = zeros<rowvec>(width);

	for (int i = 0; i < width; ++i)
	{
		double x = deform_x_at_refer_estimate(i);	// ��ֵ

		// ����
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
/***********************************************    ��˹����    *********************************************************/
/***********************************************************************************************************************/

rowvec InverseMap::EstimateInitialPosition_GaussianDeformation(const double& a, const double x0, const double& c, const int width)
{
	// ��ʼ��
	rowvec deform_x_at_refer = LARGE_NEGTIVE_NUMBER * ones<rowvec>(width);


	// Ѱ�ұ���ͼ���ڲο�ͼλ�õĴ�ŷ�Χ
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


	// �ο�ͼ������ڱ���ͼλ��
	const rowvec refer_x = regspace<rowvec>(x_min, x_max);
	const rowvec refer_x_at_deform = refer_x + GaussianDisplacement(a, x0, c, refer_x);


	// ���Ʋο�ͼÿһ�����ص��ڲο�ͼ�е�λ��
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
	// ���Ƴ�ֵ
	rowvec deform_x_at_refer_estimate = EstimateInitialPosition_GaussianDeformation(a, x0, c, width);

	// ʹ��ţ�ٵ�����
	rowvec deform_x_at_refer = zeros<rowvec>(width);

	for (int i = 0; i < width; ++i)
	{
		double x = deform_x_at_refer_estimate(i);	// ��ֵ

		// ����
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
/***********************************************    PLC������    ********************************************************/
/***********************************************************************************************************************/

rowvec InverseMap::EstimateInitialPosition_PLCDeformation(const double& a, const double x0, const double& c, const int width)
{
	// ��ʼ��
	rowvec deform_x_at_refer = LARGE_NEGTIVE_NUMBER * ones<rowvec>(width);


	// Ѱ�ұ���ͼ���ڲο�ͼλ�õĴ�ŷ�Χ
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


	// �ο�ͼ������ڱ���ͼλ��
	const rowvec refer_x = regspace<rowvec>(x_min, x_max);
	const rowvec refer_x_at_deform = refer_x + PLCDisplacement(a, x0, c, refer_x);


	// ���Ʋο�ͼÿһ�����ص��ڲο�ͼ�е�λ��
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
	// ���Ƴ�ֵ
	rowvec deform_x_at_refer_estimate = InverseMap::EstimateInitialPosition_PLCDeformation(a, x0, c, width);

	// ʹ��ţ�ٵ�����
	rowvec deform_x_at_refer = zeros<rowvec>(width);

	for (int i = 0; i < width; ++i)
	{
		double x = deform_x_at_refer_estimate(i);	// ��ֵ

		// ����
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
/***********************************************     ��ת����    ********************************************************/
/***********************************************************************************************************************/


void InverseMap::InverseMap_RotationDeformation(const double& a, const double& angle, const double& x0, const double& y0, const mat& deform_x, const mat& deform_y,
	mat& refer_x, mat& refer_y)
{
	refer_x.zeros(size(deform_x));
	refer_y.zeros(size(deform_x));

	// ת��Ϊ����
	const double theta = angle * datum::pi / 180;

	const mat xp = deform_x - x0;
	const mat yp = deform_y - y0;

	refer_x = (xp * cos(theta) + yp * sin(theta)) / a + x0;
	refer_y = (-sin(theta) * xp + yp * cos(theta)) / a + y0;

}



/***********************************************************************************************************************/
/***********************************************    ���ұ���    *********************************************************/
/***********************************************************************************************************************/


void InverseMap::EstimateInitialPosition_2DSinusoidalDeformation(const double& ax, const double Tx, const double& bx, const int& width, const double& ay, const double& Ty, const double& by, const int& height, mat& x_mat, mat& y_mat)
{
	// ���Ƴ�ֵ
	rowvec x = EstimateInitialPosition_SinusoidalDeformation(ax, Tx, bx, width);
	rowvec y = EstimateInitialPosition_SinusoidalDeformation(ay, Ty, by, height);

	// �Ծ���ֵ
	x_mat.zeros(height, width);
	y_mat.zeros(height, width);

	for (int r = 0; r < height; ++r)
	{
		for (int c = 0; c < width; ++c)
		{
			x_mat(r, c) = x(c);
			y_mat(r, c) = y(r);
		}

	}
}


void InverseMap::InverseMap_2DSinusoidalDeformation(const double& ax, const double Tx, const double& bx, const int& width, const double& ay, const double& Ty, const double& by, const int& height, mat& x_mat, mat& y_mat)
{
	// ���Ƴ�ֵ
	rowvec deform_x_at_refer_estimate = EstimateInitialPosition_SinusoidalDeformation(ax, Tx, bx, width);

	// ʹ��ţ�ٵ�����
	rowvec deform_x_at_refer = zeros<rowvec>(width);

	for (int i = 0; i < width; ++i)
	{
		double x = deform_x_at_refer_estimate(i);	// ��ֵ

		// ����
		double dx = 1;
		double f, fp;
		while (abs(dx) > 1e-4)
		{
			f = x + ax * sin(2 * datum::pi * x / Tx + bx) - i;
			fp = 1 + 2 * datum::pi * ax / Tx * cos(2 * datum::pi * x / Tx + bx);
			dx = -f / fp;
			x = x + dx;
		}

		deform_x_at_refer(i) = x;
	}


	// ���Ƴ�ֵ
	rowvec deform_y_at_refer_estimate = EstimateInitialPosition_SinusoidalDeformation(ay, Ty, by, height);

	// ʹ��ţ�ٵ�����
	rowvec deform_y_at_refer = zeros<rowvec>(height);

	for (int i = 0; i < height; ++i)
	{
		double y = deform_y_at_refer_estimate(i);	// ��ֵ

		// ����
		double dy = 1;
		double f, fp;
		while (abs(dy) > 1e-4)
		{
			f = y + ay * sin(2 * datum::pi * y / Ty + by) - i;
			fp = 1 + 2 * datum::pi * ay / Ty * cos(2 * datum::pi * y / Ty + by);
			dy = -f / fp;
			y = y + dy;
		}

		deform_y_at_refer(i) = y;
	}


	// λ�þ���
	x_mat.zeros(height, width);
	y_mat.zeros(height, width);

	for (int r = 0; r < height; ++r)
	{
		for (int c = 0; c < width; ++c)
		{
			x_mat(r, c) = deform_x_at_refer(c);
			y_mat(r, c) = deform_y_at_refer(r);
		}
	}

}



/***********************************************************************************************************************/
/***********************************************  ��ά��˹����  *********************************************************/
/***********************************************************************************************************************/


void InverseMap::EstimateInitialPosition_2DGaussianDeformation(const double& ax, const double x0, const double& cx, const int& width, const double& ay, const double& y0, const double& cy, const int& height, mat& x_mat, mat& y_mat)
{
	// ���Ƴ�ֵ
	rowvec x = EstimateInitialPosition_GaussianDeformation(ax, x0, cx, width);
	rowvec y = EstimateInitialPosition_GaussianDeformation(ay, y0, cy, height);

	// �Ծ���ֵ
	x_mat.zeros(height, width);
	y_mat.zeros(height, width);

	for (int r = 0; r < height; ++r)
	{
		for (int c = 0; c < width; ++c)
		{
			x_mat(r, c) = x(c);
			y_mat(r, c) = y(r);
		}

	}

}


void InverseMap::InverseMap_2DGaussianDeformation(const double& ax, const double x0, const double& cx, const int& width, const double& ay, const double& y0, const double& cy, const int& height, mat& x_mat, mat& y_mat)
{
	// ���Ƴ�ֵ
	rowvec deform_x_at_refer_estimate = EstimateInitialPosition_GaussianDeformation(ax, x0, cx, width);

	// ʹ��ţ�ٵ�����
	rowvec deform_x_at_refer = zeros<rowvec>(width);

	for (int i = 0; i < width; ++i)
	{
		double x = deform_x_at_refer_estimate(i);	// ��ֵ

		// ����
		double dx = 1;
		double f, fp, t;
		while (abs(dx) > 1e-4)
		{
			t = (x - x0) / cx;
			f = x + ax * exp(-t * t) - i;
			fp = 1 - 2 * ax / cx * t * exp(-t * t);
			dx = -f / fp;
			x = x + dx;
		}

		deform_x_at_refer(i) = x;
	}


	// ���Ƴ�ֵ
	rowvec deform_y_at_refer_estimate = EstimateInitialPosition_GaussianDeformation(ay, y0, cy, height);

	// ʹ��ţ�ٵ�����
	rowvec deform_y_at_refer = zeros<rowvec>(height);

	for (int i = 0; i < height; ++i)
	{
		double y = deform_y_at_refer_estimate(i);	// ��ֵ

		// ����
		double dy = 1;
		double f, fp, t;
		while (abs(dy) > 1e-4)
		{
			t = (y - y0) / cy;
			f = y + ay * exp(-t * t) - i;
			fp = 1 - 2 * ay / cy * t * exp(-t * t);
			dy = -f / fp;
			y = y + dy;
		}

		deform_y_at_refer(i) = y;
	}


	// λ�þ���
	x_mat.zeros(height, width);
	y_mat.zeros(height, width);

	for (int r = 0; r < height; ++r)
	{
		for (int c = 0; c < width; ++c)
		{
			x_mat(r, c) = deform_x_at_refer(c);
			y_mat(r, c) = deform_y_at_refer(r);
		}
	}



}



/***********************************************************************************************************************/
/***********************************************  �������ұ���  *********************************************************/
/***********************************************************************************************************************/

void InverseMap::EstimateInitialPosition_CrossSinusoidalDeformation(const double& a, const double Tx, const double& bx, const double& Ty, const double& by, const int& width, const int& height, mat& x_mat, mat& y_mat)
{
	// ��ʼ��
	mat deform_x_at_refer = LARGE_NEGTIVE_NUMBER * ones<mat>(height, width);


	// Ѱ�ұ���ͼ���ڲο�ͼλ�õĴ�ŷ�Χ
	const double xp_min = 0;
	const double xp_max = width - 1;

	const int delta_pad = 10;

	int pad = 0;
	int x_min = -pad;
	int x_max = pad + width - 1;

	while (x_min + SinusoidalDisplacement(a, Tx, bx, x_min) > xp_min ||
		x_max + SinusoidalDisplacement(a, Tx, bx, x_max) < xp_max)
	{
		pad += delta_pad;
		x_min = -pad;
		x_max = pad + width - 1;
	}


	x_mat.zeros(height, width);
	y_mat.zeros(height, width);

	for (int r = 0; r < height; ++r)
	{
		// �ο�ͼ������ڱ���ͼλ��
		const rowvec refer_x = regspace<rowvec>(x_min, x_max);

		const double aa = a * sin(2 * datum::pi * r / Ty + by);
		const rowvec refer_x_at_deform = refer_x + SinusoidalDisplacement(aa, Tx, bx, refer_x);


		// ���Ʋο�ͼÿһ�����ص��ڲο�ͼ�е�λ��
		double xl, xr;
		for (int c = 0; c < width; ++c)
		{
			for (int i = 0; i < refer_x.n_elem - 1; ++i)
			{
				xl = refer_x_at_deform(i);
				xr = refer_x_at_deform(i + 1);
				if (c >= xl && c < xr)
				{
					x_mat(r, c) = refer_x(i) + (c - xl) / (xr - xl);
					y_mat(r, c) = r;
					break;
				}
			}
		}


	}


}



void InverseMap::InverseMap_CrossSinusoidalDeformation(const double& a, const double Tx, const double& bx, const double& Ty, const double& by, const int& width, const int& height, mat& x_mat, mat& y_mat)
{
	// ���Ƴ�ֵ
	mat x_init;
	mat y_init;
	EstimateInitialPosition_CrossSinusoidalDeformation(a, Tx, bx, Ty, by, width, height, x_init, y_init);

	x_mat.zeros(height, width);
	y_mat.zeros(height, width);

	for (int r = 0; r < height; ++r)
	{
		for (int c = 0; c < width; ++c)
		{
			double x = x_init(r, c);	// ��ֵ
			double aa = a * sin(2 * datum::pi * r / Ty + by);

			// ����
			double dx = 1;
			double f, fp;
			while (abs(dx) > 1e-4)
			{
				f = x + aa * sin(2 * datum::pi * x / Tx + bx) - c;
				fp = 1 + 2 * datum::pi * aa / Tx * cos(2 * datum::pi * x / Tx + bx);
				dx = -f / fp;
				x = x + dx;
			}

			x_mat(r, c) = x;
			y_mat(r, c) = r;
		}

	}


}


/***********************************************************************************************************************/
/***********************************************  �����˹����  *********************************************************/
/***********************************************************************************************************************/


void InverseMap::EstimateInitialPosition_CrossGaussianDeformation(const double& a, const double x0, const double& cx, const double& y0, const double& cy, const int& width, const int& height, mat& x_mat, mat& y_mat)
{
	// ��ʼ��
	mat deform_x_at_refer = LARGE_NEGTIVE_NUMBER * ones<mat>(height, width);

	// Ѱ�ұ���ͼ���ڲο�ͼλ�õĴ�ŷ�Χ
	const double xp_min = 0;
	const double xp_max = width - 1;

	const int delta_pad = 10;

	int pad = 0;
	int x_min = -pad;
	int x_max = pad + width - 1;

	while (x_min + GaussianDisplacement(a, x0, cx, x_min) > xp_min ||
		x_max + GaussianDisplacement(a, x0, cx, x_max) < xp_max)
	{
		pad += delta_pad;
		x_min = -pad;
		x_max = pad + width - 1;
	}


	x_mat.zeros(height, width);
	y_mat.zeros(height, width);

	for (int r = 0; r < height; ++r)
	{
		// �ο�ͼ������ڱ���ͼλ��
		const rowvec refer_x = regspace<rowvec>(x_min, x_max);

		const double aa = a * exp(-(r - y0) * (r - y0) / (cy * cy));
		const rowvec refer_x_at_deform = refer_x + GaussianDisplacement(aa, x0, cx, refer_x);


		// ���Ʋο�ͼÿһ�����ص��ڲο�ͼ�е�λ��
		double xl, xr;
		for (int c = 0; c < width; ++c)
		{
			for (int i = 0; i < refer_x.n_elem - 1; ++i)
			{
				xl = refer_x_at_deform(i);
				xr = refer_x_at_deform(i + 1);
				if (c >= xl && c < xr)
				{
					x_mat(r, c) = refer_x(i) + (c - xl) / (xr - xl);
					y_mat(r, c) = r;
					break;
				}
			}
		}

	}


}


void InverseMap::InverseMap_CrossGaussianDeformation(const double& a, const double x0, const double& cx, const double& y0, const double& cy, const int& width, const int& height, mat& x_mat, mat& y_mat)
{
	// ���Ƴ�ֵ
	mat x_init;
	mat y_init;
	EstimateInitialPosition_CrossGaussianDeformation(a, x0, cx, y0, cy, width, height, x_init, y_init);

	x_mat.zeros(height, width);
	y_mat.zeros(height, width);

	for (int r = 0; r < height; ++r)
	{
		for (int c = 0; c < width; ++c)
		{
			double x = x_init(r,c);	// ��ֵ

			const double aa = a * exp(-(r - y0) * (r - y0) / (cy * cy));

			// ����
			double dx = 1;
			double f, fp, t;
			while (abs(dx) > 1e-4)
			{
				t = (x - x0) / cx;
				f = x + aa * exp(-t * t) - c;
				fp = 1 - 2 * aa / cx * t * exp(-t * t);
				dx = -f / fp;
				x = x + dx;
			}

			x_mat(r, c) = x;
			y_mat(r, c) = r;
		}

	}

}



/***********************************************************************************************************************/
/***********************************************  ��ָ������  *********************************************************/
/***********************************************************************************************************************/



rowvec InverseMap::EstimateInitialPosition_PowerDeformation(const double& a, const double x0, const double& n, const int& width)
{
	// ��ʼ��
	rowvec deform_x_at_refer = LARGE_NEGTIVE_NUMBER * ones<rowvec>(width);


	// Ѱ�ұ���ͼ���ڲο�ͼλ�õĴ�ŷ�Χ
	const double xp_min = 0;
	const double xp_max = width - 1;

	const int delta_pad = 10;

	int pad = 0;
	int x_min = -pad;
	int x_max = pad + width - 1;

	while (x_min + PowerDisplacement(a, x0, n, x_min) > xp_min ||
		x_max + PowerDisplacement(a, x0, n, x_max) < xp_max)
	{
		pad += delta_pad;
		x_min = -pad;
		x_max = pad + width - 1;
	}


	// �ο�ͼ������ڱ���ͼλ��
	const rowvec refer_x = regspace<rowvec>(x_min, x_max);
	const rowvec refer_x_at_deform = refer_x + PowerDisplacement(a, x0, n, refer_x);


	// ���Ʋο�ͼÿһ�����ص��ڲο�ͼ�е�λ��
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



rowvec InverseMap::InverseMap_PowerDeformation(const double& a, const double x0, const double& n, const int& width)
{
	// ���Ƴ�ֵ
	rowvec deform_x_at_refer_estimate = InverseMap::EstimateInitialPosition_PowerDeformation(a, x0, n, width);

	// ʹ��ţ�ٵ�����
	rowvec deform_x_at_refer = zeros<rowvec>(width);

	for (int i = 0; i < width; ++i)
	{
		double x = deform_x_at_refer_estimate(i);	// ��ֵ

		// ����
		double dx = 1;
		double f, fp, t;
		while (abs(dx) > 1e-4)
		{
			f = x + a * pow(x - x0, n) - i;
			fp = 1 + a * n * pow(x - x0, n - 1);
			dx = -f / fp;
			x = x + dx;
		}

		deform_x_at_refer(i) = x;
	}

	return deform_x_at_refer;
}
