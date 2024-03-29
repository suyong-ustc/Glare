#pragma once

#include <QDialog>
#include <QDoubleSpinBox>
#include <QProgressBar>
#include <QPushButton>
#include <QSpinBox>
#include "Pattern\GaussianPattern.h"


class ExportSeriesImageWidget_Rotation : public QWidget
{
	Q_OBJECT

public:
	ExportSeriesImageWidget_Rotation(QWidget *parent = Q_NULLPTR);
	~ExportSeriesImageWidget_Rotation();

	void setPattern(GaussianPattern* gaussian_pattern) { gaussian_pattern_ = gaussian_pattern; }
	void setImageSize(const QSize& image_size) { image_size_ = image_size; }

private:

	QDoubleSpinBox* doublespinbox_a_start_;
	QDoubleSpinBox* doublespinbox_a_end_;
	QSpinBox* spinbox_a_total_number_;

	QDoubleSpinBox* doublespinbox_theta_start_;
	QDoubleSpinBox* doublespinbox_theta_end_;
	QSpinBox* spinbox_theta_total_number_;

	QDoubleSpinBox* doublespinbox_x0_start_;
	QDoubleSpinBox* doublespinbox_x0_end_;
	QSpinBox* spinbox_x0_total_number_;

	QDoubleSpinBox* doublespinbox_y0_start_;
	QDoubleSpinBox* doublespinbox_y0_end_;
	QSpinBox* spinbox_y0_total_number_;

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
	void SlotThetaTotalNumberChanged(int);
	void SlotX0TotalNumberChanged(int);
	void SlotY0TotalNumberChanged(int);
};
