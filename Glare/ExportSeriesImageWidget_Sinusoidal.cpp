#include "ExportSeriesImageWidget_Sinusoidal.h"
#include <QFileDialog>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QVBoxLayout>
#include <armadillo>
#include "InverseMap.h"
#include "Pattern\RenderPatternPixmap.h"
using namespace arma;



ExportSeriesImageWidget_Sinusoidal::ExportSeriesImageWidget_Sinusoidal(QWidget *parent)
	: QWidget(parent)
{
	setupUI();
	connectSlots();
}


ExportSeriesImageWidget_Sinusoidal::~ExportSeriesImageWidget_Sinusoidal()
{

}


void ExportSeriesImageWidget_Sinusoidal::connectSlots()
{
	bool is_ok = false;

	is_ok = connect(spinbox_a_total_number_, QOverload<int>::of(&QSpinBox::valueChanged),
					this, &ExportSeriesImageWidget_Sinusoidal::SlotATotalNumberChanged);

	is_ok = connect(spinbox_T_total_number_, QOverload<int>::of(&QSpinBox::valueChanged),
					this, &ExportSeriesImageWidget_Sinusoidal::SlotTTotalNumberChanged);

	is_ok = connect(spinbox_b_total_number_, QOverload<int>::of(&QSpinBox::valueChanged),
					this, &ExportSeriesImageWidget_Sinusoidal::SlotBTotalNumberChanged);

	is_ok = connect(pushbutton_save_series_images_, &QPushButton::clicked,
					this, &ExportSeriesImageWidget_Sinusoidal::SlotSaveSeriesImages);

}



void ExportSeriesImageWidget_Sinusoidal::setupUI()
{
	QLabel* label_parameter = new QLabel(tr("Displacement : u = a sin(2*pi*x/T + b)"));

	QGridLayout* parameter_layout = new QGridLayout;

	QLabel* label_start = new QLabel(tr("Start"));
	QLabel* label_end = new QLabel(tr("End"));
	QLabel* label_total_number = new QLabel(tr("Total number"));
	parameter_layout->addWidget(label_start, 0, 1);
	parameter_layout->addWidget(label_end, 0, 2);
	parameter_layout->addWidget(label_total_number, 0, 3);

	QLabel* label_a = new QLabel("a");
	doublespinbox_a_start_ = new QDoubleSpinBox;
	doublespinbox_a_start_->setRange(-10, 10);
	doublespinbox_a_start_->setValue(5);
	doublespinbox_a_end_ = new QDoubleSpinBox;
	doublespinbox_a_end_->setRange(-10, 10);
	doublespinbox_a_end_->setValue(5);
	spinbox_a_total_number_ = new QSpinBox;
	spinbox_a_total_number_->setRange(2, 100);
	spinbox_a_total_number_->setValue(11);
	parameter_layout->addWidget(label_a, 1, 0);
	parameter_layout->addWidget(doublespinbox_a_start_, 1, 1);
	parameter_layout->addWidget(doublespinbox_a_end_, 1, 2);
	parameter_layout->addWidget(spinbox_a_total_number_, 1, 3);

	QLabel* label_T = new QLabel("T");
	doublespinbox_T_start_ = new QDoubleSpinBox;
	doublespinbox_T_start_->setRange(10, 500);
	doublespinbox_T_start_->setValue(100);
	doublespinbox_T_end_ = new QDoubleSpinBox;
	doublespinbox_T_end_->setRange(10, 500);
	doublespinbox_T_end_->setValue(100);
	spinbox_T_total_number_ = new QSpinBox;
	spinbox_T_total_number_->setRange(2, 100);
	spinbox_T_total_number_->setValue(11);
	parameter_layout->addWidget(label_T, 2, 0);
	parameter_layout->addWidget(doublespinbox_T_start_, 2, 1);
	parameter_layout->addWidget(doublespinbox_T_end_, 2, 2);
	parameter_layout->addWidget(spinbox_T_total_number_, 2, 3);

	QLabel* label_b = new QLabel("b");
	doublespinbox_b_start_ = new QDoubleSpinBox;
	doublespinbox_b_start_->setRange(-50, 50);
	doublespinbox_b_start_->setValue(-5);
	doublespinbox_b_end_ = new QDoubleSpinBox;
	doublespinbox_b_end_->setRange(-50, 50);
	doublespinbox_b_end_->setValue(5);
	spinbox_b_total_number_ = new QSpinBox;
	spinbox_b_total_number_->setRange(2, 100);
	spinbox_b_total_number_->setValue(11);
	parameter_layout->addWidget(label_b, 3, 0);
	parameter_layout->addWidget(doublespinbox_b_start_, 3, 1);
	parameter_layout->addWidget(doublespinbox_b_end_, 3, 2);
	parameter_layout->addWidget(spinbox_b_total_number_, 3, 3);

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



void ExportSeriesImageWidget_Sinusoidal::SlotSaveSeriesImages()
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

	const double T_start = doublespinbox_T_start_->value();
	const double T_end = doublespinbox_T_end_->value();
	const vec T_vec = linspace(T_start, T_end, image_total_number);

	const double b_start = doublespinbox_b_start_->value();
	const double b_end = doublespinbox_b_end_->value();
	const vec b_vec = linspace(b_start, b_end, image_total_number);

	// 设置进度条
	progressbar_save_series_images_->setMaximum(image_total_number);
	progressbar_save_series_images_->setValue(0);

	// 采样点在变形图中的位置
	rowvec deform_x = regspace<rowvec>(0, width - 1);
	vec refer_y = regspace<vec>(0, height - 1);

	// 存储图片
	for (int i = 0; i < image_total_number; ++i)
	{
		// 变形参数
		const double a = a_vec(i);
		const double T = T_vec(i);
		const double b = b_vec(i);

		// 采样点在参考图中的位置
		rowvec refer_x = InverseMap::InverseMap_SinusoidalDeformation(a, T, b, width);
		const mat xmat = repmat(refer_x, height, 1);
		const mat ymat = repmat(refer_y, 1, width);

		// 生成图像
		QPixmap pixmap = RenderPatternPixmap::RenderGaussianPatternPixmap(gaussian_pattern_, xmat, ymat, Qt::black, false);

		// 存储图像
		QString image_name = QString("%1_%2.bmp").arg(prefix).arg(i, 3, 10, QLatin1Char('0'));
		pixmap.save(image_name, "BMP");

		// 设置进度条
		progressbar_save_series_images_->setValue(i + 1);

	}


}



void ExportSeriesImageWidget_Sinusoidal::SlotATotalNumberChanged(int total_number)
{
	spinbox_T_total_number_->setValue(total_number);
	spinbox_b_total_number_->setValue(total_number);
}



void ExportSeriesImageWidget_Sinusoidal::SlotTTotalNumberChanged(int total_number)
{
	spinbox_a_total_number_->setValue(total_number);
	spinbox_b_total_number_->setValue(total_number);
}



void ExportSeriesImageWidget_Sinusoidal::SlotBTotalNumberChanged(int total_number)
{
	spinbox_a_total_number_->setValue(total_number);
	spinbox_T_total_number_->setValue(total_number);
}