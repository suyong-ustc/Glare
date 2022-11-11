#include "ExportSeriesImageDialog.h"
#include <QVBoxLayout>



ExportSeriesImageDialog::ExportSeriesImageDialog(QWidget *parent)
	: QDialog(parent)
{
	setWindowTitle(tr("Save Series Images"));

	QVBoxLayout* layout = new QVBoxLayout;
	setLayout(layout);

	// µ¼³öÐòÁÐÍ¼Æ¬
	tab_widget_ = new QTabWidget(this);

	widget_export_series_translation_ = new ExportSeriesImageWidget_Translation();
	widget_export_series_elongation_ = new ExportSeriesImageWidget_Elongation();
	widget_export_series_sinusoidal_ = new ExportSeriesImageWidget_Sinusoidal();
	widget_export_series_gaussian_ = new ExportSeriesImageWidget_Gaussian();
	widget_export_series_plc_ = new ExportSeriesImageWidget_PLC();
	widget_export_series_rotation_ = new ExportSeriesImageWidget_Rotation();

	tab_widget_->addTab(widget_export_series_translation_, tr("Translation"));
	tab_widget_->addTab(widget_export_series_elongation_, tr("Stretch"));
	tab_widget_->addTab(widget_export_series_sinusoidal_, tr("Sinusoidal"));
	tab_widget_->addTab(widget_export_series_gaussian_, tr("Gaussian"));
	tab_widget_->addTab(widget_export_series_plc_, tr("PLC Band"));
	tab_widget_->addTab(widget_export_series_rotation_, tr("Rotation"));

	layout->addWidget(tab_widget_);
	layout->setContentsMargins(0, 0, 0, 0);
}


ExportSeriesImageDialog::~ExportSeriesImageDialog()
{

}



void ExportSeriesImageDialog::setPattern(GaussianPattern* gaussian_pattern) 
{ 
	widget_export_series_translation_->setPattern(gaussian_pattern);
	widget_export_series_elongation_->setPattern(gaussian_pattern);
	widget_export_series_sinusoidal_->setPattern(gaussian_pattern);
	widget_export_series_gaussian_->setPattern(gaussian_pattern);
	widget_export_series_plc_->setPattern(gaussian_pattern);
	widget_export_series_rotation_->setPattern(gaussian_pattern);

}


void ExportSeriesImageDialog::setImageSize(const QSize& image_size)
{
	widget_export_series_translation_->setImageSize(image_size);
	widget_export_series_elongation_->setImageSize(image_size);
	widget_export_series_sinusoidal_->setImageSize(image_size);
	widget_export_series_gaussian_->setImageSize(image_size);
	widget_export_series_plc_->setImageSize(image_size);
	widget_export_series_rotation_->setImageSize(image_size);

}