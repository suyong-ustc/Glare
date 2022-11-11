#pragma once

#include <QWidget>
#include <QComboBox>
#include <QDoubleSpinBox>
#include <QSpinBox>
#include "qcustomplot.h"
#include <armadillo>


class AssessWidget_SSSIG : public QWidget
{
	Q_OBJECT

public:
	AssessWidget_SSSIG(QWidget *parent = Q_NULLPTR);
	~AssessWidget_SSSIG();

private:
	QComboBox* combobox_displacement_direction_;
	QSpinBox* spinbox_subset_size_;
	QDoubleSpinBox* doublespinbox_noise_level_;
	QComboBox* combobox_shape_function_order_;
	QDoubleSpinBox* doublespinbox_mig_;

	QCustomPlot* graph_;
	QCPColorMap *color_map_;
	QCPColorScale *color_scale_;
	QCPMarginGroup *margin_group_;

	arma::mat grad_integral_image_x_;
	arma::mat grad_integral_image_y_;

private:
	void setupUI();
	void setupGraph();
	void connectSlots();

	// 在界面上显示灰度梯度平方和
	void ShowSSSIG(const arma::mat& random_error, const int& xmin, const int& xmax, const int& ymin, const int& ymax);

public slots:
	void SlotNewImageImported(uchar* data, int h, int w);
	void SlotUpdate();

};
