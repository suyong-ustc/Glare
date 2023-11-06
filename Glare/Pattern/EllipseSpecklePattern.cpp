#include "EllipseSpecklePattern.h"
using namespace arma;


EllipseSpecklePattern::EllipseSpecklePattern(const double& diameter, const double& density, const double& variation, const double& eccentricity, const double& ratio,
	const int& height, const int& width)
{ 
	// ���ò���
	r_ = 0.5 * diameter;	// ɢ�߰뾶
	ecc_ = eccentricity;	// ��Բ��ƫ����
	ratio_ = ratio;			// ��Բ�Ŀ�����
	
	// ɢ�߸���
	const double spacing = diameter / density;
	const int rows = static_cast<int>(height / spacing);
	const int cols = static_cast<int>(width / spacing);

	// ��ɢ�߷����������
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

	// �������ƶ���
	const double limit = 0.5 * variation * spacing;					// ����ƶ���
	x_ = x_ + limit * 2 * (randu(rows + 1, cols + 1) - 1);
	y_ = y_ + limit * 2 * (randu(rows + 1, cols + 1) - 1);

	// ��Բ����ת��
	theta_ = 360 * randu(rows + 1, cols + 1);

}



EllipseSpecklePattern::~EllipseSpecklePattern()
{
}
