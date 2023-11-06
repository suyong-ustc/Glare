#pragma once

#include <QGraphicsScene>
#include "Pattern\EllipseSpecklePattern.h"
#include "Pattern\GaussianPattern.h"
#include "Pattern\PolygonSpecklePattern.h"
#include "XianType.h"


class PatternParameter;

class SpeckleScene : public QGraphicsScene
{
	Q_OBJECT

public:
	SpeckleScene();
	~SpeckleScene();

	void PaintEllipseSpecklePattern(EllipseSpecklePattern* ellipse_pattern, const BackgroundColor& background);
	void PaintPolygonSpecklePattern(PolygonSpecklePattern* polygon_pattern, const BackgroundColor& background);
	void PaintGaussianSpecklePattern(GaussianPattern* gaussian_pattern, const int& height, const int& width, const BackgroundColor& background, const double& err = 1e-5);

};