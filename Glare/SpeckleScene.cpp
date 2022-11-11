#include "SpeckleScene.h"
#include <armadillo>
#include <QBuffer>
#include <QImage>
#include "EllipseSpeckleItem.h"
#include "PolygonSpeckleItem.h"
using namespace arma;


SpeckleScene::SpeckleScene()
{
}



SpeckleScene::~SpeckleScene()
{
}



void SpeckleScene::PaintEllipseSpecklePattern(EllipseSpecklePattern* ellipse_pattern, const BackgroundColor& background)
{
	// 散斑参数
	const mat theta = ellipse_pattern->rotation();
	const mat x = ellipse_pattern->x();
	const mat y = ellipse_pattern->y();
	const double radius = ellipse_pattern->r();
	const double ratio = ellipse_pattern->ratio();
	const double eccentricity = ellipse_pattern->eccentricity();

	// 椭圆的长短轴
	const double major_radius = radius / sqrt(sqrt(1 - eccentricity * eccentricity));
	const double minor_radius = radius * sqrt(sqrt(1 - eccentricity * eccentricity));

	// 前景和背景色
	QColor backgroundColor, foregroundColor;
	if (background == BLACK_BACKGROUND) {
		foregroundColor = Qt::white;
		backgroundColor = Qt::black;
	}
	else {
		foregroundColor = Qt::black;
		backgroundColor = Qt::white;
	}

	// 绘制
	clear();
	setBackgroundBrush(QBrush(backgroundColor));

	for (int r = 0; r < x.n_rows; ++r)
	{
		for (int c = 0; c < x.n_cols; ++c)
		{
			EllipseSpeckleItem* ellipse = new EllipseSpeckleItem(major_radius, minor_radius, ratio, foregroundColor);

			addItem(ellipse);
			ellipse->setPos(x(r, c), y(r, c));
			ellipse->setRotation(theta(r, c));
			ellipse->show();

		}
	}


}



void SpeckleScene::PaintPolygonSpecklePattern(PolygonSpecklePattern* polygon_pattern, const BackgroundColor& background)
{
	// 散斑参数
	const mat theta = polygon_pattern->rotation();
	const mat x = polygon_pattern->x();
	const mat y = polygon_pattern->y();
	const imat edges = polygon_pattern->edges();
	const double radius = polygon_pattern->radius();
	const double ratio = polygon_pattern->ratio();

	// 直径
	const double diameter = 2 * radius;

	// 前景和背景色
	QColor backgroundColor, foregroundColor;
	if (background == BLACK_BACKGROUND) {
		foregroundColor = Qt::white;
		backgroundColor = Qt::black;
	}
	else {
		foregroundColor = Qt::black;
		backgroundColor = Qt::white;
	}

	// 绘制
	clear();
	setBackgroundBrush(QBrush(backgroundColor));

	for (int r = 0; r < x.n_rows; ++r)
	{
		for (int c = 0; c < x.n_cols; ++c)
		{
			PolygonSpeckleItem* polygon = new PolygonSpeckleItem(diameter, edges(r, c), ratio, foregroundColor);

			addItem(polygon);
			polygon->setPos(x(r, c), y(r, c));
			polygon->setRotation(theta(r, c));
			polygon->show();

		}
	}


}



void SpeckleScene::PaintGaussianSpecklePattern(GaussianPattern* gaussian_pattern, const int& height, const int& width, const BackgroundColor& background, const double& err)
{
	// 散斑参数
	const mat x = gaussian_pattern->x();
	const mat y = gaussian_pattern->y();
	const double radius = gaussian_pattern->radius();


	// 前景和背景色
	double background_color, foreground_color;
	if (background == BLACK_BACKGROUND) {
		background_color = 0;
		foreground_color = 255;
	}
	else {
		foreground_color = 0;
		background_color =255;
	}


	// 绘制

	mat data = gaussian_pattern->Values(width, height);

	// 量化并转化数据格式
	const double minimum = data.min();
	const double maximum = data.max();

	const double k = (foreground_color - background_color) / (maximum - minimum);

	mat q = k * (data - minimum) + background_color;
	q = round(q);


	uchar* img = new uchar[height*width]();
	for (int r = 0; r < height; ++r)
	{
		uchar* data_row = img + r * width;

		for (int c = 0; c < width; ++c)
		{
			data_row[c] = static_cast<uchar>(q(r, c));
		}
	}


	// 显示图像
	QImage qimg(img, width, height, QImage::Format_Indexed8);
	QVector<QRgb>  colorTable;
	for (int k = 0; k < 256; ++k)
		colorTable.push_back(qRgb(k, k, k));
	qimg.setColorTable(colorTable);
	QPixmap pixmap = QPixmap::fromImage(qimg);

	clear();
	QGraphicsPixmapItem* pixmapItem = addPixmap(pixmap);
	pixmapItem->show();

	delete[] img;

}