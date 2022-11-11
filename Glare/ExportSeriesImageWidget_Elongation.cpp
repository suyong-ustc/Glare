#include "ExportSeriesImageWidget_Elongation.h"
#include <QFileDialog.h>
#include <QGridLayout>
#include <QLabel>
#include <QVBoxLayout>
#include <armadillo>
#include "InverseMap.h"
#include "RenderPatternPixmap.h"
using namespace arma;


ExportSeriesImageWidget_Elongation::ExportSeriesImageWidget_Elongation(QWidget *parent)
	: QWidget(parent)
{
	setupUI();
	connectSlots();
}


ExportSeriesImageWidget_Elongation::~ExportSeriesImageWidget_Elongation()
{

}



void ExportSeriesImageWidget_Elongation::setupUI()
{
	QLabel* label_parameter = new QLabel(tr("Displacement : u = a (x - x0)"));

	QGridLayout* parameter_layout = new QGridLayout;

	QLabel* lable_start = new QLabel(tr("Start"));
	QLabel* label_end = new QLabel(tr("End"));
	QLabel* label_totol_number = new QLabel(tr("Total Number"));
	parameter_layout->addWidget(lable_start, 0, 1);
	parameter_layout->addWidget(label_end, 0, 2);
	parameter_layout->addWidget(label_totol_number, 0, 3);

	QLabel* label_a = new QLabel("a");
	doublespinbox_a_start_ = new QDoubleSpinBox;
	doublespinbox_a_start_->setRange(-3, 3);
	doublespinbox_a_start_->setDecimals(3);
	doublespinbox_a_start_->setSingleStep(0.1);
	doublespinbox_a_start_->setValue(-0.5);
	doublespinbox_a_end_ = new QDoubleSpinBox;
	doublespinbox_a_end_->setRange(-3, 3);
	doublespinbox_a_end_->setDecimals(3);
	doublespinbox_a_end_->setSingleStep(0.1);
	doublespinbox_a_end_->setValue(0.5);
	spinbox_a_total_number_ = new QSpinBox;
	spinbox_a_total_number_->setRange(2, 100);
	spinbox_a_total_number_->setValue(11);
	parameter_layout->addWidget(label_a, 1, 0);
	parameter_layout->addWidget(doublespinbox_a_start_, 1, 1);
	parameter_layout->addWidget(doublespinbox_a_end_, 1, 2);
	parameter_layout->addWidget(spinbox_a_total_number_, 1, 3);

	QLabel* label_x0 = new QLabel("x0");
	doublespinbox_x0_start_ = new QDoubleSpinBox;
	doublespinbox_x0_start_->setRange(-500, 500);
	doublespinbox_x0_start_->setDecimals(2);
	doublespinbox_x0_start_->setSingleStep(1);
	doublespinbox_x0_start_->setValue(150);
	doublespinbox_x0_end_ = new QDoubleSpinBox;
	doublespinbox_x0_end_->setRange(-500, 500);
	doublespinbox_x0_end_->setDecimals(2);
	doublespinbox_x0_end_->setSingleStep(1);
	doublespinbox_x0_end_->setValue(150);
	spinbox_x0_total_number_ = new QSpinBox;
	spinbox_x0_total_number_->setRange(2, 100);
	spinbox_x0_total_number_->setValue(11);
	parameter_layout->addWidget(label_x0, 2, 0);
	parameter_layout->addWidget(doublespinbox_x0_start_, 2, 1);
	parameter_layout->addWidget(doublespinbox_x0_end_, 2, 2);
	parameter_layout->addWidget(spinbox_x0_total_number_, 2, 3);

	QHBoxLayout* save_series_images_layout = new QHBoxLayout;
	pushbutton_save_series_images_ = new QPushButton(tr("Save series images"));
	progressbar_save_series_images_ = new QProgressBar();
	progressbar_save_series_images_->setMaximum(0);
	progressbar_save_series_images_->setMaximum(11);
	progressbar_save_series_images_->setValue(0);
	progressbar_save_series_images_->setAlignment(Qt::AlignHCenter);
	progressbar_save_series_images_->setFormat(tr("Progress: %v/%m"));
	save_series_images_layout->addWidget(pushbutton_save_series_images_);
	save_series_images_layout->addWidget(progressbar_save_series_images_);
	save_series_images_layout->setStretchFactor(pushbutton_save_series_images_, 1);
	save_series_images_layout->setStretchFactor(progressbar_save_series_images_, 2);

	QVBoxLayout* layout = new QVBoxLayout;
	layout->addWidget(label_parameter);
	layout->addSpacing(10);
	layout->addLayout(parameter_layout);
	layout->addStretch();
	layout->addLayout(save_series_images_layout);

	setLayout(layout);
}



void ExportSeriesImageWidget_Elongation::connectSlots()
{
	bool is_ok = false;

	is_ok = connect(spinbox_a_total_number_, QOverload<int>::of(&QSpinBox::valueChanged),
					this, &ExportSeriesImageWidget_Elongation::SlotATotalNumberChanged);

	is_ok = connect(spinbox_x0_total_number_, QOverload<int>::of(&QSpinBox::valueChanged),
					this, &ExportSeriesImageWidget_Elongation::SlotX0TotalNumberChanged);

	is_ok = connect(pushbutton_save_series_images_, &QPushButton::clicked,
					this, &ExportSeriesImageWidget_Elongation::SlotSaveSeriesImages);

}



void ExportSeriesImageWidget_Elongation::SlotSaveSeriesImages()
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
	const double a_start = doublespinbox_a_start_->value();
	const double a_end = doublespinbox_a_end_->value();
	const int image_total_number = spinbox_a_total_number_->value();
	const vec a_vec = linspace(a_start, a_end, image_total_number);

	const double x0_start = doublespinbox_x0_start_->value();
	const double x0_end = doublespinbox_x0_end_->value();
	const vec x0_vec = linspace(x0_start, x0_end, image_total_number);

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
		const double a = a_vec(i);
		const double x0 = x0_vec(i);

		// 采样点在参考图中的位置
		mat refer_x, refer_y;
		InverseMap::InverseMap_ElongtationDeformation(a, x0, deform_x, deform_y, refer_x, refer_y);

		// 生成图像
		QPixmap pixmap = RenderPatternPixmap::RenderGaussianPatternPixmap(gaussian_pattern_, refer_x, refer_y, Qt::black, false);

		// 存储图像
		QString image_name = QString("%1_%2.bmp").arg(prefix).arg(i, 3, 10, QLatin1Char('0'));
		pixmap.save(image_name, "BMP");

		// 设置进度条
		progressbar_save_series_images_->setValue(i + 1);
	}


}



void ExportSeriesImageWidget_Elongation::SlotATotalNumberChanged(int total_number)
{
	spinbox_x0_total_number_->setValue(total_number);
}



void ExportSeriesImageWidget_Elongation::SlotX0TotalNumberChanged(int total_number)
{
	spinbox_a_total_number_->setValue(total_number);
}