#pragma once

#include <QGraphicsView>
#include <QMouseEvent>


class SpeckleView : public QGraphicsView
{
	Q_OBJECT

public:
	SpeckleView(QWidget *parent = Q_NULLPTR);
	~SpeckleView();

protected:

	virtual void mousePressEvent(QMouseEvent* event);

	virtual void mouseMoveEvent(QMouseEvent* event);

	virtual void mouseReleaseEvent(QMouseEvent* event);

	virtual void wheelEvent(QWheelEvent* event);

	virtual void keyPressEvent(QKeyEvent* event);


private:

	// ÊÇ·ñÕıÔÚÍÏ×§Í¼Ïñ
	bool is_draging_;

	qreal drag_start_x_;
	qreal drag_start_y_;
	
};
