#include "ExportSeriesImageWidget_Rotation.h"
#include <QFileDialog>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QVBoxLayout>
#include <armadillo>
#include "InverseMap.h"
#include "Pattern\RenderPatternPixmap.h"
using namespace arma;



ExportSeriesImageWidget_Rotation::ExportSeriesImageWidget_Rotation(QWidget *parent)
	: QWidget(parent)
{
	setupUI();
	connectSlots();
}



ExportSeriesImageWidget_Rotation::~ExportSeriesImageWidget_Rotation()
{

}



void ExportSeriesImageWidget_Rotation::connectSlots()
{
	bool is_ok = false;

	is_ok = connect(spinbox_a_total_number_, QOverload<int>::of(&QSpinBox::valueChanged),
					this, &ExportSeriesImageWidget_Rotation::SlotATotalNumberChanged);

	is_ok = connect(spinbox_theta_total_number_, QOverload<int>::of(&QSpinBox::valueChanged),
					this, &ExportSeriesImageWidget_Rotation::SlotThetaTotalNumberChanged);

	is_ok = connect(spinbox_x0_total_number_, QOverload<int>::of(&QSpinBox::valueChanged),
					this, &ExportSeriesImageWidget_Rotation::SlotX0TotalNumberChanged);

	is_ok = connect(spinbox_y0_total_number_, QOverload<int>::of(&QSpinBox::valueChanged),
					this, &ExportSeriesImageWidget_Rotation::SlotY0TotalNumberChanged);

	is_ok = connect(pushbutton_save_series_images_, &QPushButton::clicked,
					this, &ExportSeriesImageWidget_Rotation::SlotSaveSeriesImages);
}



void ExportSeriesImageWidget_Rotation::setupUI()
{
	QLabel* label_parameter = new QLabel(tr("Location : (x' - x0, y' - y0) = a * (cos(theta), -sin(theta); sin(theta), cos(theta)) (x - x0, y - y0)"));

	QGridLayout* parameter_layout = new QGridLayout;

	QLabel* label_start = new QLabel(tr("Start"));
	QLabel* label_end = new QLabel(tr("End"));
	QLabel* label_total_number = new QLabel(tr("Total number"));
	parameter_layout->addWidget(label_start, 0, 1);
	parameter_layout->addWidget(label_end, 0, 2);
	parameter_layout->addWidget(label_total_number, 0, 3);

	QLabel* label_a = new QLabel("a");
	doublespinbox_a_start_ = new QDoubleSpinBox;
	doublespinbox_a_start_->setRange(0.01, 20);
	doublespinbox_a_start_->setValue(0.5);
	doublespinbox_a_end_ = new QDoubleSpinBox;
	doublespinbox_a_end_->setRange(0.01, 20);
	doublespinbox_a_end_->setValue(1.5);
	spinbox_a_total_number_ = new QSpinBox;
	spinbox_a_total_number_->setRange(2, 100);
	spinbox_a_total_number_->setValue(11);
	parameter_layout->addWidget(label_a, 1, 0);
	parameter_layout->addWidget(doublespinbox_a_start_, 1, 1);
	parameter_layout->addWidget(doublespinbox_a_end_, 1, 2);
	parameter_layout->addWidget(spinbox_a_total_number_, 1, 3);

	QLabel* label_theta = new QLabel("Theta");
	doublespinbox_theta_start_ = new QDoubleSpinBox;
	doublespinbox_theta_start_->setRange(0, 360);
	doublespinbox_theta_start_->setValue(0);
	doublespinbox_theta_end_ = new QDoubleSpinBox;
	doublespinbox_theta_end_->setRange(0, 360);
	doublespinbox_theta_end_->setValue(360);
	spinbox_theta_total_number_ = new QSpinBox;
	spinbox_theta_total_number_->setRange(2, 100);
	spinbox_theta_total_number_->setValue(11);
	parameter_layout->addWidget(label_theta, 2, 0);
	parameter_layout->addWidget(doublespinbox_theta_start_, 2, 1);
	parameter_layout->addWidget(doublespinbox_theta_end_, 2, 2);
	parameter_layout->addWidget(spinbox_theta_total_number_, 2, 3);

	QLabel* label_x0 = new QLabel("x0");
	doublespinbox_x0_start_ = new QDoubleSpinBox;
	doublespinbox_x0_start_->setRange(0, 500);
	doublespinbox_x0_start_->setValue(150);
	doublespinbox_x0_end_ = new QDoubleSpinBox;
	doublespinbox_x0_end_->setRange(0, 500);
	doublespinbox_x0_end_->setValue(150);
	spinbox_x0_total_number_ = new QSpinBox;
	spinbox_x0_total_number_->setRange(2, 100);
	spinbox_x0_total_number_->setValue(11);
	parameter_layout->addWidget(label_x0, 3, 0);
	parameter_layout->addWidget(doublespinbox_x0_start_, 3, 1);
	parameter_layout->addWidget(doublespinbox_x0_end_, 3, 2);
	parameter_layout->addWidget(spinbox_x0_total_number_, 3, 3);

	QLabel* label_y0 = new QLabel("y0");
	doublespinbox_y0_start_ = new QDoubleSpinBox;
	doublespinbox_y0_start_->setRange(-0, 500);
	doublespinbox_y0_start_->setValue(150);
	doublespinbox_y0_end_ = new QDoubleSpinBox;
	doublespinbox_y0_end_->setRange(0, 500);
	doublespinbox_y0_end_->setValue(150);
	spinbox_y0_total_number_ = new QSpinBox;
	spinbox_y0_total_number_->setRange(2, 100);
	spinbox_y0_total_number_->setValue(11);
	parameter_layout->addWidget(label_y0, 4, 0);
	parameter_layout->addWidget(doublespinbox_y0_start_, 4, 1);
	parameter_layout->addWidget(doublespinbox_y0_end_, 4, 2);
	parameter_layout->addWidget(spinbox_y0_total_number_, 4, 3);

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



void ExportSeriesImageWidget_Rotation::SlotSaveSeriesImages()
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

	const double theta_start = doublespinbox_theta_start_->value();
	const double theta_end = doublespinbox_theta_end_->value();
	const vec theta_vec = linspace(theta_start, theta_end, image_total_number);

	const double x0_start = doublespinbox_x0_start_->value();
	const double x0_end = doublespinbox_x0_end_->value();
	const vec x0_vec = linspace(x0_start, x0_end, image_total_number);

	const double y0_start = doublespinbox_y0_start_->value();
	const double y0_end = doublespinbox_y0_end_->value();
	const vec y0_vec = linspace(y0_start, y0_end, image_total_number);

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
		const double theta = theta_vec(i);
		const double x0 = x0_vec(i);
		const double y0 = y0_vec(i);

		// 采样点在参考图中的位置
		mat refer_x, refer_y;
		InverseMap::InverseMap_RotationDeformation(a, theta, x0, y0, deform_x, deform_y, refer_x, refer_y);

		// 生成图像
		QPixmap pixmap = RenderPatternPixmap::RenderGaussianPatternPixmap(gaussian_pattern_, refer_x, refer_y, Qt::black, false);

		// 存储图像
		QString image_name = QString("%1_%2.bmp").arg(prefix).arg(i, 3, 10, QLatin1Char('0'));
		pixmap.save(image_name, "BMP");

		// 设置进度条
		progressbar_save_series_images_->setValue(i + 1);

	}


}



void ExportSeriesImageWidget_Rotation::SlotATotalNumberChanged(int total_number)
{
	spinbox_theta_total_number_->setValue(total_number);
	spinbox_x0_total_number_->setValue(total_number);
	spinbox_y0_total_number_->setValue(total_number);
}



void ExportSeriesImageWidget_Rotation::SlotThetaTotalNumberChanged(int total_number)
{
	spinbox_a_total_number_->setValue(total_number);
	spinbox_x0_total_number_->setValue(total_number);
	spinbox_y0_total_number_->setValue(total_number);
}



void ExportSeriesImageWidget_Rotation::SlotX0TotalNumberChanged(int total_number)
{
	spinbox_a_total_number_->setValue(total_number);
	spinbox_theta_total_number_->setValue(total_number);
	spinbox_y0_total_number_->setValue(total_number);
}



void ExportSeriesImageWidget_Rotation::SlotY0TotalNumberChanged(int total_number)
{
	spinbox_a_total_number_->setValue(total_number);
	spinbox_theta_total_number_->setValue(total_number);
	spinbox_x0_total_number_->setValue(total_number);
}