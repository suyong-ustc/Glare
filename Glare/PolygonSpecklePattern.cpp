#include "PolygonSpecklePattern.h"
using namespace arma;


PolygonSpecklePattern::PolygonSpecklePattern(const double& diameter, const double& density, const double& variation, const int& edge, const double& ratio,
	const int& height, const int& width)
{
	// ɢ�߰뾶
	r_ = 0.5 * diameter;

	// ������
	ratio_ = ratio;

	// ɢ�߸���
	const double spacing = diameter / density;
	const int rows = static_cast<int>(floor(height / spacing));
	const int cols = static_cast<int>(floor(width / spacing));

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
	const double limit = 0.5 * variation * spacing;
	x_ = x_ + limit * 2 * (randu(rows + 1, cols + 1) - 1);
	y_ = y_ + limit * 2 * (randu(rows + 1, cols + 1) - 1);

	// ����ε���ת��
	theta_ = 360 * randu(rows + 1, cols + 1);

	// ����εı���
	edges_.zeros(rows + 1, cols + 1);
	if (edge >= 3)
		edges_.fill(edge);
	else
	{
		mat edges = floor(3 + 8 * randu(rows + 1, cols + 1));
		for (int r = 0; r <= rows; ++r)
			for (int c = 0; c <= cols; ++c)
				edges_(r, c) = static_cast<int>(edges(r, c));
	}

}


PolygonSpecklePattern::~PolygonSpecklePattern()
{

}
