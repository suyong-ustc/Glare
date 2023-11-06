#pragma once

#include <QDialog>
#include <QDoubleSpinBox>
#include <QProgressBar>
#include <QPushButton>
#include <QSize>
#include <QSpinBox>
#include "Pattern\GaussianPattern.h"


class ExportSeriesImageWidget_Translation : public QWidget
{
	Q_OBJECT

public:
	ExportSeriesImageWidget_Translation(QWidget *parent = Q_NULLPTR);
	~ExportSeriesImageWidget_Translation();

	void setPattern(GaussianPattern* gaussian_pattern) { gaussian_pattern_ = gaussian_pattern; }
	void setImageSize(const QSize& image_size) { image_size_ = image_size; }

private:
	QDoubleSpinBox* doublespinbox_u0_start_;
	QDoubleSpinBox* doublespinbox_u0_end_;
	QSpinBox* spinbox_u0_count_;

	QPushButton* pushbutton_save_series_images_;
	QProgressBar* progressbar_save_series_images_;

	GaussianPattern* gaussian_pattern_;
	QSize image_size_;

	void setupUI();
	void connectSlots();

public slots:
	void SlotSaveSeriesImages();

};
