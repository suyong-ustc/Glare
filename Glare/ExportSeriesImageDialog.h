#pragma once

#include <QDialog>
#include <QSize>
#include <QTabWidget>
#include "GaussianPattern.h"
#include "ExportSeriesImageWidget_Translation.h"
#include "ExportSeriesImageWidget_Elongation.h"
#include "ExportSeriesImageWidget_Sinusoidal.h"
#include "ExportSeriesImageWidget_Gaussian.h"
#include "ExportSeriesImageWidget_PLC.h"
#include "ExportSeriesImageWidget_Rotation.h"
#include "XianType.h"



class ExportSeriesImageDialog : public QDialog
{
	Q_OBJECT

public:
	ExportSeriesImageDialog(QWidget *parent = Q_NULLPTR);
	~ExportSeriesImageDialog();

	void setCurrentTab(const TransformType& transform_type)
	{
		tab_widget_->setCurrentIndex(static_cast<int>(transform_type));
	}

	void setPattern(GaussianPattern* gaussian_pattern);
	void setImageSize(const QSize& image_size);


private:

	// µ¼³öÐòÁÐÍ¼Æ¬
	QTabWidget* tab_widget_;
	ExportSeriesImageWidget_Translation* widget_export_series_translation_;
	ExportSeriesImageWidget_Elongation* widget_export_series_elongation_;
	ExportSeriesImageWidget_Sinusoidal* widget_export_series_sinusoidal_;
	ExportSeriesImageWidget_Gaussian* widget_export_series_gaussian_;
	ExportSeriesImageWidget_PLC* widget_export_series_plc_;
	ExportSeriesImageWidget_Rotation* widget_export_series_rotation_;


};
