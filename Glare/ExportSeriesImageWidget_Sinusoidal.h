#pragma once

#include <QDialog>
#include <QDoubleSpinBox>
#include <QProgressBar>
#include <QPushButton>
#include <QSpinBox>
#include "GaussianPattern.h"


class ExportSeriesImageWidget_Sinusoidal : public QWidget
{
	Q_OBJECT

public:
	ExportSeriesImageWidget_Sinusoidal(QWidget *parent = Q_NULLPTR);
	~ExportSeriesImageWidget_Sinusoidal();

	void setPattern(GaussianPattern* gaussian_pattern) { gaussian_pattern_ = gaussian_pattern; }
	void setImageSize(const QSize& image_size) { image_size_ = image_size; }

private:

	QDoubleSpinBox* doublespinbox_a_start_;
	QDoubleSpinBox* doublespinbox_a_end_;
	QSpinBox* spinbox_a_total_number_;

	QDoubleSpinBox* doublespinbox_T_start_;
	QDoubleSpinBox* doublespinbox_T_end_;
	QSpinBox* spinbox_T_total_number_;

	QDoubleSpinBox* doublespinbox_b_start_;
	QDoubleSpinBox* doublespinbox_b_end_;
	QSpinBox* spinbox_b_total_number_;

	QPushButton* pushbutton_save_series_images_;
	QProgressBar* progressbar_save_series_images_;

	GaussianPattern* gaussian_pattern_;
	QSize image_size_;

	void setupUI();
	void connectSlots();

public slots:
	void SlotSaveSeriesImages();

private slots:
	void SlotATotalNumberChanged(int);
	void SlotTTotalNumberChanged(int);
	void SlotBTotalNumberChanged(int);
};
