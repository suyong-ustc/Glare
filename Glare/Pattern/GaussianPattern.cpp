#include "GaussianPattern.h"
using namespace arma;


GaussianPattern::GaussianPattern(const double& diameter, const double& coverage, const double& variation, const int& height, const int& width, const double& err)
{
	// 散斑网格点间距
	double spacing = diameter / coverage;

	// 散斑行数和列数
	const int rows = static_cast<int>(floor(height / spacing));
	const int cols = static_cast<int>(floor(width / spacing));

	x_.zeros(rows + 1, cols + 1);
	y_.zeros(rows + 1, cols + 1);

	// 将散斑置于网格点
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

	// 添加随机量
	double random_limit = 0.5 * variation * spacing;
	x_ = x_ + random_limit * 2 * (randu(rows + 1, cols + 1) - 1);
	y_ = y_ + random_limit * 2 * (randu(rows + 1, cols + 1) - 1);


	// 散斑半径相关
	radius_ = 0.5 * diameter;	
	r2_ = 1.0 / (radius_ * radius_);

	// 截断半径
	trunction_ = radius_ * sqrt(-log(err));

	// 预渲染图片
	buildCacheImage();
}


GaussianPattern::~GaussianPattern()
{


}


void GaussianPattern::buildCacheImage()
{
	// 预渲染图像的边界
	const int padding = 5;
	cache_xmin_ = static_cast<int>(floor(x_.min() - trunction_ - padding));
	cache_ymin_ = static_cast<int>(floor(y_.min() - trunction_ - padding));
	cache_xmax_ = static_cast<int>(ceil(x_.max() + trunction_ + padding));
	cache_ymax_ = static_cast<int>(ceil(y_.max() + trunction_ + padding));

	// 初始化预渲染图像
	const int cache_width = cache_xmax_ - cache_xmin_ + 1;
	const int cache_height = cache_ymax_ - cache_ymin_ + 1;
	cache_.zeros(cache_height, cache_width);

	// 计算预渲染图像
	for (int sc = 0; sc < x_.n_cols; ++sc)
	{
		for (int sr = 0; sr < x_.n_rows; ++sr)
		{
			// 第 (sr,sc) 个散斑的坐标
			const double speckle_x = x_(sr, sc);
			const double speckle_y = y_(sr, sc);

			// 第 (sr,sc) 个散斑的影响范围
			const int speckle_influence_xmin = static_cast<int>(floor(speckle_x - trunction_));
			const int speckle_influence_ymin = static_cast<int>(floor(speckle_y - trunction_));
			const int speckle_influence_xmax = static_cast<int>(ceil(speckle_x + trunction_));
			const int speckle_influence_ymax = static_cast<int>(ceil(speckle_y + trunction_));

			// 第 (sr,sc) 个散斑在图中的灰度
			for (int image_x = speckle_influence_xmin; image_x <= speckle_influence_xmax; ++image_x)
			{
				// (image_x, image_y) 到 第(sr,sc)个散斑中心 的 水平距离
				const double dx = image_x - speckle_x; 

				// (image_x, image_y) 在 cache 矩阵中的 列数
				const int cache_col = image_x - cache_xmin_;

				for (int image_y = speckle_influence_ymin; image_y <= speckle_influence_ymax; ++image_y)
				{
					// (image_x, image_y) 到 第(sr,sc)个散斑中心 的 竖直距离
					const double dy = image_y - speckle_y;
					const double ratio = (dx * dx + dy * dy) * r2_;
					const double delta = exp(-ratio);

					// (image_x, image_y) 在 cache 矩阵中的 行数
					const int cache_row = image_y - cache_ymin_;

					// 添加灰度
					cache_(cache_row, cache_col) = cache_(cache_row, cache_col) + exp(-ratio);
				}
			}

		}
	}

}



mat GaussianPattern::Values(const int& width, const int& height) const
{
	// 计算未量化高斯散斑的亮度
	mat data = zeros<mat>(height, width);

	// 量化并转化数据格式
	for (int sr = 0; sr < x_.n_rows; ++sr)
	{
		for (int sc = 0; sc < x_.n_cols; ++sc)
		{
			// 第 (sr,sc) 个散斑的坐标
			const double speckle_x = x_(sr, sc);
			const double speckle_y = y_(sr, sc);

			// 第 (sr,sc) 个散斑的影响范围
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

			// 第 (sr,sc) 个散斑在图中的灰度
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

	// 计算未量化高斯散斑的亮度
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
	// 计算未量化高斯散斑的亮度
	mat data = zeros<mat>(height, width);

	for (int c = 0; c < width; ++c)
	{
		// 如果在所有散斑的影响范围之外，直接设置为 0
		if (c < cache_xmin_ || c > cache_xmax_)
			continue;

		// 图像列数 c 在 cache 图像中对应的列数
		const int cache_col = c - cache_xmin_;

		for (int r = 0; r < height; ++r)
		{
			// 如果在所有散斑的影响范围之外，直接设置为 0
			if (r < cache_ymin_ || r > cache_ymax_)
				continue;

			// 图像行数 r 在 cache 图像中对应的行数
			const int cache_row = r - cache_ymin_;

			data(r, c) = cache_(cache_row, cache_col);
		}

	}

	return data;
}



mat GaussianPattern::EstimateValues(const mat& img_x, const mat& img_y) const
{
	// 图像尺寸
	const int height = img_x.n_rows;
	const int width = img_x.n_cols;

	// 计算未量化高斯散斑的亮度
	mat data = zeros<mat>(height, width);

	for (int c = 0; c < width; ++c)
	{
		for (int r = 0; r < height; ++r)
		{
			const double x = img_x(r, c);
			const double y = img_y(r, c);

			// 如果在所有散斑的影响范围之外，直接设置为 0
			if (x < cache_xmin_ || x > cache_xmax_ - 1 || y < cache_ymin_ || y > cache_ymax_ - 1)
				continue;

			// 在 cache 图像中的坐标
			const double cache_x = x - cache_xmin_;
			const double cache_y = y - cache_ymin_;

			// 通过线性插值估计灰度
			const int cache_x0 = static_cast<int>(floor(cache_x));
			const int cache_y0 = static_cast<int>(floor(cache_y));

			const double u = cache_x - cache_x0;
			const double v = cache_y - cache_y0;

			const double gm = (1 - u) * cache_(cache_y0, cache_x0) + u * cache_(cache_y0, cache_x0 + 1);
			const double gn = (1 - u) * cache_(cache_y0+1, cache_x0) + u * cache_(cache_y0+1, cache_x0 + 1);
			const double gq = (1 - v) * gm + v * gn;

			// 赋值
			data(r, c) = gq;
		}
	}

	return data;

}
