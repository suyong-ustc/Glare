#include "ExportSeriesImageWidget_Translation.h"
#include <QFileDialog>
#include <QGridLayout>
#include <QLabel>
#include <QVBoxLayout>
#include "InverseMap.h"
#include "RenderPatternPixmap.h"
using namespace arma;


ExportSeriesImageWidget_Translation::ExportSeriesImageWidget_Translation(QWidget *parent)
	: QWidget(parent)
{
	setupUI();
	connectSlots();
}


ExportSeriesImageWidget_Translation::~ExportSeriesImageWidget_Translation()
{

}



void ExportSeriesImageWidget_Translation::setupUI()
{
	QVBoxLayout* layout = new QVBoxLayout;
	setLayout(layout);

	QLabel* label_parameter = new QLabel(tr("Displacement : u = u0"));
	layout->addWidget(label_parameter);
	layout->addSpacing(10);

	// 变形参数
	QGridLayout* parameter_layout = new QGridLayout;

	QLabel* label_start = new QLabel(tr("Start"));
	QLabel* label_end = new QLabel(tr("End"));
	QLabel* label_total_number = new QLabel(tr("Total number"));
	parameter_layout->addWidget(label_start, 0, 1);
	parameter_layout->addWidget(label_end, 0, 2);
	parameter_layout->addWidget(label_total_number, 0, 3);

	QLabel* label_u0 = new QLabel("u0");
	parameter_layout->addWidget(label_u0, 1, 0);

	doublespinbox_u0_start_ = new QDoubleSpinBox;
	doublespinbox_u0_start_->setRange(-100, 100);
	doublespinbox_u0_start_->setValue(0);
	doublespinbox_u0_start_->setSuffix(tr(" Pixel"));
	parameter_layout->addWidget(doublespinbox_u0_start_, 1, 1);

	doublespinbox_u0_end_ = new QDoubleSpinBox;
	doublespinbox_u0_end_->setRange(-100, 100);
	doublespinbox_u0_end_->setValue(1);
	doublespinbox_u0_end_->setSuffix(tr(" Pixel"));
	parameter_layout->addWidget(doublespinbox_u0_end_, 1, 2);

	spinbox_u0_count_ = new QSpinBox;
	spinbox_u0_count_->setRange(2, 100);
	spinbox_u0_count_->setValue(21);
	parameter_layout->addWidget(spinbox_u0_count_, 1, 3);

	layout->addLayout(parameter_layout);
	layout->addStretch();

	QHBoxLayout* save_series_images_layout = new QHBoxLayout;
	pushbutton_save_series_images_ = new QPushButton(tr("Save series images"));
	progressbar_save_series_images_ = new QProgressBar();
	progressbar_save_series_images_->setMaximum(0);
	progressbar_save_series_images_->setMaximum(21);
	progressbar_save_series_images_->setValue(0);
	progressbar_save_series_images_->setAlignment(Qt::AlignHCenter);
	progressbar_save_series_images_->setFormat(tr("Progress: %v/%m"));
	save_series_images_layout->addWidget(pushbutton_save_series_images_);
	save_series_images_layout->addWidget(progressbar_save_series_images_);
	save_series_images_layout->setStretchFactor(pushbutton_save_series_images_, 1);
	save_series_images_layout->setStretchFactor(progressbar_save_series_images_, 2);

	layout->addLayout(save_series_images_layout);
}



void ExportSeriesImageWidget_Translation::connectSlots()
{
	bool is_ok = false;

	is_ok = connect(pushbutton_save_series_images_, &QPushButton::clicked,
					this, &ExportSeriesImageWidget_Translation::SlotSaveSeriesImages);
}



void ExportSeriesImageWidget_Translation::SlotSaveSeriesImages()
{
	// 导出的文件名
	QFileDialog dialog(this);
	dialog.setWindowModality(Qt::WindowModal);
	dialog.setNameFilter(tr("Images (*.bmp)"));
	dialog.setAcceptMode(QFileDialog::AcceptSave);
	dialog.exec();
	QStringList files = dialog.selectedFiles();
	if (files.isEmpty())
		return;
	QString bmp_name = files.at(0);
	QString prefix = bmp_name.split(".")[0];

	// 图像尺寸
	const int width = image_size_.width();
	const int height = image_size_.height();

	// 读取变形序列参数
	const double u0_start = doublespinbox_u0_start_->value();
	const double u0_end = doublespinbox_u0_end_->value();
	const int image_total_number = spinbox_u0_count_->value();

	vec u0_vec = linspace(u0_start, u0_end, image_total_number);

	// 设置进度条
	progressbar_save_series_images_->setMaximum(image_total_number);
	progressbar_save_series_images_->setValue(0);

	// 采样点在变形图中的位置
	mat deform_x, deform_y;
	RenderPatternPixmap::MeshGrid(0, 0, width, height, deform_x, deform_y);

	// 存储图片
	for (int i = 0; i < image_total_number; ++i)
	{
		// 变形参数
		const double u0 = u0_vec(i);

		// 采样点在参考图中的位置
		mat refer_x, refer_y;
		InverseMap::InverseMap_TranslationDeformation(u0, deform_x, deform_y, refer_x, refer_y);

		// 生成图像
		QPixmap pixmap = RenderPatternPixmap::RenderGaussianPatternPixmap(gaussian_pattern_, refer_x, refer_y, Qt::black, false);

		// 存储图像
		QString image_name = QString("%1_%2.bmp").arg(prefix).arg(i, 3, 10, QLatin1Char('0'));
		pixmap.save(image_name, "BMP");

		// 设置进度条
		progressbar_save_series_images_->setValue(i + 1);

	}

}