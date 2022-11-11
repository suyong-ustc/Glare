#include "EllipseSpecklePattern.h"
using namespace arma;


EllipseSpecklePattern::EllipseSpecklePattern(const double& diameter, const double& density, const double& variation, const double& eccentricity, const double& ratio,
	const int& height, const int& width)
{ 
	// 设置参数
	r_ = 0.5 * diameter;	// 散斑半径
	ecc_ = eccentricity;	// 椭圆的偏心率
	ratio_ = ratio;			// 椭圆的空心率
	
	// 散斑个数
	const double spacing = diameter / density;
	const int rows = static_cast<int>(height / spacing);
	const int cols = static_cast<int>(width / spacing);

	// 将散斑放置于网格点
	x_.zeros(rows + 1, cols + 1);
	y_.zeros(rows + 1, cols + 1);

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

	// 添加随机移动量
	const double limit = 0.5 * variation * spacing;					// 最大移动量
	x_ = x_ + limit * 2 * (randu(rows + 1, cols + 1) - 1);
	y_ = y_ + limit * 2 * (randu(rows + 1, cols + 1) - 1);

	// 椭圆的旋转角
	theta_ = 360 * randu(rows + 1, cols + 1);

}



EllipseSpecklePattern::~EllipseSpecklePattern()
{
}
