#pragma once

#include <QWidget>
#include <QAction>
#include <QDoubleSpinBox>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QMenu>
#include <QPixmap>
#include <opencv2/core.hpp>


class AssessWidget_Coverage : public QWidget
{
	Q_OBJECT

public:
	AssessWidget_Coverage(QWidget *parent = Q_NULLPTR);
	~AssessWidget_Coverage();

private:
	QDoubleSpinBox* doublespinbox_threshold_;
	QDoubleSpinBox* doublespinbox_coverage_;

	QGraphicsScene* scene_;
	QGraphicsView* view_;

	cv::Mat image_;

private:
	void setupUI();
	void connectSlots();

public slots:
	void SlotNewImageImported(uchar* data, const int& h, const int& w);
	void SlotRenderBinaryImage(double threshold);
};
