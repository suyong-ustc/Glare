#pragma once

#include <QWidget>
#include <QDoubleSpinBox>
#include <QSpinBox>
#include <armadillo>
#include "qcustomplot.h"


class AssessWidget_Autocorrelation : public QWidget
{
	Q_OBJECT

public:
	AssessWidget_Autocorrelation(QWidget *parent = Q_NULLPTR);
	~AssessWidget_Autocorrelation();

private:
	QCustomPlot* graph_;

	QSpinBox* spinbox_subset_size_;
	QSpinBox* spinbox_maximal_translation_;
	QDoubleSpinBox* doublespinbox_speckle_size_;

	arma::mat image_;
	arma::vec zncc_x_;
	arma::vec zncc_y_;

private:
	void setupUI();
	void connectSlots();

	// 显示自相关曲线
	void ShowAutoCorrelationCurves();

public slots:
	void SlotNewImageImported(uchar* data, const int& h, const int& w);
	void SlotRenderAutoCorrelation();
};
