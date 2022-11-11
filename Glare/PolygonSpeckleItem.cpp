#include "PolygonSpeckleItem.h"
#define M_PI 3.141592653

PolygonSpeckleItem::PolygonSpeckleItem(double diameter, int edge, double diameter_ratio, QColor color)
	: diameter_(diameter), edge_(edge), diameter_ratio_(diameter_ratio), color_(color)
{

}


PolygonSpeckleItem::~PolygonSpeckleItem()
{

}


QRectF PolygonSpeckleItem::boundingRect() const
{
	double radius = 0.5 * diameter_;
	return QRectF(-radius, -radius, diameter_, diameter_);
}


void PolygonSpeckleItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* options, QWidget* widget)
{
	double radius = 0.5 * diameter_;
	double delta = 2 * M_PI / edge_;
	QPolygonF outer_polygon(edge_);
	QPolygonF inner_polygon(edge_);
	for (int i = 0; i < edge_; ++i) {
		QPointF& outer_point = outer_polygon[i];
		QPointF& inner_point = inner_polygon[i];
		double angle = i * delta;
		double x = radius * cos(angle);
		double y = radius * sin(angle);
		outer_point.setX(x);
		outer_point.setY(y);
		inner_point.setX(diameter_ratio_ * x);
		inner_point.setY(diameter_ratio_ * y);
	}

	painter->setPen(Qt::NoPen);
	painter->setBrush(QBrush(color_));
	QPainterPath path;
	path.addPolygon(outer_polygon);
	path.addPolygon(inner_polygon);
	painter->drawPath(path);
}