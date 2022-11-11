#include "EllipseSpeckleItem.h"


EllipseSpeckleItem::EllipseSpeckleItem(double major_radius, double minor_radius, double diameter_ratio, QColor color)
	: major_radius_(major_radius), minor_radius_(minor_radius), diameter_ratio_(diameter_ratio), color_(color)
{
	
}


EllipseSpeckleItem::~EllipseSpeckleItem()
{

}


QRectF EllipseSpeckleItem::boundingRect() const
{
	return QRectF(-major_radius_, -minor_radius_, 2 * major_radius_, 2 * minor_radius_);
}


void EllipseSpeckleItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* options, QWidget* widget)
{
	painter->setPen(Qt::NoPen);
	painter->setBrush(QBrush(color_));

	QPainterPath path;
	path.addEllipse(QPointF(0, 0), major_radius_, minor_radius_);
	path.addEllipse(QPointF(0, 0), diameter_ratio_ * major_radius_, diameter_ratio_ * minor_radius_);

	painter->drawPath(path);
}

