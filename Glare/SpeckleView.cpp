#include "SpeckleView.h"
#include <QScrollBar>


SpeckleView::SpeckleView(QWidget *parent)
	: QGraphicsView(parent)
{
	// �Ƿ�������ק
	is_draging_ = false;

	drag_start_x_ = 0;
	drag_start_y_ = 0;

}

SpeckleView::~SpeckleView()
{
}



void SpeckleView::mousePressEvent(QMouseEvent* event)
{
	// �����λ��
	QPointF point = mapToScene(event->pos());

	if (event->button() == Qt::MidButton)
	{
		is_draging_ = true;

		drag_start_x_ = event->x();
		drag_start_y_ = event->y();

		setCursor(Qt::ClosedHandCursor);
	}

	QGraphicsView::mousePressEvent(event);
}



void SpeckleView::mouseMoveEvent(QMouseEvent* event)
{
	if (is_draging_)
	{
		qreal dx = event->x() - drag_start_x_;
		qreal dy = event->y() - drag_start_y_;

		qreal a = horizontalScrollBar()->value();
		qreal b = verticalScrollBar()->value();

		horizontalScrollBar()->setValue(horizontalScrollBar()->value() - dx);
		verticalScrollBar()->setValue(verticalScrollBar()->value() - dy);

		drag_start_x_ = event->x();
		drag_start_y_ = event->y();
	}

	QGraphicsView::mouseMoveEvent(event);
}



void SpeckleView::mouseReleaseEvent(QMouseEvent* event)
{
	QPointF point = mapToScene(event->pos());

	if (event->button() == Qt::MidButton)
	{
		is_draging_ = false;
		setCursor(Qt::ArrowCursor);

	}

	QGraphicsView::mouseReleaseEvent(event);
}



void SpeckleView::wheelEvent(QWheelEvent* event)
{
	// �¼��� view �е�����
	QPoint view_pos = event->pos();

	// �¼��� scene �е�����
	QPointF scene_pos = mapToScene(view_pos);

	// ��������
	centerOn(scene_pos);

	// ʹ�ù��ַŴ���С
	if (event->delta() > 0)
	{
		scale(1.25, 1.25);
	}
	else
	{
		scale(0.8, 0.8);
	}

	QPointF delta = mapToScene(view_pos) - mapToScene(viewport()->rect().center());

	centerOn(scene_pos - delta);
}



void SpeckleView::keyPressEvent(QKeyEvent* event)
{
	if (event->key() == Qt::Key_0)
	{
		if (event->modifiers() & Qt::ControlModifier)
		{
			fitInView(scene()->sceneRect(), Qt::KeepAspectRatio);
		}
	}
	else if (event->key() == Qt::Key_1)
	{
		if (event->modifiers() & Qt::ControlModifier)
		{
			resetTransform();
		}
	}

	QGraphicsView::keyPressEvent(event);
}

