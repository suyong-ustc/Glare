#include "RenderPatternPixmap.h"
#include <QImage>
#include <QPainterPath>
#include <QPointF>
#include <QVector>
using namespace arma;


RenderPatternPixmap::RenderPatternPixmap(QObject *parent)
	: QObject(parent)
{

}


RenderPatternPixmap::~RenderPatternPixmap()
{

}



QPixmap RenderPatternPixmap::RenderEllipsePatternPixmap(EllipseSpecklePattern* ellipse_pattern, const int& height, const int& width,
	const QPainter::RenderHint& render_hint, const Qt::GlobalColor& background_color)
{
	// 初始化 pixmap
	QPixmap pixmap(width, height);

	QPainter painter(&pixmap);
	painter.setPen(Qt::NoPen);

	if (render_hint == QPainter::Antialiasing)
		painter.setRenderHint(QPainter::Antialiasing, true);
	else
		painter.setRenderHint(QPainter::Antialiasing, false);

	if (background_color == Qt::black) {
		pixmap.fill(Qt::black);
		painter.setBrush(QBrush(Qt::white));
	}
	else {
		pixmap.fill(Qt::white);
		painter.setBrush(QBrush(Qt::black));
	}

	// 散斑参数
	const double radius = ellipse_pattern->r();
	const double eccentricity = ellipse_pattern->eccentricity();
	const double ratio = ellipse_pattern->ratio();
	const mat x = ellipse_pattern->x();
	const mat y = ellipse_pattern->y();
	const mat theta = ellipse_pattern->rotation();

	// 长轴和短轴
	const double major_radius = radius / sqrt(sqrt(1 - eccentricity * eccentricity));
	const double minor_radius = radius * sqrt(sqrt(1 - eccentricity * eccentricity));

	// 绘图
	for (int r = 0; r < x.n_rows; ++r)
	{
		for (int c = 0; c < x.n_cols; ++c)
		{
			painter.save();
			painter.translate(x(r,c), y(r,c));
			painter.rotate(theta(r,c));

			QPainterPath path;
			path.addEllipse(QPointF(0, 0), major_radius, minor_radius);
			path.addEllipse(QPointF(0, 0), ratio*major_radius, ratio*minor_radius);
			painter.drawPath(path);
			painter.restore();
		}

	}

	return pixmap;

}




QPixmap RenderPatternPixmap::RenderPolygonPatternPixmap(PolygonSpecklePattern* polygon_pattern, const int& height, const int& width,
	const QPainter::RenderHint& render_hint, const Qt::GlobalColor& background_color)
{
	// 初始化 pixmap
	QPixmap pixmap(width, height);

	QPainter painter(&pixmap);
	painter.setPen(Qt::NoPen);

	if (render_hint == QPainter::Antialiasing)
		painter.setRenderHint(QPainter::Antialiasing, true);
	else
		painter.setRenderHint(QPainter::Antialiasing, false);

	if (background_color == Qt::black) {
		pixmap.fill(Qt::black);
		painter.setBrush(QBrush(Qt::white));
	}
	else {
		pixmap.fill(Qt::white);
		painter.setBrush(QBrush(Qt::black));
	}

	// 散斑参数
	const double radius = polygon_pattern->radius();
	const double ratio = polygon_pattern->ratio();
	const mat x = polygon_pattern->x();
	const mat y = polygon_pattern->y();
	const mat theta = polygon_pattern->rotation();
	const imat edges = polygon_pattern->edges();


	// 绘图
	for (int r = 0; r < x.n_rows; ++r)
	{
		for (int c = 0; c < x.n_cols; ++c)
		{
			painter.save();
			painter.translate(x(r, c), y(r, c));
			painter.rotate(theta(r, c));

			const int edge = edges(r, c);
			const double delta = 2 * datum::pi / edge;
			QPolygonF outer_polygon(edge);
			QPolygonF inner_polygon(edge);
			for (int j = 0; j < edge; ++j) 
			{
				QPointF& outer_point = outer_polygon[j];
				QPointF& inner_point = inner_polygon[j];
				double angle = j * delta;
				double xx = radius * cos(angle);
				double yy = radius * sin(angle);
				outer_point.setX(xx);
				outer_point.setY(yy);
				inner_point.setX(ratio*xx);
				inner_point.setY(ratio*yy);
			}

			QPainterPath path;
			path.addPolygon(outer_polygon);
			path.addPolygon(inner_polygon);
			painter.drawPath(path);
			painter.restore();

		}

	}

	return pixmap;

}



QPixmap RenderPatternPixmap::RenderGaussianPatternPixmap(GaussianPattern* gaussian_pattern, const int& height, const int& width,
	const Qt::GlobalColor& background_color)
{
	// 前景和背景色
	double bc, fc;
	if (background_color == Qt::black) 
	{
		bc = 0;
		fc = 255;
	}
	else 
	{
		fc = 0;
		bc = 255;
	}

	// 绘制计算灰度
	//mat data = gaussian_pattern->Values(width, height);
	mat data = gaussian_pattern->EstimateValues(width, height);

	// 量化
	const double minimum = data.min();
	const double maximum = data.max();
	const double k = (fc - bc) / (maximum - minimum);
	
	mat q = k * (data - minimum) + bc;
	q = round(q);


	// 转化数据格式
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

	delete[] img;

	return pixmap;

}



QPixmap RenderPatternPixmap::RenderGaussianPatternPixmap(GaussianPattern* gaussian_pattern, const mat& sample_x, const mat& sample_y,
	const Qt::GlobalColor& background_color, bool using_approximation)
{
	// 图像尺寸
	const int height = sample_x.n_rows;
	const int width = sample_x.n_cols;

	// 前景和背景色
	double bc, fc;
	if (background_color == Qt::black)
	{
		bc = 0;
		fc = 255;
	}
	else
	{
		fc = 0;
		bc = 255;
	}

	// 绘制计算灰度
	mat data;
	if (using_approximation)
		data = gaussian_pattern->EstimateValues(sample_x, sample_y);
	else
		data = gaussian_pattern->Values(sample_x, sample_y);

	// 量化
	const double minimum = data.min();
	const double maximum = data.max();
	const double k = (fc - bc) / (maximum - minimum);

	mat q = k * (data - minimum) + bc;
	q = round(q);


	// 转化数据格式
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

	delete[] img;

	return pixmap;

}




void RenderPatternPixmap::MeshGrid(const int& xmin, const int& ymin, const int& ncols, const int& nrows, mat& xmat, mat& ymat)
{
	const int xmax = xmin + ncols - 1;
	const int ymax = ymin + nrows - 1;

	const rowvec xvec = regspace<rowvec>(xmin, xmax);
	const vec yvec = regspace<vec>(ymin, ymax);

	xmat = repmat(xvec, nrows, 1);
	ymat = repmat(yvec, 1, ncols);

}
