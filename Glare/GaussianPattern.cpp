#include "GaussianPattern.h"
using namespace arma;


GaussianPattern::GaussianPattern(const double& diameter, const double& coverage, const double& variation, const int& height, const int& width, const double& err)
{
	// ɢ���������
	double spacing = diameter / coverage;

	// ɢ������������
	const int rows = static_cast<int>(floor(height / spacing));
	const int cols = static_cast<int>(floor(width / spacing));

	x_.zeros(rows + 1, cols + 1);
	y_.zeros(rows + 1, cols + 1);

	// ��ɢ�����������
	const double xmin = 0.5 * (width - cols * spacing);
	const double ymin = 0.5 * (height - rows * spacing);
	for (int r = 0; r <= rows; ++r)
	{
		for (int c = 0; c <= cols; ++c)
		{
			x_(r, c) = xmin + c * spacing;
			y_(r, c) = ymin + r * spacing;
		}
	}

	// ��������
	double random_limit = 0.5 * variation * spacing;
	x_ = x_ + random_limit * 2 * (randu(rows + 1, cols + 1) - 1);
	y_ = y_ + random_limit * 2 * (randu(rows + 1, cols + 1) - 1);


	// ɢ�߰뾶���
	radius_ = 0.5 * diameter;	
	r2_ = 1.0 / (radius_ * radius_);

	// �ضϰ뾶
	trunction_ = radius_ * sqrt(-log(err));

	// Ԥ��ȾͼƬ
	buildCacheImage();
}


GaussianPattern::~GaussianPattern()
{


}


void GaussianPattern::buildCacheImage()
{
	// Ԥ��Ⱦͼ��ı߽�
	const int padding = 5;
	cache_xmin_ = static_cast<int>(floor(x_.min() - trunction_ - padding));
	cache_ymin_ = static_cast<int>(floor(y_.min() - trunction_ - padding));
	cache_xmax_ = static_cast<int>(ceil(x_.max() + trunction_ + padding));
	cache_ymax_ = static_cast<int>(ceil(y_.max() + trunction_ + padding));

	// ��ʼ��Ԥ��Ⱦͼ��
	const int cache_width = cache_xmax_ - cache_xmin_ + 1;
	const int cache_height = cache_ymax_ - cache_ymin_ + 1;
	cache_.zeros(cache_height, cache_width);

	// ����Ԥ��Ⱦͼ��
	for (int sc = 0; sc < x_.n_cols; ++sc)
	{
		for (int sr = 0; sr < x_.n_rows; ++sr)
		{
			// �� (sr,sc) ��ɢ�ߵ�����
			const double speckle_x = x_(sr, sc);
			const double speckle_y = y_(sr, sc);

			// �� (sr,sc) ��ɢ�ߵ�Ӱ�췶Χ
			const int speckle_influence_xmin = static_cast<int>(floor(speckle_x - trunction_));
			const int speckle_influence_ymin = static_cast<int>(floor(speckle_y - trunction_));
			const int speckle_influence_xmax = static_cast<int>(ceil(speckle_x + trunction_));
			const int speckle_influence_ymax = static_cast<int>(ceil(speckle_y + trunction_));

			// �� (sr,sc) ��ɢ����ͼ�еĻҶ�
			for (int image_x = speckle_influence_xmin; image_x <= speckle_influence_xmax; ++image_x)
			{
				// (image_x, image_y) �� ��(sr,sc)��ɢ������ �� ˮƽ����
				const double dx = image_x - speckle_x; 

				// (image_x, image_y) �� cache �����е� ����
				const int cache_col = image_x - cache_xmin_;

				for (int image_y = speckle_influence_ymin; image_y <= speckle_influence_ymax; ++image_y)
				{
					// (image_x, image_y) �� ��(sr,sc)��ɢ������ �� ��ֱ����
					const double dy = image_y - speckle_y;
					const double ratio = (dx * dx + dy * dy) * r2_;
					const double delta = exp(-ratio);

					// (image_x, image_y) �� cache �����е� ����
					const int cache_row = image_y - cache_ymin_;

					// ��ӻҶ�
					cache_(cache_row, cache_col) = cache_(cache_row, cache_col) + exp(-ratio);
				}
			}

		}
	}

}



mat GaussianPattern::Values(const int& width, const int& height) const
{
	// ����δ������˹ɢ�ߵ�����
	mat data = zeros<mat>(height, width);

	// ������ת�����ݸ�ʽ
	for (int sr = 0; sr < x_.n_rows; ++sr)
	{
		for (int sc = 0; sc < x_.n_cols; ++sc)
		{
			// �� (sr,sc) ��ɢ�ߵ�����
			const double speckle_x = x_(sr, sc);
			const double speckle_y = y_(sr, sc);

			// �� (sr,sc) ��ɢ�ߵ�Ӱ�췶Χ
			int xmin = floor(speckle_x - trunction_);
			if (xmin < 0)
				xmin = 0;

			int xmax = ceil(speckle_x + trunction_);
			if (xmax >= width)
				xmax = width - 1;

			int ymin = floor(speckle_y - trunction_);
			if (ymin < 0)
				ymin = 0;

			int ymax = ceil(speckle_y + trunction_);
			if (ymax >= height)
				ymax = height - 1;

			// �� (sr,sc) ��ɢ����ͼ�еĻҶ�
			for (int ir = ymin; ir <= ymax; ++ir)
			{
				const double dy = ir - speckle_y;
				for (int ic = xmin; ic <= xmax; ++ic)
				{
					const double dx = ic - speckle_x;
					const double ratio = (dx * dx + dy * dy) * r2_;
					data(ir, ic) = data(ir, ic) + exp(-ratio);
				}
			}

		}
	}

	return data;
}




mat GaussianPattern::Values(const mat& img_x, const mat& img_y) const
{
	const int height = img_x.n_rows;
	const int width = img_x.n_cols;

	// ����δ������˹ɢ�ߵ�����
	mat data = zeros<mat>(height, width);

	for (int r = 0; r < height; ++r)
	{
		for (int c = 0; c < width; ++c)
		{
			double intensity = 0;

			for (int sr = 0; sr < x_.n_rows; ++sr)
			{
				for (int sc = 0; sc < x_.n_cols; ++sc)
				{
					const double dy = img_y(r, c) - y_(sr, sc);
					const double dx = img_x(r, c) - x_(sr, sc);

					if (abs(dx) > trunction_ || abs(dy) > trunction_)
						continue;

					const double ratio = (dx * dx + dy * dy) * r2_;
					intensity = intensity + exp(-ratio);

				}

			}

			data(r, c) = intensity;
		}

	}


	return data;

}



mat GaussianPattern::EstimateValues(const int& width, const int& height) const
{
	// ����δ������˹ɢ�ߵ�����
	mat data = zeros<mat>(height, width);

	for (int c = 0; c < width; ++c)
	{
		// ���������ɢ�ߵ�Ӱ�췶Χ֮�⣬ֱ������Ϊ 0
		if (c < cache_xmin_ || c > cache_xmax_)
			continue;

		// ͼ������ c �� cache ͼ���ж�Ӧ������
		const int cache_col = c - cache_xmin_;

		for (int r = 0; r < height; ++r)
		{
			// ���������ɢ�ߵ�Ӱ�췶Χ֮�⣬ֱ������Ϊ 0
			if (r < cache_ymin_ || r > cache_ymax_)
				continue;

			// ͼ������ r �� cache ͼ���ж�Ӧ������
			const int cache_row = r - cache_ymin_;

			data(r, c) = cache_(cache_row, cache_col);
		}

	}

	return data;
}



mat GaussianPattern::EstimateValues(const mat& img_x, const mat& img_y) const
{
	// ͼ��ߴ�
	const int height = img_x.n_rows;
	const int width = img_x.n_cols;

	// ����δ������˹ɢ�ߵ�����
	mat data = zeros<mat>(height, width);

	for (int c = 0; c < width; ++c)
	{
		for (int r = 0; r < height; ++r)
		{
			const double x = img_x(r, c);
			const double y = img_y(r, c);

			// ���������ɢ�ߵ�Ӱ�췶Χ֮�⣬ֱ������Ϊ 0
			if (x < cache_xmin_ || x > cache_xmax_ - 1 || y < cache_ymin_ || y > cache_ymax_ - 1)
				continue;

			// �� cache ͼ���е�����
			const double cache_x = x - cache_xmin_;
			const double cache_y = y - cache_ymin_;

			// ͨ�����Բ�ֵ���ƻҶ�
			const int cache_x0 = static_cast<int>(floor(cache_x));
			const int cache_y0 = static_cast<int>(floor(cache_y));

			const double u = cache_x - cache_x0;
			const double v = cache_y - cache_y0;

			const double gm = (1 - u) * cache_(cache_y0, cache_x0) + u * cache_(cache_y0, cache_x0 + 1);
			const double gn = (1 - u) * cache_(cache_y0+1, cache_x0) + u * cache_(cache_y0+1, cache_x0 + 1);
			const double gq = (1 - v) * gm + v * gn;

			// ��ֵ
			data(r, c) = gq;
		}
	}

	return data;

}
