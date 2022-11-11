#pragma once

#include <QWidget>
#include <opencv2/core.hpp>
#include "qcustomplot.h"


class AssessWidget_InterpolationBias : public QWidget
{
	Q_OBJECT

public:
	AssessWidget_InterpolationBias(QWidget *parent = Q_NULLPTR);
	~AssessWidget_InterpolationBias();

private:
	QCustomPlot* graph_;
	QCPColorMap *color_map_;
	QCPMarginGroup *margin_group_;

	QComboBox* combobox_interpolation_type_;
	QDoubleSpinBox* doublespinbox_interpolation_bias_;

	cv::Mat image_;
	cv::Mat power_spectra_;

private:
	void setupUI();
	void setupGraph();
	void connectSlots();

	void ShowPowerSpectrum();
	void SetInterpolationBias();

public slots:
	void SlotNewImageImported(uchar* data, int h, int w);
	void SlotCalculateInterpolationBias(int interp_id);
};
