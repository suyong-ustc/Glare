#include "AssessSpecklePattern.h"

AssessSpecklePattern::AssessSpecklePattern(QObject *parent)
	: QObject(parent)
{
}



AssessSpecklePattern::~AssessSpecklePattern()
{
}



arma::mat AssessSpecklePattern::Uchar2ArmaMat(uchar* data, int h, int w)
{
	arma::mat image = arma::zeros(h, w);

	for (int r = 0; r < h; ++r)
	{
		uchar* data_row = data + r * w;
		for (int c = 0; c < w; ++c)
		{
			image(r, c) = data_row[c];
		}
	}

	return image;
}



double AssessSpecklePattern::Sinc(const double& f)
{
	return abs(f) < 1e-10 ? 1 : sin(CV_PI*f) / (CV_PI*f);
}



double AssessSpecklePattern::TransferFunction_Keys(const double& f)
{
	const double sinc = Sinc(f);
	const double sinc3 = sinc * sinc * sinc;
	return sinc3 * (3 * sinc - 2 * cos(CV_PI*f));
}



double AssessSpecklePattern::TransferFunction_BSpline3(const double& f)
{
	const double sinc = Sinc(f);
	const double sinc2 = sinc * sinc;
	const double sinc4 = sinc2 * sinc2;
	return 3 * sinc4 / (2 + cos(2 * CV_PI*f));
}



double AssessSpecklePattern::TransferFunction_BSpline5(const double& f)
{
	const double sinc = Sinc(f);
	const double sinc3 = sinc * sinc * sinc;
	const double sinc6 = sinc3 * sinc3;
	return 60 * sinc6 / (33 + 26 * cos(2 * CV_PI*f) + cos(4 * CV_PI*f));
}




double AssessSpecklePattern::TransferFunction_BSpline7(const double& f)
{
	const double sinc = Sinc(f);
	const double sinc2 = sinc * sinc;
	const double sinc4 = sinc2 * sinc2;
	const double sinc8 = sinc4 * sinc4;
	return 2520 * sinc8 / (1208 + 1191 * cos(2 * CV_PI*f) + 120 * cos(4 * CV_PI*f) + cos(6 * CV_PI*f));
}



double AssessSpecklePattern::TrapezoidIntegral(const cv::Mat& a)
{
	cv::Mat b(a.rows, 1, CV_64F);

	for (int r = 0; r < a.rows; ++r)
	{
		b.at<double>(r, 0) = sum(a.row(r))[0] - 0.5 * (a.at<double>(r, 0) + a.at<double>(r, a.cols - 1));
	}

	double integral = sum(b)[0] - 0.5 * (b.at<double>(0, 0) + b.at<double>(a.rows - 1, 0));
	return integral;
}



cv::Mat AssessSpecklePattern::PowerSpectrum(const cv::Mat& I)
{
	cv::Mat padded;
	const int m = cv::getOptimalDFTSize(I.rows);
	const int n = cv::getOptimalDFTSize(I.cols);
	copyMakeBorder(I, padded, 0, m - I.rows, 0, n - I.cols, cv::BORDER_CONSTANT, cv::Scalar::all(0));

	cv::Mat planes[] = { cv::Mat_<double>(padded), cv::Mat::zeros(padded.size(), CV_64F) };
	cv::Mat complexI;
	merge(planes, 2, complexI);

	cv::dft(complexI, complexI);

	split(complexI, planes);
	magnitude(planes[0], planes[1], planes[0]);
	cv::Mat powerSpecI;
	pow(planes[0], 2, powerSpecI);

	const int cx = powerSpecI.cols / 2;
	const int cy = powerSpecI.rows / 2;
	cv::Mat q0(powerSpecI, cv::Rect(0, 0, cx, cy));
	cv::Mat q1(powerSpecI, cv::Rect(cx, 0, cx, cy));
	cv::Mat q2(powerSpecI, cv::Rect(0, cy, cx, cy));
	cv::Mat q3(powerSpecI, cv::Rect(cx, cy, cx, cy));

	cv::Mat tmp;
	q0.copyTo(tmp);
	q3.copyTo(q0);
	tmp.copyTo(q3);

	q1.copyTo(tmp);
	q2.copyTo(q1);
	tmp.copyTo(q2);

	return powerSpecI;
}



void AssessSpecklePattern::Kernel_Keys(const cv::Size& sz, cv::Mat& bias_kernel, cv::Mat& sssig_kernel)
{
	bias_kernel = cv::Mat::zeros(sz, CV_64F);
	sssig_kernel = cv::Mat::zeros(sz, CV_64F);

	const int m = sz.height;
	const int n = sz.width;
	for (int c = 0; c < n; ++c)
	{
		const double fx = -0.5 + c / double(n);
		const double tfx = TransferFunction_Keys(fx);
		const double tfr = TransferFunction_Keys(fx - 1);
		const double tfl = TransferFunction_Keys(fx + 1);
		for (int r = 0; r < m; ++r)
		{
			const double fy = -0.5 + r / double(m);
			const double tfy = TransferFunction_Keys(fy);
			bias_kernel.at<double>(r, c) = (fx - 1) * tfr * tfy - (fx + 1) * tfl * tfy + tfx * tfy * (tfr * tfy + tfl * tfy);
			sssig_kernel.at<double>(r, c) = fx * fx * tfx * tfy;
		}
	}

}



void AssessSpecklePattern::Kernel_BSpline3(const cv::Size& sz, cv::Mat& bias_kernel, cv::Mat& sssig_kernel)
{
	bias_kernel = cv::Mat::zeros(sz, CV_64F);
	sssig_kernel = cv::Mat::zeros(sz, CV_64F);

	const int m = sz.height;
	const int n = sz.width;
	for (int c = 0; c < n; ++c)
	{
		const double fx = -0.5 + c / double(n);
		const double tfx = TransferFunction_BSpline3(fx);
		const double tfr = TransferFunction_BSpline3(fx - 1);
		const double tfl = TransferFunction_BSpline3(fx + 1);
		for (int r = 0; r < m; ++r)
		{
			const double fy = -0.5 + r / double(m);
			const double tfy = TransferFunction_BSpline3(fy);
			bias_kernel.at<double>(r, c) = (fx - 1) * tfr * tfy - (fx + 1) * tfl * tfy + tfx * tfy * (tfr * tfy + tfl * tfy);
			sssig_kernel.at<double>(r, c) = fx * fx * tfx * tfy;
		}
	}



}



void AssessSpecklePattern::Kernel_BSpline5(const cv::Size& sz, cv::Mat& bias_kernel, cv::Mat& sssig_kernel)
{

	bias_kernel = cv::Mat::zeros(sz, CV_64F);
	sssig_kernel = cv::Mat::zeros(sz, CV_64F);

	const int m = sz.height;
	const int n = sz.width;
	for (int c = 0; c < n; ++c)
	{
		const double fx = -0.5 + c / double(n);
		const double tfx = TransferFunction_BSpline5(fx);
		const double tfr = TransferFunction_BSpline5(fx - 1);
		const double tfl = TransferFunction_BSpline5(fx + 1);
		for (int r = 0; r < m; ++r)
		{
			const double fy = -0.5 + r / double(m);
			const double tfy = TransferFunction_BSpline5(fy);
			bias_kernel.at<double>(r, c) = (fx - 1) * tfr * tfy - (fx + 1) * tfl * tfy + tfx * tfy * (tfr * tfy + tfl * tfy);
			sssig_kernel.at<double>(r, c) = fx * fx * tfx * tfy;
		}
	}




}



void AssessSpecklePattern::Kernel_BSpline7(const cv::Size& sz, cv::Mat& bias_kernel, cv::Mat& sssig_kernel)
{

	bias_kernel = cv::Mat::zeros(sz, CV_64F);
	sssig_kernel = cv::Mat::zeros(sz, CV_64F);

	const int m = sz.height;
	const int n = sz.width;
	for (int c = 0; c < n; ++c)
	{
		const double fx = -0.5 + c / double(n);
		const double tfx = TransferFunction_BSpline7(fx);
		const double tfr = TransferFunction_BSpline7(fx - 1);
		const double tfl = TransferFunction_BSpline7(fx + 1);
		for (int r = 0; r < m; ++r)
		{
			const double fy = -0.5 + r / double(m);
			const double tfy = TransferFunction_BSpline7(fy);
			bias_kernel.at<double>(r, c) = (fx - 1) * tfr * tfy - (fx + 1) * tfl * tfy + tfx * tfy * (tfr * tfy + tfl * tfy);
			sssig_kernel.at<double>(r, c) = fx * fx * tfx * tfy;
		}
	}



}



double AssessSpecklePattern::InterpolationBias(const cv::Mat& I, const InterpolationAlgorithm& interp_type)
/* 估计插值偏差 */
{
	cv::Mat powerSpecI(PowerSpectrum(I));

	cv::Mat bias_kernel, sssig_kernel;
	if (interp_type == KEYS_INTERPOLATION)
		Kernel_Keys(powerSpecI.size(), bias_kernel, sssig_kernel);
	else if (interp_type == CUBIC_BSPLINE_INTERPOLATION)
		Kernel_BSpline3(powerSpecI.size(), bias_kernel, sssig_kernel);
	else if (interp_type == QUINTIC_BSPLINE_INTERPOLATION)
		Kernel_BSpline5(powerSpecI.size(), bias_kernel, sssig_kernel);
	else if (interp_type == SEPTIC_BSPLINE_INTERPOLATION)
		Kernel_BSpline7(powerSpecI.size(), bias_kernel, sssig_kernel);

	cv::Mat a = bias_kernel.mul(powerSpecI);
	cv::Mat b = sssig_kernel.mul(powerSpecI);
	double ue = 0.5 / CV_PI * TrapezoidIntegral(a) / TrapezoidIntegral(b);

	return ue;
}



double AssessSpecklePattern::OtsuThreshold(cv::Mat ori_image, int total_level)
{
	cv::Mat src;
	if (ori_image.type() != CV_32F)
	{
		ori_image.convertTo(src, CV_32F);
	}
	else
		src = ori_image;

	int N = total_level;           //整个图分的等级
	double max_value;
	cv::minMaxLoc(ori_image, NULL, &max_value);
	double per_d = (max_value + 1) / N;

	cv::Mat fix_mat = cv::Mat(ori_image.rows, ori_image.cols, CV_16U);

	for (int i = 0; i < src.rows; i++)
	{
		float* _src = src.ptr<float>(i);
		ushort* _fix_mat = fix_mat.ptr<ushort>(i);
		for (int j = 0; j < src.cols; j++)
		{
			_fix_mat[j] = floor(_src[j] / per_d);
		}
	}

	int image_width = ori_image.cols;
	int image_height = ori_image.rows;


	//int pn[N] ={0};                //直方图的比例
	std::vector<int> pn;
	for (int i = 0; i < N; i++)
	{
		pn.push_back(0);
	}

	for (int i = 0; i < image_height; i++)
	{
		ushort* _fix_mat = fix_mat.ptr<ushort>(i);
		for (int j = 0; j < image_width; j++)
		{
			pn[int(_fix_mat[j])]++;
		}
	}

	double scale = 1. / (image_width*image_height);
	double mean = 0;
	for (int i = 0; i < N; i++)
	{
		mean += i * (double)pn[i];
	}
	mean *= scale;

	double q1 = 0, mean1 = 0;
	double max_sigma = 0, max_val = 0;

	for (int i = 0; i < N; i++)
	{
		double p_i, q2, mean2, sigma;

		p_i = pn[i] * scale;
		mean1 *= q1;
		q1 += p_i;
		q2 = 1. - q1;

		if (std::min(q1, q2) < FLT_EPSILON || std::max(q1, q2) > 1. - FLT_EPSILON)
			continue;

		mean1 = (mean1 + i * p_i) / q1;
		mean2 = (mean - q1 * mean1) / q2;

		sigma = q1 * q2*(mean1 - mean2)*(mean1 - mean2);
		//sigma = q1*(mean1 - mean)*(mean1 - mean) + q2*(mean2 - mean)*(mean2 - mean);

		if (sigma > max_sigma)
		{
			max_sigma = sigma;
			max_val = i;
		}
	}

	return max_val * per_d;

}



arma::vec AssessSpecklePattern::Normalize(const arma::mat& data)
{
	// 向量化
	arma::vec v = arma::vectorise(data);

	// 减去均值
	v = v - arma::mean(v);

	// 正则化
	v = arma::normalise(v);

	// 返回值
	return v;

}



void AssessSpecklePattern::CalculateAutoCorrelation(const arma::mat& image, const int& subset_size, const int& maximal_translation, 
	arma::vec& zncc_x,  arma::vec& zncc_y)
{
	// 初始化
	zncc_x = arma::zeros<arma::vec>(2 * maximal_translation + 1);
	zncc_y = arma::zeros<arma::vec>(2 * maximal_translation + 1);

	// 图像分辨率
	const int height = image.n_rows;
	const int width = image.n_cols;

	// 子区大小的一半
	const int half_subset_size = (subset_size - 1) / 2;

	// 参考子区
	const int x0 = width / 2;
	const int y0 = height / 2;

	const int xmin = x0 - half_subset_size;
	const int xmax = x0 + half_subset_size;
	const int ymin = y0 - half_subset_size;
	const int ymax = y0 + half_subset_size;

	// 如果参考子区范围超过图像范围
	if (xmin < 0 || xmax >= width || ymin < 0 || ymax >= height)
		return;

	const arma::mat refer_subset = image(arma::span(ymin, ymax), arma::span(xmin, xmax));
	const arma::vec normalized_refer_subset_vector = Normalize(refer_subset);


	// 计算 x 方向相关系数

	for (int dx = -maximal_translation; dx <= maximal_translation; ++dx)
	{
		// 平移子区超过图像范围
		if (xmin + dx < 0 || xmax + dx >= width)
		{
			zncc_x(dx + maximal_translation) = 0;
			continue;
		}

		// 平移子区灰度
		const arma::mat translated_subset = image(arma::span(ymin, ymax), arma::span(xmin + dx, xmax + dx));
		const arma::vec normalized_translated_subset_vector = Normalize(translated_subset);

		// 相关系数
		const double zncc = arma::dot(normalized_refer_subset_vector, normalized_translated_subset_vector);
		zncc_x(dx + maximal_translation) = zncc;
	}


	// 计算 y 方向相关系数

	for (int dy = -maximal_translation; dy <= maximal_translation; ++dy)
	{
		// 平移子区超过图像范围
		if (ymin + dy < 0 || ymax + dy >= height)
		{
			zncc_y(dy + maximal_translation) = 0;
			continue;
		}

		// 平移子区灰度
		const arma::mat translated_subset = image(arma::span(ymin + dy, ymax + dy), arma::span(xmin, xmax));
		const arma::vec normalized_translated_subset_vector = Normalize(translated_subset);

		// 相关系数
		const double zncc = arma::dot(normalized_refer_subset_vector, normalized_translated_subset_vector);
		zncc_y(dy + maximal_translation) = zncc;
	}

}



double AssessSpecklePattern::CalculateSpeckleSize(const arma::vec& zncc_x, const arma::vec& zncc_y)
{
	// 中心位置
	const int n = (zncc_x.n_elem - 1) / 2;

	// x+ 方向的半高宽
	double x_plus = -1;
	for (int i = n; i < 2 * n; ++i)
	{
		const double c0 = zncc_x(i);
		const double c1 = zncc_x(i + 1);

		if (c0 >= 0.5 && c1 < 0.5)
		{
			x_plus = i + (c0 - 0.5) / (c0 - c1);
			break;
		}
	}


	// x- 方向的半高宽
	double x_minus = -1;
	for (int i = n; i > 0; --i)
	{
		const double c0 = zncc_x(i);
		const double c1 = zncc_x(i - 1);

		if (c0 >= 0.5 && c1 < 0.5)
		{
			x_minus = i - (c0 - 0.5) / (c0 - c1);
			break;
		}
	}

	// y+ 方向的半高宽
	double y_plus = -1;
	for (int i = n; i < 2 * n; ++i)
	{
		const double c0 = zncc_y(i);
		const double c1 = zncc_y(i + 1);

		if (c0 >= 0.5 && c1 < 0.5)
		{
			y_plus = i + (c0 - 0.5) / (c0 - c1);
			break;
		}
	}


	// y- 方向的半高宽
	double y_minus = -1;
	for (int i = n; i > 0; --i)
	{
		const double c0 = zncc_y(i);
		const double c1 = zncc_y(i - 1);

		if (c0 >= 0.5 && c1 < 0.5)
		{
			y_minus = i - (c0 - 0.5) / (c0 - c1);
			break;
		}
	}

	// 如果没找到
	if (x_plus < 0 || x_minus < 0 || y_plus < 0 || y_minus < 0)
		return 0;

	// 计算散斑大小
	const double diameter_x = x_plus - x_minus;
	const double diameter_y = y_plus - y_minus;
	const double diameter = sqrt(diameter_x * diameter_y);

	return diameter;
}



void AssessSpecklePattern::CalculateImageGradients(const arma::mat& image, arma::mat& gradient_x, arma::mat& gradient_y)
{	
	// 图像尺寸
	const int h = image.n_rows;
	const int w = image.n_cols;

	// 梯度初始化
	gradient_x.zeros(h, w);
	gradient_y.zeros(h, w);

	// 使用 Sobel 算子计算梯度
	for (int y = 1; y < h - 1; ++y)
	{
		for (int x = 1; x < w - 1; ++x)
		{
			gradient_x(y, x) = (image(y - 1, x + 1) + 2. * image(y, x + 1) + image(y + 1, x + 1)
				- image(y - 1, x - 1) - 2. * image(y, x - 1) - image(y + 1, x - 1)) / 8.0;

			gradient_y(y, x) = (image(y + 1, x - 1) + 2. * image(y + 1, x) + image(y + 1, x + 1)
				- image(y - 1, x - 1) - 2. * image(y - 1, x) - image(y - 1, x + 1)) / 8.0;
		}
	}

}



arma::mat AssessSpecklePattern::CalculateIntegralImage(const arma::mat& image)
{
	// 图像大小
	const int h = image.n_rows;
	const int w = image.n_cols;

	// 计算求和表
	arma::mat integral_image = arma::zeros(h, w);

	// 左上角赋值
	integral_image(0, 0) = image(0, 0);

	// 第一行赋值
	for (int c = 1; c < w; ++c)
		integral_image(0, c) = integral_image(0, c - 1) + image(0, c);

	// 第一列赋值
	for (int r = 1; r < h; ++r)
		integral_image(r, 0) = integral_image(r - 1, 0) + image(r, 0);

	// 剩余积分图像赋值
	for (int c = 1; c < w; ++c)
	{
		for (int r = 1; r < h; ++r)
		{
			integral_image(r, c) =
				integral_image(r - 1, c)
				+ integral_image(r, c - 1)
				- integral_image(r - 1, c - 1)
				+ image(r, c);
		}

	}

	return integral_image;

}