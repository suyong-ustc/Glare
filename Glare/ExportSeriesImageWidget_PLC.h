#pragma once

#include <QDialog>
#include <QDoubleSpinBox>
#include <QProgressBar>
#include <QPushButton>
#include "GaussianPattern.h"


class ExportSeriesImageWidget_PLC : public QWidget
{
	Q_OBJECT

public:
	ExportSeriesImageWidget_PLC(QWidget *parent = Q_NULLPTR);
	~ExportSeriesImageWidget_PLC();

	void setPattern(GaussianPattern* gaussian_pattern) { gaussian_pattern_ = gaussian_pattern; }
	void setImageSize(const QSize& image_size) { image_size_ = image_size; }

private:

	QDoubleSpinBox* doublespinbox_a_start_;
	QDoubleSpinBox* doublespinbox_a_end_;
	QSpinBox* spinbox_a_total_number_;

	QDoubleSpinBox* doublespinbox_x0_start_;
	QDoubleSpinBox* doublespinbox_x0_end_;
	QSpinBox* spinbox_x0_total_number_;

	QDoubleSpinBox* doublespinbox_c_start_;
	QDoubleSpinBox* doublespinbox_c_end_;
	QSpinBox* spinbox_c_total_number_;

	QPushButton* pushbutton_save_series_images_;
	QProgressBar* progressbar_save_series_images_;

	GaussianPattern* gaussian_pattern_;
	QSize image_size_;

	void setupUI();
	void connectSlots();

public slots:
	void SlotSaveSeriesImages();

private slots:
	void SlotATotalValueChanged(int);
	void SlotX0TotalValueChanged(int);
	void SlotCTotalValueChanged(int);

};
