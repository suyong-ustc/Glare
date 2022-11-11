#pragma once

#include <QGraphicsItem>
#include <QPainter>


class EllipseSpeckleItem : public QGraphicsItem
{
public:
	EllipseSpeckleItem(double major_radius, double minor_radius, double diameter_ratio, QColor color);
	~EllipseSpeckleItem();

private:
	QRectF boundingRect() const;
	void paint(QPainter* painter, const QStyleOptionGraphicsItem* options, QWidget* widget);

	double major_radius_;
	double minor_radius_;
	double diameter_ratio_;
	QColor color_;
};
