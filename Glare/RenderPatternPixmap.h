#pragma once

#include <QObject>
#include <QPainter>
#include <QPixmap>
#include <armadillo>
#include "EllipseSpecklePattern.h"
#include "PolygonSpecklePattern.h"
#include "GaussianPattern.h"



class RenderPatternPixmap : public QObject
{
	Q_OBJECT

public:
	RenderPatternPixmap(QObject *parent);
	~RenderPatternPixmap();

	static QPixmap RenderEllipsePatternPixmap(EllipseSpecklePattern* ellipse_pattern, const int& height, const int& width, 
		const QPainter::RenderHint& render_hint, const Qt::GlobalColor& background_color);

	static QPixmap RenderPolygonPatternPixmap(PolygonSpecklePattern* polygon_pattern, const int& height, const int& width,
		const QPainter::RenderHint& render_hint, const Qt::GlobalColor& background_color);

	static QPixmap RenderGaussianPatternPixmap(GaussianPattern* gaussian_pattern, const int& height, const int& width, 
		const Qt::GlobalColor& background_color);

	static QPixmap RenderGaussianPatternPixmap(GaussianPattern* gaussian_pattern, const arma::mat& sample_x, const arma::mat& sample_y,
		const Qt::GlobalColor& background_color, bool using_approximation);

	static void MeshGrid(const int& xmin, const int& ymin, const int& ncols, const int& nrows, arma::mat& xmat, arma::mat& ymat);

};
