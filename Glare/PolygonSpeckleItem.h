#pragma once

#include <QGraphicsItem>
#include <QPainter>

class PolygonSpeckleItem : public QGraphicsItem
{
public:
	PolygonSpeckleItem(double diameter, int edge, double diameter_ratio, QColor color);
	~PolygonSpeckleItem();

private:
	QRectF boundingRect() const;
	void paint(QPainter* painter, const QStyleOptionGraphicsItem* options, QWidget* widget);

	double diameter_;
	int edge_;
	double diameter_ratio_;
	QColor color_;
};
